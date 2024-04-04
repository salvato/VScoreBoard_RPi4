/*
 *
Copyright (C) 2023  Gabriele Salvato

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once

#include <QMainWindow>
#include <QFile>
#include <QSettings>
#include <QProcess>
#include <QFileInfoList>
#include <QBluetoothLocalDevice>
#include <QBluetoothHostInfo>

#include "generalsetuparguments.h"


QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(SlideWidget)
QT_FORWARD_DECLARE_CLASS(BtServer)


class ScoreController : public QMainWindow
{
    Q_OBJECT

public:
    ScoreController(QFile *myLogFile, QWidget *parent = nullptr);
    ~ScoreController();

protected slots:
    void onButtonSpotLoopClicked();
    void onButtonSlideShowClicked();
    void onButtonSetupClicked();
    void onButtonShutdownClicked();
    void onSpotClosed(int exitCode, QProcess::ExitStatus exitStatus);
    void onStartNextSpot(int exitCode, QProcess::ExitStatus exitStatus);
    void closeEvent(QCloseEvent*) override;

private slots:
    void initBluetooth();
    void btClientConnected(const QString &name);
    void btClientDisconnected(const QString &name);
    void btClientDisconnected();
    void processBtMessage(const QString &sender, const QString &message);

signals:
    // void messageReceived(const QString &sender, const QString &message);
    void sendBtMessage(const QString &message);

protected:
    bool            eventFilter(QObject *obj, QEvent *event) override;
    bool            prepareLogFile();
    void            prepareServices();
    void            UpdateUI();
    QHBoxLayout*    CreateSpotButtons();
    void            connectButtonSignals();
    bool            startSlideShow();
    void            stopSlideShow();
    bool            startSpotLoop();
    void            stopSpotLoop();
    virtual void    SaveStatus();
    virtual void    GeneralSetup();
    void            doProcessCleanup();
    QString         XML_Parse(const QString& input_string, const QString& token);
    virtual void    processBtMessage(QString sMessage);
    virtual void    btSendAll();

protected:
    GeneralSetupArguments gsArgs;
    QFile*                pLogFile;
    QSettings*            pSettings;
    QPushButton*          pSpotButton{};
    QPushButton*          pSlideShowButton{};
    QPushButton*          pGeneralSetupButton{};
    QPushButton*          pShutdownButton{};
    QHBoxLayout*          pSpotButtonsLayout;
    enum status {
        showPanel,
        showSpots,
        showSlides,
        showCamera
    };
    status          myStatus;
    QProcess*       pVideoPlayer;
    QString         sProcess;
    QString         sProcessArguments;
    SlideWidget*    pMySlideWindow;
    QFileInfoList   spotList;
    struct spot {
        QString spotFilename;
        qint64  spotFileSize;
    };
    QList<spot>        availabeSpotList;
    int                iCurrentSpot;
    QString            sVideoPlayer;

    BtServer*          pBtServer;
    QString            sLocalName;
    QBluetoothLocalDevice btDevice;
};
