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
#include <QtGlobal>
#include <QtWidgets>
#include <QProcess>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include "volleypanel.h"
#include "timeoutwindow.h"
#include "utility.h"


VolleyPanel::VolleyPanel(QFile *myLogFile, QWidget *parent)
    : ScorePanel(myLogFile, parent)
    , iServizio(0)
    , maxTeamNameLen(15)
    , pTimeoutWindow(Q_NULLPTR)
{
    pPixmapLogo[0] = new QPixmap(QString(":/Logo_UniMe.png"));
    pPixmapLogo[1] = new QPixmap(QString(":/Logo_SSD_UniMe.png"));

    sFontName = QString("Liberation Sans Bold");
    fontWeight = QFont::Black;

    QSize panelSize = QGuiApplication::primaryScreen()->geometry().size();
    iTeamFontSize    = std::min(panelSize.height()/8,
                                int(panelSize.width()/(2.2*maxTeamNameLen)));
    iScoreFontSize   = std::min(panelSize.height()/5,
                                int(panelSize.width()/10));
    iLabelsFontSize  = panelSize.height()/8; // 2 Righe
    iTimeoutFontSize = panelSize.height()/8; // 2 Righe
    iSetFontSize     = panelSize.height()/8; // 2 Righe

    // QWidget propagates explicit palette roles from parent to child.
    // If you assign a brush or color to a specific role on a palette and
    // assign that palette to a widget, that role will propagate to all
    // the widget's children, overriding any system defaults for that role.
    panelPalette = QWidget::palette();
    panelGradient = QLinearGradient(0.0, 0.0, 0.0, height());
    panelGradient.setColorAt(0, QColor(0, 0, START_GRADIENT));
    panelGradient.setColorAt(1, QColor(0, 0, END_GRADIENT));
    panelBrush = QBrush(panelGradient);
    panelPalette.setBrush(QPalette::Active,   QPalette::Window, panelBrush);
    panelPalette.setBrush(QPalette::Inactive, QPalette::Window, panelBrush);
    panelPalette.setColor(QPalette::WindowText,    Qt::yellow);
    panelPalette.setColor(QPalette::Base,          Qt::black);
    panelPalette.setColor(QPalette::AlternateBase, Qt::blue);
    panelPalette.setColor(QPalette::Text,          Qt::yellow);
    panelPalette.setColor(QPalette::BrightText,    Qt::white);
    setPalette(panelPalette);

    pTimeoutWindow = new TimeoutWindow(Q_NULLPTR);
    connect(pTimeoutWindow, SIGNAL(doneTimeout()),
            this, SLOT(onTimeoutDone()));

    createPanelElements();
    buildLayout();
}


VolleyPanel::~VolleyPanel() {
    if(pTimeoutWindow) delete pTimeoutWindow;
    pTimeoutWindow = nullptr;
}


void
VolleyPanel::setTeam(int iTeam, QString sTeamName) {
    pTeam[iTeam]->setText(sTeamName.left(maxTeamNameLen));
}


void
VolleyPanel::setScore(int iTeam, int iScore) {
    pScore[iTeam]->setText(QString("%1").arg(iScore));
}


void
VolleyPanel::setSets(int iTeam, int iSets) {
    pSet[iTeam]->setText(QString("%1").arg(iSets));
}


void
VolleyPanel::setServizio(int iServizio) {
    pServizio[0]->setText(" ");
    pServizio[1]->setText(" ");
    if(iServizio == 0) {
        pServizio[0]->setPixmap(*pPixmapService);
    } else if(iServizio == 1) {
        pServizio[1]->setPixmap(*pPixmapService);
    }
}


void
VolleyPanel::setTimeout(int iTeam, int iTimeout) {
    pTimeout[iTeam]->setText(QString("%1").arg(iTimeout));
}


void
VolleyPanel::startTimeout(int iTimeoutDuration) {
    pTimeoutWindow->startTimeout(iTimeoutDuration*1000);
    pTimeoutWindow->showFullScreen();
}


void
VolleyPanel::stopTimeout() {
    pTimeoutWindow->stopTimeout();
    pTimeoutWindow->hide();
}


void
VolleyPanel::setMirrored(bool isPanelMirrored) {
    isMirrored = isPanelMirrored;
    buildLayout();
}


bool
VolleyPanel::getMirrored() {
    return isMirrored;
}


void
VolleyPanel::closeEvent(QCloseEvent *event) {
    if(pTimeoutWindow) delete pTimeoutWindow;
    pTimeoutWindow = nullptr;
    ScorePanel::closeEvent(event);
    event->accept();
}


