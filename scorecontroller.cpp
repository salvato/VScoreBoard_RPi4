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

#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QKeyEvent>
//#if QT_CONFIG(permissions)
//    #include <QPermissions>
//#endif

#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothserver.h>

#include "scorecontroller.h"
#include "slidewidget.h"
#include "utility.h"
#include "btserver.h"


ScoreController::ScoreController(QFile *myLogFile, QWidget *parent)
    : QMainWindow(parent)
    , pLogFile(myLogFile)
    , pSettings(new QSettings("Gabriele Salvato", "Volley Controller"))
    , pVideoPlayer(nullptr)
    , pMySlideWindow(new SlideWidget())
    #ifdef Q_OS_WINDOWS
        , sVideoPlayer(QString("ffplay.exe"))
    #else
        , sVideoPlayer(QString("/usr/bin/ffplay"))
    #endif
{
    qApp->installEventFilter(this);

    setWindowTitle("Score Controller - ©Gabriele Salvato (2024)");
    setWindowIcon(QIcon(":/Logo.ico"));

    iCurrentSpot = 0;
    pSpotButtonsLayout = CreateSpotButtons();
    connectButtonSignals();

    pMySlideWindow->showFullScreen();
    pMySlideWindow->hide();

    initBluetooth();

    myStatus = showPanel;
}


ScoreController::~ScoreController() {
    if(pMySlideWindow)
        pMySlideWindow->deleteLater();
    pMySlideWindow = nullptr;
    doProcessCleanup();
}


void
ScoreController::closeEvent(QCloseEvent*) {
    if(pMySlideWindow)
        pMySlideWindow->deleteLater();
    pMySlideWindow = nullptr;
    doProcessCleanup();
}


bool
ScoreController::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(((keyEvent->key() == Qt::Key_Left)   ||
            (keyEvent->key() == Qt::Key_Right)) &&
           (keyEvent->modifiers() == Qt::AltModifier))
        {
            activateWindow();
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    return QObject::eventFilter(obj, event);
}


QString
ScoreController::XML_Parse(const QString& input_string, const QString& token) {
    // simple XML parser
    //   XML_Parse("<score1>10</score1>","beam")   will return "10"
    // returns "" on error
    QString start_token, end_token, result = QString("NoData");
    start_token = "<" + token + ">";
    end_token = "</" + token + ">";

    int start_pos=-1, end_pos=-1, len=0;

    start_pos = input_string.indexOf(start_token);
    end_pos   = input_string.indexOf(end_token);

    if(start_pos < 0 || end_pos < 0) {
        result = QString("NoData");
    } else {
        start_pos += start_token.length();
        len = end_pos - start_pos;
        if(len>0)
            result = input_string.mid(start_pos, len);
        else
            result = "";
    }

    return result;
}


void
ScoreController::initBluetooth() {
//#if QT_CONFIG(permissions)
//    QBluetoothPermission permission{};
//    Qt::PermissionStatus btPermission = qApp->checkPermission(permission);
//    switch (btPermission) {
//        case Qt::PermissionStatus::Undetermined:
//            qApp->requestPermission(permission, this, &ScoreController::initBluetooth);
//            return;
//        case Qt::PermissionStatus::Denied:
//#ifdef LOG_MESG
//            logMessage(pLogFile,
//                       Q_FUNC_INFO,
//                       tr("Permissions are needed to use Bluetooth. "
//                          "Please grant the permissions to this "
//                          "application in the system settings."));
//#endif
//            QMessageBox::warning(this, tr("Missing permissions"),
//                                 tr("Permissions are needed to use Bluetooth. "
//                                    "Please grant the permissions to this "
//                                    "application in the system settings."));
//            qApp->quit();
//            return;
//        case Qt::PermissionStatus::Granted:
//            // qDebug() << "Permissions Granted!:";
//            break; // proceed to initialization
//    }
//#endif // QT_CONFIG(permissions)

    if (!btDevice.isValid()) {
        QMessageBox::critical(this, tr("No Bluetooth adapter has been found!"),
                             tr("Program can continue but Bluetooth has been"
                                "disabled and Remote control will not works"));
        return;
    }

    btDevice.powerOn();
    btDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    sLocalName = btDevice.name();

    // Create The Bluetooth Panel Server
    pBtServer = new BtServer(this);

    connect(pBtServer, SIGNAL(clientConnected(QString)),
            this, SLOT(btClientConnected(QString)));
    connect(pBtServer, SIGNAL(clientDisconnected(QString)),
            this,  SLOT(btClientDisconnected(QString)));
    connect(pBtServer, SIGNAL(messageReceived(QString,QString)),
            this,  SLOT(processBtMessage(QString,QString)));
    connect(this, SIGNAL(sendBtMessage(QString)),
            pBtServer, SLOT(sendMessage(QString)));

    if(!pBtServer->startServer()) {
        QMessageBox::critical(this, tr("Bluetooth Server Could Not Start !"),
                             tr("Program can continue but Bluetooth has been"
                                "disabled and Remote control will not works"));
        pBtServer->disconnect();
        pBtServer->deleteLater();
        pBtServer = nullptr;
    }
}


