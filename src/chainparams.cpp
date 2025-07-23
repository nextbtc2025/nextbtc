// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsbase.h>
#include <common/args.h>
#include <consensus/params.h>
#include <deploymentinfo.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <logging.h>
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
    const char* pszTimestamp = "From values, perception is born. From perception, judgment arises. From judgment, choice is made. From choice, destiny unfolds.";
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_DUP << OP_HASH160 << ParseHex("0000000000000000000000000000000000000000") << OP_EQUALVERIFY << OP_CHECKSIG;

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

void ReadSigNetArgs(const ArgsManager& args, CChainParams::SigNetOptions& options)
{
    if (!args.GetArgs("-signetseednode").empty()) {
        options.seeds.emplace(args.GetArgs("-signetseednode"));
    }
    if (!args.GetArgs("-signetchallenge").empty()) {
        const auto signet_challenge = args.GetArgs("-signetchallenge");
        if (signet_challenge.size() != 1) {
            throw std::runtime_error("-signetchallenge cannot be multiple values.");
        }
        const auto val{TryParseHex<uint8_t>(signet_challenge[0])};
        if (!val) {
            throw std::runtime_error(strprintf("-signetchallenge must be hex, not '%s'.", signet_challenge[0]));
        }
        options.challenge.emplace(*val);
    }
}

void ReadRegTestArgs(const ArgsManager& args, CChainParams::RegTestOptions& options)
{
    if (auto value = args.GetBoolArg("-fastprune")) options.fastprune = *value;
    if (HasTestOption(args, "bip94")) options.enforce_bip94 = true;

    for (const std::string& arg : args.GetArgs("-testactivationheight")) {
        const auto found{arg.find('@')};
        if (found == std::string::npos) {
            throw std::runtime_error(strprintf("Invalid format (%s) for -testactivationheight=name@height.", arg));
        }

        const auto value{arg.substr(found + 1)};
        int32_t height;
        if (!ParseInt32(value, &height) || height < 0 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Invalid height value (%s) for -testactivationheight=name@height.", arg));
        }

        const auto deployment_name{arg.substr(0, found)};
        if (const auto buried_deployment = GetBuriedDeployment(deployment_name)) {
            options.activation_heights[*buried_deployment] = height;
        } else {
            throw std::runtime_error(strprintf("Invalid name (%s) for -testactivationheight=name@height.", arg));
        }
    }

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams = SplitString(strDeployment, ':');
        if (vDeploymentParams.size() < 3 || 4 < vDeploymentParams.size()) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end[:min_activation_height]");
        }
        CChainParams::VersionBitsParameters vbparams{};
        if (!ParseInt64(vDeploymentParams[1], &vbparams.start_time)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &vbparams.timeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        if (vDeploymentParams.size() >= 4) {
            if (!ParseInt32(vDeploymentParams[3], &vbparams.min_activation_height)) {
                throw std::runtime_error(strprintf("Invalid min_activation_height (%s)", vDeploymentParams[3]));
            }
        } else {
            vbparams.min_activation_height = 0;
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                options.version_bits_parameters[Consensus::DeploymentPos(j)] = vbparams;
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld, min_activation_height=%d\n", vDeploymentParams[0], vbparams.start_time, vbparams.timeout, vbparams.min_activation_height);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

std::unique_ptr<const CChainParams> CChainParams::Main() {
    auto params = std::make_unique<CChainParams>();

    params->strNetworkID =  CBaseChainParams::MAIN;

    params->consensus.nSubsidyHalvingInterval = 210000;
    params->consensus.BIP16Exception = uint256();
    params->consensus.BIP34Height = 1;
    params->consensus.BIP65Height = 1;
    params->consensus.BIP66Height = 1;
    params->consensus.CSVHeight = 1;
    params->consensus.SegwitHeight = 0;
    params->consensus.MinBIP9WarningHeight = 0;

    params->consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    params->consensus.nPowTargetTimespan = 3 * 60 * 60;
    params->consensus.nPowTargetSpacing = 5 * 60;
    params->consensus.fPowAllowMinDifficultyBlocks = true;
    params->consensus.fPowNoRetargeting = false;
    params->consensus.nRuleChangeActivationThreshold = 15;
    params->consensus.nMinerConfirmationWindow = 20;

    for (int i = 0; i < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; i++) {
        params->consensus.vDeployments[i].bit = 28;
        params->consensus.vDeployments[i].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        params->consensus.vDeployments[i].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        params->consensus.vDeployments[i].min_activation_height = 0;
    }

    params->genesis = CreateGenesisBlock(1753258714, 255602, 0x1f00ffff, 1, 50 * COIN);
    params->consensus.hashGenesisBlock = params->genesis.GetHash();
    assert(params->consensus.hashGenesisBlock == uint256S("0x00005fb947b1797a2101f42b9c494970ecb596b5b2810016b7a019c06dcf828a"));
    assert(params->genesis.hashMerkleRoot == uint256S("0xca498ad0479e3f25eca1b4bc9d9a5eb0900dae7da60db5e69b9578e3803f2cc9"));

    params->pchMessageStart[0] = 0xda;
    params->pchMessageStart[1] = 0xge;
    params->pchMessageStart[2] = 0xb5;
    params->pchMessageStart[3] = 0xda;
    params->nDefaultPort = 8555;
    params->nPruneAfterHeight = 1000;
    params->m_assumed_blockchain_size = 1;
    params->m_assumed_chain_state_size = 1;
    params->vFixedSeeds.clear();
    params->vSeeds.clear();

    params->base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
    params->base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
    params->base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
    params->base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
    params->base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
    params->bech32_hrp = "bc";

    return params;
}

std::unique_ptr<const CChainParams> CChainParams::TestNet() { throw std::runtime_error("Testnet is not supported in this build."); }
std::unique_ptr<const CChainParams> CChainParams::TestNet4() { throw std::runtime_error("Testnet4 is not supported in this build."); }
std::unique_ptr<const CChainParams> CChainParams::SigNet(const CChainParams::SigNetOptions& options) { throw std::runtime_error("Signet is not supported in this build."); }
std::unique_ptr<const CChainParams> CChainParams::RegTest(const CChainParams::RegTestOptions& options) { throw std::runtime_error("Regtest is not supported in this build."); }


static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const ChainType chain)
{
    switch (chain) {
    case ChainType::MAIN:
        return CChainParams::Main();
    default:
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, ChainTypeToString(chain)));
    }
}

void SelectParams(const ChainType chain)
{
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
