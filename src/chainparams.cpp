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
    const char* pszTimestamp = "From values, perception is born. From perception, judgment arises. From judgment, choice is made. From choice, destiny unfolds.";
    CMutableTransaction txNew;
    txNew.version = 1; 
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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
        consensus.nSubsidyHalvingInterval = 210000;

        consensus.BIP34Height = 1; 
        consensus.BIP65Height = 1; 
        consensus.BIP66Height = 1; 
        consensus.CSVHeight = 1;   
        consensus.SegwitHeight = 0; 

        consensus.powLimit = uint256("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 3 * 60 * 60; 
        consensus.nPowTargetSpacing = 5 * 60; 
        consensus.fPowAllowMinDifficultyBlocks = true; 
        consensus.fPowNoRetargeting = false; 

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;

        genesis = CreateGenesisBlock(1753258714, 255602, 0x1f00ffff, 1, 50 * COIN); 
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("00003bc4a3307f5f082587474bc55258d4b8504bb54ed1e7434ad6f3cc25e453"));
        assert(genesis.hashMerkleRoot == uint256("ae41db9e1127f666272fa489303c037d3ba5573d4801312d0c5e26dc46fd351a"));

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
    }

    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, ChainTypeToString(chain)));
}

void SelectParams(const ChainType chain)
{
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