void
ScoreController::doProcessCleanup() {
#ifdef LOG_VERBOSE
    logMessage(pLogFile,
               Q_FUNC_INFO,
               QString("Cleaning all processes"));
#endif
    if(pMySlideWindow) {
        pMySlideWindow->close();
    }
    if(pVideoPlayer) {
        pVideoPlayer->disconnect();
        pVideoPlayer->close();
#ifdef LOG_MESG
        logMessage(pLogFile,
                   Q_FUNC_INFO,
                   QString("Closing Video Player..."));
#endif
        pVideoPlayer->waitForFinished(3000);
        pVideoPlayer->deleteLater();
        pVideoPlayer = Q_NULLPTR;
    }
}


QHBoxLayout*
ScoreController::CreateSpotButtons() {
    auto* spotButtonLayout = new QHBoxLayout();

    QPixmap pixmap(":/buttonIcons/PlaySpots.png");
    QIcon ButtonIcon(pixmap);
    pSpotButton = new QPushButton(ButtonIcon, "");
    pSpotButton->setIconSize(pixmap.rect().size());
    pSpotButton->setFlat(true);
    pSpotButton->setToolTip("Start/Stop Spot Loop");

    pixmap.load(":/buttonIcons/PlaySlides.png");
    ButtonIcon.addPixmap(pixmap);
    pSlideShowButton = new QPushButton(ButtonIcon, "");
    pSlideShowButton->setIconSize(pixmap.rect().size());
    pSlideShowButton->setFlat(true);
    pSlideShowButton->setToolTip("Start/Stop Slide Show");

    pixmap.load(":/buttonIcons/PanelSetup.png");
    ButtonIcon.addPixmap(pixmap);
    pGeneralSetupButton = new QPushButton(ButtonIcon, "");
    pGeneralSetupButton->setIconSize(pixmap.rect().size());
    pGeneralSetupButton->setFlat(true);
    pGeneralSetupButton->setToolTip("General Setup");

    pixmap.load(":/buttonIcons/video-display.png");
    ButtonIcon.addPixmap(pixmap);
    pShutdownButton = new QPushButton(ButtonIcon, "");
    pShutdownButton->setIconSize(pixmap.rect().size());
    pShutdownButton->setFlat(true);
    pShutdownButton->setToolTip("Shutdown System");

    spotButtonLayout->addWidget(pSpotButton);
    spotButtonLayout->addStretch();
    spotButtonLayout->addWidget(pSlideShowButton);
    spotButtonLayout->addStretch();
    spotButtonLayout->addWidget(pGeneralSetupButton);
    spotButtonLayout->addStretch();
    spotButtonLayout->addWidget(pShutdownButton);

    return spotButtonLayout;
}


void
ScoreController::connectButtonSignals() {
    connect(pSpotButton, SIGNAL(clicked(bool)),
            this, SLOT(onButtonSpotLoopClicked()));

    connect(pSlideShowButton, SIGNAL(clicked(bool)),
            this, SLOT(onButtonSlideShowClicked()));
    connect(pGeneralSetupButton, SIGNAL(clicked(bool)),
            this, SLOT(onButtonSetupClicked()));

    connect(pShutdownButton, SIGNAL(clicked(bool)),
            this, SLOT(onButtonShutdownClicked()));
}


void
ScoreController::onButtonSetupClicked() {
    GeneralSetup();
}


// Dummy... see Volley Panel
void
ScoreController::GeneralSetup() {
}


// Dummy... see Volley Panel
void
ScoreController::SaveStatus() {
}


