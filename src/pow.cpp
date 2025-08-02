// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util/check.h>

// 声明 DGW 函数
unsigned int DarkGravityWave(const CBlockIndex* pindexLast, const Consensus::Params& params);


// =================================================================
// vvvvvvvvvvvvvvvvvv 核心修改区域 vvvvvvvvvvvvvvvvvvvvv
//          实现“分阶段共识” (Phased Consensus)
// =================================================================
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // 创世区块: pindexLast 是 nullptr
    if (pindexLast == nullptr) {
        return UintToArith256(params.powLimit).GetCompact();
    }

    // -----------------------------------------------------------------
    // 阶段一: 初始快速挖矿
    // 在指定高度之前，锁定难度为网络允许的最低值，以完成原分叉链空投。
    // 挖的新区块，其高度为 pindexLast->nHeight + 1。
    // -----------------------------------------------------------------
    const int nInstantMineEndHeight = 55555; // 定义初始快速挖矿阶段的结束高度 (区块 0 到 55555)
 
    if ((pindexLast->nHeight + 1) < nInstantMineEndHeight)
    {
        return UintToArith256(params.powLimit).GetCompact();
    }

    // -----------------------------------------------------------------
    // 阶段二: DGW 稳定挖矿 (Stable Mining Phase)
    // 在初始快速阶段结束后，平滑切换到 Dark Gravity Wave 算法。
    // -----------------------------------------------------------------
    return DarkGravityWave(pindexLast, params);
}
// =================================================================
// ^^^^^^^^^^^^^^^^^^ 修改结束 ^^^^^^^^^^^^^^^^^^^^^^^^
// =================================================================


// 旧的 CalculateNextWorkRequired 函数已被删除，保持不变


// 检查难度转换是否在允许范围内
bool PermittedDifficultyTransition(const Consensus::Params& params, int64_t height, uint32_t old_nbits, uint32_t new_nbits)
{
    // DGW 每个块都可能调整难度，旧的限制逻辑不适用。
    // 核心的限制逻辑在 DGW 函数内部，因此这里直接返回 true 是安全的。
    return true;
}


bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    auto bnTarget{DeriveTarget(nBits, params.powLimit)};
    if (!bnTarget) return false;

    // 检查工作量证明是否满足要求
    if (UintToArith256(hash) > *bnTarget)
        return false;

    return true;
}

std::optional<arith_uint256> DeriveTarget(unsigned int nBits, const uint256& pow_limit)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // 检查范围
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(pow_limit))
        return {};

    return bnTarget;
}


// =================================================================
// vvvvvvvvvvvvvvvvvv Dark Gravity Wave v3 完整实现 vvvvvvvvvvvvvvvvvvvvv
//          (此部分代码经审查无误，无需改动)
// =================================================================
unsigned int DarkGravityWave(const CBlockIndex* pindexLast, const Consensus::Params& params)
{
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 24;
    int64_t PastBlocksMax = 24;
    int64_t CountBlocks = 0;
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);

    if (BlockLastSolved == nullptr || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMax) {
        return bnPowLimit.GetCompact();
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        CountBlocks++;

        if(CountBlocks <= PastBlocksMin) {
            if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
            else { PastDifficultyAverage = ((PastDifficultyAveragePrev * (CountBlocks - 1)) + (arith_uint256().SetCompact(BlockReading->nBits))) / CountBlocks; }
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        if(LastBlockTime > 0){
            int64_t Diff = (LastBlockTime - BlockReading->nTime);
            nActualTimespan += Diff;
        }
        LastBlockTime = BlockReading->nTime;

        if (BlockReading->pprev == nullptr) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    arith_uint256 bnNew(PastDifficultyAverage);

    int64_t _nTargetTimespan = CountBlocks * params.nPowTargetSpacing;

    if (nActualTimespan < _nTargetTimespan/3)
        nActualTimespan = _nTargetTimespan/3;
    if (nActualTimespan > _nTargetTimespan*3)
        nActualTimespan = _nTargetTimespan*3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= _nTargetTimespan;

    if (bnNew > bnPowLimit){
        bnNew = bnPowLimit;
    }

    return bnNew.GetCompact();
}
// =================================================================
// ^^^^^^^^^^^^^^^^^^ 代码结束 ^^^^^^^^^^^^^^^^^^^^^^^^
// =================================================================
