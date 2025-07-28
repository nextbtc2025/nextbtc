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

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "I COME,I SEE,I HODL. by @btcdage";
    CMutableTransaction txNew;
    txNew.version = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);

    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << std::vector<unsigned char>(pszTimestamp, pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript();

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

class CMainParams : public CChainParams {
public:
    CMainParams() {
        m_chain_type = ChainType::MAIN;
        consensus.nSubsidyHalvingInterval = 800000;

        consensus.BIP34Height = 1;
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;

        consensus.powLimit = uint256("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 150;
        consensus.nPowTargetSpacing = 150;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;

        genesis = CreateGenesisBlock(1368374520, 62980, 0x1f00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("0000b64ca9d334e6d745141102fd101fd872f7c47c24395d38451a55dea83c62"));
        assert(genesis.hashMerkleRoot == uint256("62f8061412d36ce62f8cfb09099bd602c459b2cf47f114d3c1a4abae79abd77e"));

        // <<<<<<<<<<<<<<<< 在这里开始添加 <<<<<<<<<<<<<<<<
        checkpointData = CChainParams::CheckpointData{
            {
                { 22222, uint256("0000020e6b8c1cb86af7abb5ad701a7801fa282286129605b9ff4774814aaac0")},
            }
        };
        // <<<<<<<<<<<<<<<< 在这里结束添加 <<<<<<<<<<<<<<<<     

        vSeeds.clear();
        vFixedSeeds.clear();
 
        pchMessageStart[0] = 0xda;
        pchMessageStart[1] = 0x9e;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 8555;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "bc";
    }
};

void ReadSigNetArgs(const ArgsManager& args, CChainParams::SigNetOptions& options)
{
}

void ReadRegTestArgs(const ArgsManager& args, CChainParams::RegTestOptions& options)
{
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const ChainType chain)
{
    if (chain == ChainType::MAIN) {
        return std::make_unique<const CMainParams>();
    } else {
        // 对于 testnet, regtest, signet 等，都返回一个 CMainParams 实例
        return std::make_unique<const CMainParams>();
    }
}

void SelectParams(const ChainType chain)
{
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
