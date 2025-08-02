# nextBTC

[English Version](#english)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**如果你错过了2009年的比特币，那么现在，机会就在眼前！**

---

## ✨ 项目简介

`nextBTC` 是一个旨在致敬并复刻比特币核心体验的开源平行链项目。我们通过调整共识参数，为区块链初学者和资深爱好者创建了一个低门槛、零风险的实验环境。

在这里，您可以像比特币的早期先驱一样，亲手体验挖矿、转账和UTXO模型的独特魅力，而无需担心高昂的硬件成本和市场风险。这个项目完全出于教育和技术探索的目的。

## 🚀 主要特性

*   **核心钱包**：提供 Windows 和 Linux 下完整的全节点钱包功能。
*   **真实体验**：模拟比特币的挖矿、转账和区块生成过程。
*   **低门槛**：无需专业矿机，普通计算机即可参与挖矿。
*   **教育目的**：是学习和理解比特币底层技术的理想沙盒。
*   **完善的生态工具**：
    *   **项目官网**: [https://nextBTC.rf.gd](https://nextBTC.rf.gd)
    *   **区块链浏览器**: [http://115.190.97.253:3001/](http://115.190.97.253:3001/)
    *   **图文教程**: [查看零基础入门教程 (PDF)](https://nextBTC.rf.gd/pdf/nextBTC.pdf)

## ⚠️ 重要声明：教育与实验目的

> `nextBTC` 是一个为教育和技术探索目的而创建的开源实验项目。其 Coin 是项目内部的实验性产物，**不具备任何现实世界的经济价值**。
>
> 我们强烈建议并要求用户**不得**将其用于任何形式的金融交易、投资或销售活动。任何基于此项目的操作均为用户的自愿实验行为，项目创建者及社区贡献者**概不负责**。

## 🏁 如何开始

1.  **下载预编译钱包**
    访问本项目的 [Releases 页面](https://github.com/nextbtc2025/nextbtc/releases) 下载最新的预编译钱包程序。

2.  **阅读教程**
    我们强烈建议您阅读详细的图文教程，它将指导您完成所有必要设置。
    *   **[查看详细图文教程 (PDF)](https://nextbtc.rf.gd/pdf/nextBTC.pdf)**

3.  **配置与运行**
    *   在 nextBTC 的数据目录中 (通常是 `%APPDATA%/Bitcoin` 或 `~/.bitcoin/`)，创建一个名为 `bitcoin.conf` 的文件。
    *   将以下配置内容复制到文件中：
      ```conf
      server=1
      rpcuser=nextbtc
      rpcpassword=nextbtc
      rpcbind=127.0.0.1
      rpcallowip=127.0.0.1
      rpcport=8554
      ```
    *   运行 `bitcoind.exe` (或 `bitcoind`) 启动节点，或者运行 `bitcoin-qt.exe`打开钱包图形界面（windows）。

## 🛠️ 技术参数

| 参数 | 值 |
| :--- | :--- |
| **创世区块信息** | `"I COME,I SEE,I HODL."` |
| **算法** | SHA256d |
| **平均出块时间** | 2.5 分钟 |
| **初始区块奖励** | 3.125 nextBTC |
| **奖励减半周期**| 每 800,000 个区块 |
| **总供应量** | 5,000,000 nextBTC |
| **地址格式** | Bech32 (bc1...), Base58 |
| **RPC 端口** | 8554 |
| **P2P 端口** | 8555 |

## 🤝 如何贡献

这是一个由社区驱动的项目，我们欢迎任何形式的贡献！
*   报告 Bug
*   提出新功能建议
*   贡献代码 (Pull Requests)
*   
## ❤️ 支持项目

如果您觉得这个项目对您有帮助，或者您希望支持我继续开发和维护，可以考虑请我喝杯咖啡。您的支持是我最大的动力！

*   **BTC**: `bc1qy6ak4243fc0vd3d9hpsed8x3m9q62jpk3vxmlh`

非常感谢您的支持！
让我们一起回到原点，重新体验区块链的创世魅力！

**Happy HODLing!**

---
<br>
<br>

# English

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**If you missed Bitcoin in 2009, now is your chance to experience it anew!**

---

## ✨ About The Project

`nextBTC` is an open-source parallel chain project designed to pay homage to and replicate the core Bitcoin experience. By adjusting consensus parameters, we have created a low-barrier, zero-risk experimental environment for blockchain beginners and enthusiasts.

Here, you can experience the unique charm of mining, transfers, and the UTXO model firsthand, just like the early pioneers of Bitcoin, without worrying about high hardware costs and market risks. This project is created purely for educational and technological exploration purposes.

## 🚀 Key Features

*   **Core Wallet**: Provides full-node wallet functionality for Windows and Linux.
*   **Authentic Experience**: Simulates Bitcoin's mining, transaction, and block generation processes.
*   **Low Barrier to Entry**: No professional mining rigs required; a standard computer is sufficient to participate in mining.
*   **Educational Purpose**: An ideal sandbox for learning and understanding the underlying technology of Bitcoin.
*   **Comprehensive Ecosystem Tools**:
    *   **Official Website**: [https://nextBTC.rf.gd](https://nextBTC.rf.gd)
    *   **Block Explorer**: [http://115.190.97.253:3001/](http://115.190.97.253:3001/)
    *   **Tutorial**: [View Step-by-Step Tutorial for Beginners (PDF)](https://nextBTC.rf.gd/pdf/nextBTC.pdf)

## ⚠️ Important Disclaimer: Educational and Experimental Purpose

> `nextBTC` is an open-source experimental project created for educational and technological exploration. Its native coin is an experimental product within the project and **has no real-world economic value**.
>
> We strongly advise and require that users **do not** use it for any form of financial transactions, investments, or sales. All actions based on this project are voluntary experiments by the user. The project creators and community contributors **are not responsible** for any user actions.

## 🏁 Getting Started

1.  **Download Pre-compiled Wallet**
    Visit the project's [Releases Page](https://github.com/nextbtc2025/nextbtc/releases) to download the latest pre-compiled wallet application.

2.  **Read the Tutorial**
    We highly recommend reading the detailed tutorial, which will guide you through all the necessary setup steps.
    *   **[View Detailed Tutorial (PDF)](https://nextbtc.rf.gd/pdf/nextBTC.pdf)**

3.  **Configuration and Execution**
    *   In the nextBTC data directory (usually `%APPDATA%/Bitcoin` or `~/.bitcoin/`), create a file named `bitcoin.conf`.
    *   Copy the following configuration into the file:
      ```conf
      server=1
      rpcuser=nextbtc
      rpcpassword=nextbtc
      rpcbind=127.0.0.1
      rpcallowip=127.0.0.1
      rpcport=8554
      ```
    *   Run `bitcoind.exe` (or `bitcoind`) to start the node, then run `bitcoin-qt.exe`to open the wallet's graphical interface（windows）.

## 🛠️ Technical Specifications

| Parameter | Value |
| :--- | :--- |
| **Genesis Block Message**| `"I COME,I SEE,I HODL."` |
| **Algorithm** | SHA256d |
| **Average Block Time** | 2.5 Minutes |
| **Initial Block Reward** | 3.125 nextBTC |
| **Reward Halving Interval**| Every 800,000 blocks |
| **Total Supply** | 5,000,000 nextBTC |
| **Address Format** | Bech32 (bc1...), Base58 |
| **RPC Port** | 8554 |
| **P2P Port** | 8666 |

## 🤝 How to Contribute

This is a community-driven project, and we welcome contributions of all forms!
*   Reporting Bugs
*   Suggesting New Features
*   Contributing Code (Pull Requests)
*   
## ❤️ Support the Project

If you find this project helpful or wish to support my continued development and maintenance, please consider buying me a coffee. Your support is greatly appreciated and keeps me motivated!

*   **BTC**: `bc1qy6ak4243fc0vd3d9hpsed8x3m9q62jpk3vxmlh`

Thank you for your support!

Let's go back to the beginning and re-experience the genesis charm of blockchain together!

**Happy HODLing!**
