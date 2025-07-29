// Copyright (c) 2011-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin-build-config.h> // IWYU pragma: keep

#include <qt/utilitydialog.h>

#include <qt/forms/ui_helpmessagedialog.h>

#include <qt/guiutil.h>

#include <clientversion.h>
#include <common/args.h>
#include <init.h>
#include <util/strencodings.h>

#include <cstdio>

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QRegularExpression>
#include <QString>
#include <QTextCursor>
#include <QTextTable>
#include <QVBoxLayout>

#include <QMessageBox> 
/** "Help message" or "About" dialog box */
HelpMessageDialog::HelpMessageDialog(QWidget *parent, bool about) :
    QDialog(parent, GUIUtil::dialog_flags),
    ui(new Ui::HelpMessageDialog)
{

    ui->setupUi(this);

    QString version = QString{CLIENT_NAME} + " " + tr("version") + " " + QString::fromStdString(FormatFullVersion());

if (about)
{
    setWindowTitle(QString::fromUtf8("关于 nextBTC"));
    QString versionText = QString{CLIENT_NAME} + " " + tr("version") + " " + QString::fromStdString(FormatFullVersion());
    QString myCustomAboutText = QString::fromUtf8(
        "<h3>重要声明与免责条款</h3>"
        "<p><b>注意！</b>本软件不是原版 Bitcoin Core, 而是根据其源码修改的 <b>nextBTC</b> v1.0.2。"
        "这是一个为教育和技术探索目的而创建的开源实验项目。</p>"
        "<p><b>项目地址:</b> <a href=\"https://github.com/nextbtc2025/nextbtc\">https://github.com/nextbtc2025/nextbtc</a></p>"
        "<p>其代币是项目内部的实验性产物，不具备任何现实世界的经济价值。"
        "我们强烈建议并要求用户不得将其用于任何形式的金融交易、投资、销售或欺诈活动。</p>"
        "<hr>" // 分割线
        "<p><b>原始软件许可证信息如下:</b></p>"
    );
    QString licenseInfoHTML = QString::fromStdString(LicenseInfo());
    QRegularExpression uri(QStringLiteral("<(.*)>"), QRegularExpression::InvertedGreedinessOption);
    licenseInfoHTML.replace(uri, QStringLiteral("<a href=\"\\1\">\\1</a>"));
    licenseInfoHTML.replace("\n", "<br>");
    ui->aboutMessage->setText(versionText + "<br><br>" + myCustomAboutText + licenseInfoHTML);
    ui->aboutMessage->setTextFormat(Qt::RichText);
    ui->aboutMessage->setWordWrap(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->helpMessage->setVisible(false);
    text = versionText + "\n\n" + QString::fromStdString(LicenseInfo());
} else {
        setWindowTitle(tr("Command-line options"));
        QString header = "The bitcoin-qt application provides a graphical interface for interacting with " CLIENT_NAME ".\n\n"
                         "It combines the core functionalities of bitcoind with a user-friendly interface for wallet management, transaction history, and network statistics.\n\n"
                         "It is suitable for users who prefer a graphical over a command-line interface.\n\n"
                         "You can optionally specify a payment [URI], in e.g. the BIP21 URI format.\n\n"
                         "Usage: bitcoin-qt [options] [URI]\n\n";
        QTextCursor cursor(ui->helpMessage->document());
        cursor.insertText(version);
        cursor.insertBlock();
        cursor.insertText(header);
        cursor.insertBlock();

        std::string strUsage = gArgs.GetHelpMessage();
        QString coreOptions = QString::fromStdString(strUsage);
        text = version + "\n\n" + header + "\n" + coreOptions;

        QTextTableFormat tf;
        tf.setBorderStyle(QTextFrameFormat::BorderStyle_None);
        tf.setCellPadding(2);
        QVector<QTextLength> widths;
        widths << QTextLength(QTextLength::PercentageLength, 35);
        widths << QTextLength(QTextLength::PercentageLength, 65);
        tf.setColumnWidthConstraints(widths);

        QTextCharFormat bold;
        bold.setFontWeight(QFont::Bold);

        for (const QString &line : coreOptions.split("\n")) {
            if (line.startsWith("  -"))
            {
                cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::PreviousCell);
                cursor.movePosition(QTextCursor::NextRow);
                cursor.insertText(line.trimmed());
                cursor.movePosition(QTextCursor::NextCell);
            } else if (line.startsWith("   ")) {
                cursor.insertText(line.trimmed()+' ');
            } else if (line.size() > 0) {
                //Title of a group
                if (cursor.currentTable())
                    cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::Down);
                cursor.insertText(line.trimmed(), bold);
                cursor.insertTable(1, 2, tf);
            }
        }

        ui->helpMessage->moveCursor(QTextCursor::Start);
        ui->scrollArea->setVisible(false);
        ui->aboutLogo->setVisible(false);
    }

    GUIUtil::handleCloseWindowShortcut(this);
}

HelpMessageDialog::~HelpMessageDialog()
{
    delete ui;
}

void HelpMessageDialog::printToConsole()
{
    // On other operating systems, the expected action is to print the message to the console.
    tfm::format(std::cout, "%s", qPrintable(text));
}

void HelpMessageDialog::showOrPrint()
{
#if defined(WIN32)
    // On Windows, show a message box, as there is no stderr/stdout in windowed applications
    exec();
#else
    // On other operating systems, print help text to console
    printToConsole();
#endif
}

void HelpMessageDialog::on_okButton_accepted()
{
    close();
}


/** "Shutdown" window */
ShutdownWindow::ShutdownWindow(QWidget *parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel(
        tr("%1 is shutting down…").arg(CLIENT_NAME) + "<br /><br />" +
        tr("Do not shut down the computer until this window disappears.")));
    setLayout(layout);

    GUIUtil::handleCloseWindowShortcut(this);
}

QWidget* ShutdownWindow::showShutdownWindow(QMainWindow* window)
{
    assert(window != nullptr);

    // Show a simple window indicating shutdown status
    QWidget *shutdownWindow = new ShutdownWindow();
    shutdownWindow->setWindowTitle(window->windowTitle());

    // Center shutdown window at where main window was
    const QPoint global = window->mapToGlobal(window->rect().center());
    shutdownWindow->move(global.x() - shutdownWindow->width() / 2, global.y() - shutdownWindow->height() / 2);
    shutdownWindow->show();
    return shutdownWindow;
}

void ShutdownWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