bool
ScoreController::startSpotLoop() {
    QDir spotDir(gsArgs.sSpotDir);
    spotList = QFileInfoList();
    if(spotDir.exists()) {
        QStringList nameFilter(QStringList() << "*.mp4" << "*.MP4" << "*.mov" << "*.MOV");
        spotDir.setNameFilters(nameFilter);
        spotDir.setFilter(QDir::Files);
        spotList = spotDir.entryInfoList();
    }
#ifdef LOG_VERBOSE
    logMessage(pLogFile,
               Q_FUNC_INFO,
               QString("Found %1 spots").arg(spotList.count()));
#endif
    if(!spotList.isEmpty()) {
        iCurrentSpot = iCurrentSpot % spotList.count();
        if(!pVideoPlayer) {
            pVideoPlayer = new QProcess(this);
            connect(pVideoPlayer, SIGNAL(finished(int,QProcess::ExitStatus)),
                    this, SLOT(onStartNextSpot(int,QProcess::ExitStatus)));

            QStringList sArguments;
            sArguments = QStringList{"-noborder",
                                     "-sn",
                                     "-autoexit",
                                     "-fs"
                                    };
            QList<QScreen*> screens = QApplication::screens();
            if(screens.count() > 1) {
                QRect screenres = screens.at(1)->geometry();
                sArguments.append(QString("-left"));
                sArguments.append(QString("%1").arg(screenres.x()));
                sArguments.append(QString("-top"));
                sArguments.append(QString("%1").arg(screenres.y()));
                sArguments.append(QString("-x"));
                sArguments.append(QString("%1").arg(screenres.width()));
                sArguments.append(QString("-y"));
                sArguments.append(QString("%1").arg(screenres.height()));
            }
            sArguments.append(spotList.at(iCurrentSpot).absoluteFilePath());
            pVideoPlayer->start(sVideoPlayer, sArguments);

#ifdef LOG_VERBOSE
            logMessage(pLogFile,
                       Q_FUNC_INFO,
                       QString("Now playing: %1")
                       .arg(spotList.at(iCurrentSpot).absoluteFilePath()));
#endif
            iCurrentSpot = (iCurrentSpot+1) % spotList.count();// Prepare Next Spot
            if(!pVideoPlayer->waitForStarted(3000)) {
                pVideoPlayer->close();
                logMessage(pLogFile,
                           Q_FUNC_INFO,
                           QString("Impossibile mandare lo spot."));
                QMessageBox::critical(this, tr("Impossibile mandare lo spot !"),
                                      tr("Il programma %1 é stato installato ?")
                                          .arg(sVideoPlayer));
                pVideoPlayer->disconnect();
                delete pVideoPlayer;
                pVideoPlayer = nullptr;
                return false;
            }
        } // if(!videoPlayer)
    } // if(!spotList.isEmpty())
    else
        return false;
    return true;
}


void
ScoreController::stopSpotLoop() {
    if(pVideoPlayer) {
        pVideoPlayer->disconnect();
        connect(pVideoPlayer, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(onSpotClosed(int,QProcess::ExitStatus)));
        pVideoPlayer->terminate();
    }
}


bool
ScoreController::startSlideShow() {
    if(pVideoPlayer)
        return false;// No Slide Show if movies are playing or camera is active
    if(!pMySlideWindow) {
        logMessage(pLogFile,
                   Q_FUNC_INFO,
                   QString("Invalid Slide Window"));
        return false;
    }
    if(!pMySlideWindow->setSlideDir(gsArgs.sSlideDir)) {
        return false;
    }
    pMySlideWindow->showFullScreen();
    if(!pMySlideWindow->startSlideShow()) {
        pMySlideWindow->hide();
        return false;
    }
    return true;
}


void
ScoreController::stopSlideShow() {
    if(pMySlideWindow) {
        pMySlideWindow->stopSlideShow();
        pMySlideWindow->hide();
    }
}


void
ScoreController::UpdateUI() {
    pSpotButton->setEnabled(true);
    pSlideShowButton->setEnabled(true);
    pShutdownButton->setEnabled(true);
}


void
ScoreController::onButtonSpotLoopClicked() {
    QPixmap pixmap;
    QIcon ButtonIcon;
    if(myStatus == showPanel) {
        if(startSpotLoop()) {
            pixmap.load(":/buttonIcons/sign_stop.png");
            ButtonIcon.addPixmap(pixmap);
            pSpotButton->setIcon(ButtonIcon);
            pSpotButton->setIconSize(pixmap.rect().size());
            pSlideShowButton->setDisabled(true);
            pGeneralSetupButton->setDisabled(true);
            myStatus = showSpots;
            QString sMessage = QString("<startSpotLoop>1</startSpotLoop>");
            emit sendBtMessage(sMessage);
        }
    }
    else {
        pixmap.load(":/buttonIcons/PlaySpots.png");
        ButtonIcon.addPixmap(pixmap);
        pSpotButton->setIcon(ButtonIcon);
        pSpotButton->setIconSize(pixmap.rect().size());
        pSlideShowButton->setEnabled(true);
        pGeneralSetupButton->setEnabled(true);
        stopSpotLoop();
        myStatus = showPanel;
        QString sMessage = QString("<endSpotLoop>1</endSpotLoop>");
        emit sendBtMessage(sMessage);
    }
}