void
VolleyPanel::createPanelElements() {
    // Timeout
    pTimeoutLabel = new QLabel("Timeout");
    pTimeoutLabel->setFont(QFont(sFontName, iLabelsFontSize/2, fontWeight));
    pTimeoutLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    for(int i=0; i<2; i++) {
        pTimeout[i] = new QLabel("8");
        pTimeout[i]->setFrameStyle(QFrame::NoFrame);
        pTimeout[i]->setFont(QFont(sFontName, iTimeoutFontSize, fontWeight));
    }

    // Set
    pSetLabel = new QLabel(tr("Set"));
    pSetLabel->setFont(QFont(sFontName, iLabelsFontSize/2, fontWeight));
    pSetLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    for(int i=0; i<2; i++) {
        pSet[i] = new QLabel("8");
        pSet[i]->setFrameStyle(QFrame::NoFrame);
        pSet[i]->setFont(QFont(sFontName, iSetFontSize, fontWeight));
    }

    // Score
    pScoreLabel = new QLabel(tr(" "));
    pScoreLabel->setFont(QFont(sFontName, iLabelsFontSize, fontWeight));
    pScoreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    for(int i=0; i<2; i++){
        pScore[i] = new QLabel("88");
        pScore[i]->setAlignment(Qt::AlignHCenter);
        pScore[i]->setFont(QFont(sFontName, iScoreFontSize, fontWeight));
        pScore[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    // Servizio
    for(int i=0; i<2; i++){
        pServizio[i] = new QLabel(" ");
        pServizio[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    // Loghi
    for(int i=0; i<2; i++){
        logoLabel[i] = new QLabel(" ");
        logoLabel[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    // Teams
    QPalette pal = panelPalette;
    pal.setColor(QPalette::WindowText, Qt::white);
    for(int i=0; i<2; i++) {
        pTeam[i] = new QLabel();
        pTeam[i]->setPalette(pal);
        pTeam[i]->setFont(QFont(sFontName, iTeamFontSize, fontWeight));
        pTeam[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    pTeam[0]->setText(tr("Locali"));
    pTeam[1]->setText(tr("Ospiti"));

    // Copyright
    pCopyRight = new QLabel("© Gabriele Salvato (2024)");
    pCopyRight->setPalette(pal);
    pCopyRight->setFont(QFont(sFontName, iTeamFontSize/2, fontWeight));
    pCopyRight->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}


QGridLayout*
VolleyPanel::createPanel() {
    QGridLayout *layout = new QGridLayout();

    int ileft  = 0;
    int iright = 1;
    if(isMirrored) {
        ileft  = 1;
        iright = 0;
    }

    pPixmapService = new QPixmap(":/ball2.png");
    *pPixmapService = pPixmapService->scaled(2*iLabelsFontSize/3, 2*iLabelsFontSize/3);

    layout->addWidget(pTeam[ileft],      0, 0, 2, 6, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pTeam[iright],     0, 6, 2, 6, Qt::AlignHCenter|Qt::AlignVCenter);

    layout->addWidget(pScore[ileft],     2, 1, 4, 3, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pServizio[ileft],  2, 4, 4, 1, Qt::AlignLeft   |Qt::AlignTop);
    layout->addWidget(pScoreLabel,       2, 5, 4, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pServizio[iright], 2, 7, 4, 1, Qt::AlignRight  |Qt::AlignTop);
    layout->addWidget(pScore[iright],    2, 8, 4, 3, Qt::AlignHCenter|Qt::AlignVCenter);

    layout->addWidget(pSet[ileft],       6, 3, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pSetLabel,         6, 4, 2, 4, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pSet[iright],      6, 8, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);

    layout->addWidget(logoLabel[ileft],  6, 0, 4, 3, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pTimeout[ileft],   8, 3, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pTimeoutLabel,     8, 4, 2, 4, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(pTimeout[iright],  8, 8, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(logoLabel[iright], 6, 9, 4, 3, Qt::AlignHCenter|Qt::AlignVCenter);

    layout->addWidget(pCopyRight,       10, 6, 1, 6, Qt::AlignRight  |Qt::AlignVCenter);

    return layout;
}


void
VolleyPanel::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
#ifdef LOG_VERBOSE
        logMessage(pLogFile,
                   Q_FUNC_INFO,
                   QString("%1  %2")
                   .arg(pSetLabel->text(), pScoreLabel->text()));
#endif
        pSetLabel->setText(tr("Set"));
        pScoreLabel->setText(tr("Punti"));
    } else
        QWidget::changeEvent(event);
}


void
VolleyPanel::setLogo(int iTeam, QString sFileLogo) {
    if(QFile::exists(sFileLogo)) {
        if(pPixmapLogo[iTeam]) delete pPixmapLogo[iTeam];
        pPixmapLogo[iTeam] =  new QPixmap(sFileLogo);
        logoLabel[iTeam]->setPixmap(pPixmapLogo[iTeam]->scaled(192, 192, Qt::KeepAspectRatio));
    }
}


void
VolleyPanel::onTimeoutDone() {
    showFullScreen();
    pTimeoutWindow->hide();
}

