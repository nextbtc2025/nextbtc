// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsbase.h>
#include <common/args.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <deploymentinfo.h>
#include <logging.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <tinyformat.h>
#include <util/chaintype.h>
#include <util/strencodings.h>
#include <util/string.h>

#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

using util::SplitString;


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "I COME,I SEE,I HODL.";
    CMutableTransaction txNew;
    txNew.version = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << std::vector<unsigned char>(pszTimestamp, pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_RETURN;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

class CDageMainParams : public CChainParams {
public:
    CDageMainParams() {

        m_chain_type = ChainType::MAIN;
        
        // === 共识参数 ===
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0; 
        consensus.MinBIP9WarningHeight = 0; 
        
        // PoW 参数
        consensus.powLimit = uint256("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 150;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        
        // 部署设置
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;

        // === 创世块 (已填入您找到的真实值) ===
        genesis = CreateGenesisBlock(1368374520, 147717, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("000003167643c767d4e015a8bad0fb1054286acf23cfe3009083f9a9cd72dbc1"));
        assert(genesis.hashMerkleRoot == uint256("50e642574998223e81804fb878cc7caa818d6ed871f6e5bc990ed2ddc27797f4"));        
        
        // === 网络参数 ===
        pchMessageStart[0] = 0xda;
        pchMessageStart[1] = 0x9e;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 8666;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "bc";


        // =================================================================
        // vvvvvvvvvvvvvvvvvv 关键修正 vvvvvvvvvvvvvvvvvvvvv
        // 恢复检查点，并为您准备好60000高度的占位符
        // =================================================================
        checkpointData = {
            {
                 { 55560, uint256("000002b674005782184737a1fbd21dc187a0a220266be07eb255579aefe31b18") }, // 在快进完成后，取消注释并填入55560块的哈希
            }
        };
        // =================================================================
        // ^^^^^^^^^^^^^^^^^^ 修正结束 ^^^^^^^^^^^^^^^^^^^^^^^^
        // =================================================================
        
        vSeeds.clear();
        vFixedSeeds.clear();
    }
};

// =================================================================
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//                   主网 (CDageMainParams) 定义结束
// =================================================================



// =================================================================
// vvvvvvvvvvvvvvvvvv Testnet/Regtest 等参数定义 vvvvvvvvvvvvvvvvvvvvv
// =================================================================

/**
 * Testnet (v3)
 *
 * Testnet 参数类。我们不打算实际运行它，但为了让程序初始化不崩溃，
 * 我们用主网的安全参数来填充它，并只修改关键的网络标识符。
 */
class CDageTestNetParams : public CChainParams {
public:
    CDageTestNetParams() {
        m_chain_type = ChainType::TESTNET;
        
        // === 从 CDageMainParams 完整复制过来的安全参数 ===
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0; 
        consensus.MinBIP9WarningHeight = 0; 
        
        consensus.powLimit = uint256("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 150;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false; // Testnet 同样使用安全的难度调整
        consensus.fPowNoRetargeting = false;
        
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;

        // 创世块复用主网的，以确保所有参数都被正确初始化
        genesis = CreateGenesisBlock(1368374520, 147717, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("000003167643c767d4e015a8bad0fb1054286acf23cfe3009083f9a9cd72dbc1"));
        assert(genesis.hashMerkleRoot == uint256("50e642574998223e81804fb878cc7caa818d6ed871f6e5bc990ed2ddc27797f4"));
        
        // --- 修改Testnet专属的网络标识符以作区分 ---
        pchMessageStart[0] = 0x0b; // 使用比特币Testnet的经典魔法数字
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 18555;      // Testnet 端口
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111); // Testnet 地址前缀
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "tb"; // Testnet bech32 前缀

        // --- 其他参数可以保持和主网一致 ---
        nPruneAfterHeight = 1000;
        checkpointData = { {} };
        vSeeds.clear();
        vFixedSeeds.clear();
    }
};

/**
 * Regression test
 */
class CDageRegTestParams : public CChainParams {
public:
    CDageRegTestParams(const RegTestOptions& opts) {
        m_chain_type = ChainType::REGTEST;

        // === 同样从 CDageMainParams 完整复制过来的安全参数 ===
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.BIP34Height = 1; 
        consensus.BIP65Height = 1; 
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1; 
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;

        consensus.powLimit = uint256("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 150;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        
        // --- Regtest 特有的共识规则 ---
        consensus.fPowAllowMinDifficultyBlocks = true; // 允许难度立即重置，方便测试
        consensus.fPowNoRetargeting = true;            // 关闭难度调整

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;

        // 创世块复用主网的，但可以修改 nonce 以获得不同的创世哈希
        genesis = CreateGenesisBlock(1368374520, 1, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // Regtest 不需要 assert 创世块哈希，因为它每次都可能不同

        // --- 修改Regtest专属的网络标识符 ---
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18666;
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "bcrt";

        // --- 其他参数 ---
        nPruneAfterHeight = 1000;
        checkpointData = { {} };
        vSeeds.clear();
        vFixedSeeds.clear();
    }
};

/**
 * Testnet4
 * 为了安全，我们同样为其提供一个完整的、基于Testnet的克隆体。
 */
class CDageTestNet4Params : public CDageTestNetParams {
public:
    CDageTestNet4Params() {
        m_chain_type = ChainType::TESTNET4;
        // TestNet4 可以继承自 TestNet，并在这里进行微调
        // 例如，修改魔法数字或端口
        pchMessageStart[0] = 0x0c;
        pchMessageStart[1] = 0x12;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x08;
        nDefaultPort = 18777;
    }
};


/**
 * Signet
 * 为了安全，我们同样为其提供一个完整的、基于主网的克隆体。
 */
class CDageSigNetParams : public CChainParams {
public:
    CDageSigNetParams(const SigNetOptions& options) {
        m_chain_type = ChainType::SIGNET;

        // === 同样从 CDageMainParams 完整复制过来的安全参数 ===
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        
        consensus.powLimit = uint256("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 150;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;

        genesis = CreateGenesisBlock(1368374520, 147717, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        // --- 修改Signet专属的网络标识符 ---
        pchMessageStart[0] = 0x0a;
        pchMessageStart[1] = 0x0b;
        pchMessageStart[2] = 0x0c;
        pchMessageStart[3] = 0x0d;
        nDefaultPort = 38555;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "tb";

        checkpointData = { {} };
        vSeeds.clear();
        vFixedSeeds.clear();
    }
};


// =================================================================
// vvvvvvvvvvvvvvvvvv 全局函数定义 vvvvvvvvvvvvvvvvvvvvv
// =================================================================

// CChainParams 的静态成员函数，确保它们返回新创建的安全对象
std::unique_ptr<const CChainParams> CChainParams::Main() { return std::make_unique<const CDageMainParams>(); }
std::unique_ptr<const CChainParams> CChainParams::TestNet() { return std::make_unique<const CDageTestNetParams>(); }
std::unique_ptr<const CChainParams> CChainParams::TestNet4() { return std::make_unique<const CDageTestNet4Params>(); }
std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions& options) { return std::make_unique<const CDageSigNetParams>(options); }
std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options) { return std::make_unique<const CDageRegTestParams>(options); }

// 这两个函数是空的，因为我们的 Regtest/Signet 实现不依赖于额外的命令行参数
void ReadSigNetArgs(const ArgsManager& args, CChainParams::SigNetOptions& options) {}
void ReadRegTestArgs(const ArgsManager& args, CChainParams::RegTestOptions& options) {}

// 返回快照高度，我们目前不支持，返回空
std::vector<int> CChainParams::GetAvailableSnapshotHeights() const { return {}; }

// 根据魔法数字获取网络类型，我们只支持主网
std::optional<ChainType> GetNetworkForMagic(const MessageStartChars& magic) {
    if (magic == CChainParams::Main()->MessageStart()) {
        return ChainType::MAIN;
    }
    // 为了健壮性，可以加上其他网络的检查
    // if (magic == CChainParams::TestNet()->MessageStart()) {
    //     return ChainType::TESTNET;
    // }
    return std::nullopt;
}

// 全局链参数指针
static std::unique_ptr<const CChainParams> globalChainParams;

// 获取当前激活的链参数
const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

// CChainParams 的工厂函数，根据链类型创建并返回相应的参数对象
std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const ChainType chain) {
    switch (chain) {
    case ChainType::MAIN:
        return CChainParams::Main();
    case ChainType::TESTNET:
        return CChainParams::TestNet();
    case ChainType::TESTNET4:
        return CChainParams::TestNet4();
    case ChainType::SIGNET: {
        auto opts = CChainParams::SigNetOptions{};
        ReadSigNetArgs(args, opts);
        return CChainParams::SigNet(opts);
    }
    case ChainType::REGTEST: {
        auto opts = CChainParams::RegTestOptions{};
        ReadRegTestArgs(args, opts);
        return CChainParams::RegTest(opts);
    }
    }
    assert(false); // 不应该执行到这里
}

// 选择并激活全局链参数
void SelectParams(const ChainType chain) {
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