void
ScoreController::onButtonSlideShowClicked() {
    QPixmap pixmap;
    QIcon ButtonIcon;
    setCursor(Qt::WaitCursor);
    if(myStatus == showPanel) {
        if(startSlideShow()) {
            pSpotButton->setDisabled(true);
            pGeneralSetupButton->setDisabled(true);
            pixmap.load(":/buttonIcons/sign_stop.png");
            ButtonIcon.addPixmap(pixmap);
            pSlideShowButton->setIcon(ButtonIcon);
            pSlideShowButton->setIconSize(pixmap.rect().size());
            myStatus = showSlides;
            QString sMessage = QString("<startSlideShow>1</startSlideShow>");
            emit sendBtMessage(sMessage);
        }
    }
    else {
        pSpotButton->setEnabled(true);
        pGeneralSetupButton->setEnabled(true);
        pixmap.load(":/buttonIcons/PlaySlides.png");
        ButtonIcon.addPixmap(pixmap);
        pSlideShowButton->setIcon(ButtonIcon);
        pSlideShowButton->setIconSize(pixmap.rect().size());
        stopSlideShow();
        myStatus = showPanel;
        QString sMessage = QString("<endSlideShow>1</endSlideShow>");
        emit sendBtMessage(sMessage);
    }
    unsetCursor();
}


void
ScoreController::onButtonShutdownClicked() {
    QMessageBox msgBox;
    msgBox.setText("Sei Sicuro di Volere Spegnere");
    msgBox.setInformativeText("i Tabelloni ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int answer = msgBox.exec();
    if(answer != QMessageBox::Yes) return;
    close();
}


void
ScoreController::onSpotClosed(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    if(pVideoPlayer) {
        pVideoPlayer->disconnect();
        pVideoPlayer->close();// Closes all communication with the process and kills it.
        delete pVideoPlayer;
        pVideoPlayer = nullptr;
    } // if(pVideoPlayer)
}


void
ScoreController::onStartNextSpot(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    // Update spot list just in case we are updating the spot list...
    QDir spotDir(gsArgs.sSpotDir);
    spotList = QFileInfoList();
    QStringList nameFilter(QStringList() << "*.mp4" << "*.MP4");
    spotDir.setNameFilters(nameFilter);
    spotDir.setFilter(QDir::Files);
    spotList = spotDir.entryInfoList();
    if(spotList.count() == 0) {
#ifdef LOG_VERBOSE
        logMessage(pLogFile,
                   Q_FUNC_INFO,
                   QString("No spots available !"));
#endif
        if(pVideoPlayer) {
            pVideoPlayer->disconnect();
            delete pVideoPlayer;
            pVideoPlayer = nullptr;
        }
        return;
    }

    iCurrentSpot = iCurrentSpot % spotList.count();
    if(!pVideoPlayer) {
        pVideoPlayer = new QProcess(this);
        connect(pVideoPlayer, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(onStartNextSpot(int,QProcess::ExitStatus)));
    }

    QStringList sArguments;
    sArguments = QStringList{"-noborder",
                             "-sn",
                             "-autoexit",
                             "-fs"
                            };
    QList<QScreen*> screens = QApplication::screens();
    if(screens.count() > 1) {
        QRect screenres = screens.at(1)->geometry();
        sArguments.append(QString("-left"));
        sArguments.append(QString("%1").arg(screenres.x()));
        sArguments.append(QString("-top"));
        sArguments.append(QString("%1").arg(screenres.y()));
        sArguments.append(QString("-x"));
        sArguments.append(QString("%1").arg(screenres.width()));
        sArguments.append(QString("-y"));
        sArguments.append(QString("%1").arg(screenres.height()));
    }
    sArguments.append(spotList.at(iCurrentSpot).absoluteFilePath());

    pVideoPlayer->start(sVideoPlayer, sArguments);
#ifdef LOG_VERBOSE
    logMessage(pLogFile,
               Q_FUNC_INFO,
               QString("Now playing: %1")
               .arg(spotList.at(iCurrentSpot).absoluteFilePath()));
#endif
    iCurrentSpot = (iCurrentSpot+1) % spotList.count();// Prepare Next Spot
    if(!pVideoPlayer->waitForStarted(3000)) {
        pVideoPlayer->close();
        logMessage(pLogFile,
                   Q_FUNC_INFO,
                   QString("Impossibile mandare lo spot"));
        pVideoPlayer->disconnect();
        delete pVideoPlayer;
        pVideoPlayer = Q_NULLPTR;
        return;
    }
}


void
ScoreController::btClientConnected(const QString &name) {
    Q_UNUSED(name)
    btSendAll();
}


void
ScoreController::btSendAll(){
}


void
ScoreController::btClientDisconnected(const QString &name) {
    Q_UNUSED(name)
}


void
ScoreController::processBtMessage(const QString &sender, const QString &message) {
    Q_UNUSED(sender)
    processBtMessage(message);
}


void
ScoreController::processBtMessage(QString sMessage) {
    Q_UNUSED(sMessage)
}


void
ScoreController::btClientDisconnected() {
}
