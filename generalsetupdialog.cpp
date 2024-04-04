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
#include "generalsetupdialog.h"
#include "utility.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDir>
#include <QApplication>
#include <QLabel>
#include <QFrame>
#include <QFileDialog>
#include <QStandardPaths>


GeneralSetupDialog::GeneralSetupDialog(GeneralSetupArguments* pArguments)
    : QDialog()
    , pTempArguments(pArguments)
{
    setWindowTitle("General Setup");
    setWindowIcon(QIcon(":/buttonIcons/PanelSetup.png"));

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

    buttonOk.setText("OK");
    buttonCancel.setText("Cancel");
    connect(&buttonOk,     SIGNAL(clicked()), this, SLOT(onOk()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

    slidesDirEdit.setReadOnly(true);
    spotsDirEdit.setReadOnly(true);
    slidesDirEdit.setStyleSheet("background:red;color:white;");
    spotsDirEdit.setStyleSheet("background:red;color:white;");
    setSlideDir();
    setSpotDir();

    for(int i=0; i<2; i++) {
        teamLogoPathEdit[i].setReadOnly(true);
        teamLogoPathEdit[i].setStyleSheet("background:red;color:white;");
    }
    setTeam0Path();
    setTeam1Path();


    QLabel* pSlidesPathLabel = new QLabel(tr("Slides folder:"));
    QLabel* pSpotsPathLabel  = new QLabel(tr("Movies folder:"));
    QLabel* pLabelDirection = new QLabel(("Orientamento"));
    directionCombo.addItem(tr("Normale"));
    directionCombo.addItem(tr("Riflesso"));
    if(pArguments->isPanelMirrored)
        directionCombo.setCurrentText(tr("Riflesso"));
    else
        directionCombo.setCurrentText(tr("Normale"));

    buttonSelectSlidesDir.setText("Change");
    buttonSelectSpotsDir.setText("Change");
    buttonSelectTeam0Logo.setText("Change");
    buttonSelectTeam1Logo.setText("Change");

    // Signals from Dialog Elements
    connect(&directionCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangePanelOrientation(int)));
    connect(&buttonSelectSlidesDir, SIGNAL(clicked()),
            this, SLOT(onSelectSlideDir()));
    connect(&buttonSelectSpotsDir, SIGNAL(clicked()),
            this, SLOT(onSelectSpotDir()));
    connect(&buttonSelectTeam0Logo, SIGNAL(clicked()),
            this, SLOT(onSelectLogo0()));
    connect(&buttonSelectTeam1Logo, SIGNAL(clicked()),
            this, SLOT(onSelectLogo1()));

    QLabel* pNumTimeoutLabel      = new QLabel(tr("Max Timeouts:"));
    QLabel* pMaxSetLabel          = new QLabel(tr("Max Sets:"));
    QLabel* pTimeoutDurationLabel = new QLabel(tr("Timeout sec:"));
    QLabel* pTeamLabel[2];
    for(int i=0; i<2; i++) {
        pTeamLabel[i] = new QLabel("Logo "+pTempArguments->sTeam[i]);
    }

    numTimeoutEdit.setMaxLength(1);
    maxSetEdit.setMaxLength(1);
    timeoutDurationEdit.setMaxLength(2);

    numTimeoutEdit.setStyleSheet("background:white;color:black");
    maxSetEdit.setStyleSheet("background:white;color:black");
    timeoutDurationEdit.setStyleSheet("background:white;color:black");
    directionCombo.setStyleSheet("background:white;color:black");

    numTimeoutEdit.setText(QString("%1").arg(pArguments->maxTimeout));
    maxSetEdit.setText(QString("%1").arg(pArguments->maxSet));
    timeoutDurationEdit.setText(QString("%1").arg(pArguments->iTimeoutDuration));

    QGridLayout* pMainLayout = new QGridLayout;

    pMainLayout->addWidget(pSlidesPathLabel,        0, 0, 1, 1);
    pMainLayout->addWidget(&slidesDirEdit,          0, 1, 1, 6);
    pMainLayout->addWidget(&buttonSelectSlidesDir,  0, 7, 1, 1);

    pMainLayout->addWidget(pSpotsPathLabel,         1, 0, 1, 1);
    pMainLayout->addWidget(&spotsDirEdit,           1, 1, 1, 6);
    pMainLayout->addWidget(&buttonSelectSpotsDir,   1, 7, 1, 1);

    pMainLayout->addWidget(pNumTimeoutLabel,        2, 0, 1, 3);
    pMainLayout->addWidget(&numTimeoutEdit,         2, 3, 1, 1);

    pMainLayout->addWidget(pMaxSetLabel,            3, 0, 1, 3);
    pMainLayout->addWidget(&maxSetEdit,             3, 3, 1, 1);

    pMainLayout->addWidget(pTimeoutDurationLabel,   4, 0, 1, 3);
    pMainLayout->addWidget(&timeoutDurationEdit,    4, 3, 1, 1);

    pMainLayout->addWidget(pLabelDirection,         5, 0, 1, 2);
    pMainLayout->addWidget(&directionCombo,         5, 2, 1, 6);

    pMainLayout->addWidget(pTeamLabel[0],           6, 0, 1, 1);
    pMainLayout->addWidget(&teamLogoPathEdit[0],    6, 1, 1, 6);
    pMainLayout->addWidget(&buttonSelectTeam0Logo,  6, 7, 1, 1);

    pMainLayout->addWidget(pTeamLabel[1],           7, 0, 1, 1);
    pMainLayout->addWidget(&teamLogoPathEdit[1],    7, 1, 1, 6);
    pMainLayout->addWidget(&buttonSelectTeam1Logo,  7, 7, 1, 1);

    pMainLayout->addWidget(&buttonCancel,           8, 6, 1, 1);
    pMainLayout->addWidget(&buttonOk,               8, 7, 1, 1);

    setLayout(pMainLayout);
}


void
GeneralSetupDialog::setSlideDir() {
    slidesDirEdit.setText(pTempArguments->sSlideDir);
    QDir slideDir(pTempArguments->sSlideDir);
    if(slideDir.exists()) {
        QFileInfoList slideList = QFileInfoList();
        QStringList nameFilter = QStringList() << "*.jpg" << "*.jpeg" << "*.png";
        slideDir.setNameFilters(nameFilter);
        slideDir.setFilter(QDir::Files);
        slideList = slideDir.entryInfoList();
        if(!slideList.isEmpty()) {
            slidesDirEdit.setStyleSheet("background:white;color:black;");
            buttonOk.setEnabled(true);
        }
        else {
            slidesDirEdit.setStyleSheet("background:red;color:white;");
            buttonOk.setDisabled(true);
        }
    }
    else {
        slidesDirEdit.setStyleSheet("background:red;color:white;");
        buttonOk.setDisabled(true);
    }
}


void
GeneralSetupDialog::setSpotDir() {
    spotsDirEdit.setText(pTempArguments->sSpotDir);
    QDir spotDir(pTempArguments->sSpotDir);
    if(spotDir.exists()) {
        QStringList nameFilter(QStringList() << "*.mp4" << "*.MP4");
        spotDir.setNameFilters(nameFilter);
        spotDir.setFilter(QDir::Files);
        QFileInfoList spotList = spotDir.entryInfoList();
        if(!spotList.isEmpty()) {
            spotsDirEdit.setStyleSheet("background:white;color:black;");
            buttonOk.setEnabled(true);
        }
        else {
            spotsDirEdit.setStyleSheet("background:red;color:white;");
            buttonOk.setDisabled(true);
        }
    }
    else {
        spotsDirEdit.setStyleSheet("background:red;color:white;");
        buttonOk.setDisabled(true);
    }
}


void
GeneralSetupDialog::onSelectSlideDir() {
    QString sSlideDir = slidesDirEdit.text();
    QDir slideDir = QDir(sSlideDir);
    if(slideDir.exists()) {
        sSlideDir = QFileDialog::getExistingDirectory(this,
                                                      "Slide Directory",
                                                      sSlideDir);
    }
    else {
        sSlideDir = QFileDialog::getExistingDirectory(this,
                                                      "Slide Directory",
                                                      QStandardPaths::displayName(QStandardPaths::PicturesLocation));
    }
    if(sSlideDir == QString()) return; // "Cancel" has been pressed...
    if(!sSlideDir.endsWith(QString("/"))) sSlideDir+= QString("/");
    pTempArguments->sSlideDir = sSlideDir;
    setSlideDir();
}


void
GeneralSetupDialog::onSelectSpotDir() {
    QString sSpotDir = spotsDirEdit.text();
    QDir spotDir = QDir(sSpotDir);
    if(spotDir.exists()) {
        sSpotDir = QFileDialog::getExistingDirectory(this,
                                                     "Spot Directory",
                                                     sSpotDir);
    }
    else {
        sSpotDir = QFileDialog::getExistingDirectory(this,
                                                     "Spot Directory",
                                                     QStandardPaths::displayName(QStandardPaths::MoviesLocation));
    }
    if(sSpotDir == QString()) return; // "Cancel" has been pressed...
    if(!sSpotDir.endsWith(QString("/"))) sSpotDir+= QString("/");
    pTempArguments->sSpotDir = sSpotDir;
    setSpotDir();
}


void
GeneralSetupDialog::onSelectLogo0() {
    QString sFileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Logo File"),
                                         teamLogoPathEdit[0].text(),
                                         tr("Image Files (*.png *.jpg *.bmp)"));
    if(sFileName == QString()) return; // "Cancel" has been pressed...
    pTempArguments->sTeamLogoFilePath[0] = sFileName;
    setTeam0Path();
}


void
GeneralSetupDialog::onSelectLogo1() {
    QString sFileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Logo File"),
                                         teamLogoPathEdit[1].text(),
                                         tr("Image Files (*.png *.jpg *.bmp)"));
    if(sFileName == QString()) return; // "Cancel" has been pressed...
    pTempArguments->sTeamLogoFilePath[1] = sFileName;
    setTeam1Path();
}


void
GeneralSetupDialog::onChangePanelOrientation(int iOrientation) {
    pTempArguments->isPanelMirrored = false;
    PanelOrientation newOrientation = PanelOrientation::Normal;
    if(iOrientation == 1) {
        newOrientation = PanelOrientation::Reflected;
        pTempArguments->isPanelMirrored = true;
    }
    emit changeOrientation(newOrientation);
}


void
GeneralSetupDialog::onOk() {
    pTempArguments->iTimeoutDuration     = timeoutDurationEdit.text().toInt();
    pTempArguments->maxTimeout           = numTimeoutEdit.text().toInt();
    pTempArguments->maxSet               = maxSetEdit.text().toInt();
    pTempArguments->sSlideDir            = slidesDirEdit.text();
    pTempArguments->sSpotDir             = spotsDirEdit.text();
    pTempArguments->sTeamLogoFilePath[0] = teamLogoPathEdit[0].text();
    pTempArguments->sTeamLogoFilePath[1] = teamLogoPathEdit[1].text();
    accept();
}


void
GeneralSetupDialog::onCancel() {
    reject();
}


void
GeneralSetupDialog::setTeam0Path() {
    teamLogoPathEdit[0].setText(pTempArguments->sTeamLogoFilePath[0]);
    QFile logoFile(pTempArguments->sTeamLogoFilePath[0]);
    if(logoFile.exists()) {
        teamLogoPathEdit[0].setStyleSheet("background:white;color:black;");
    }
    else {
        teamLogoPathEdit[0].setStyleSheet("background:red;color:white;");
    }
}


void
GeneralSetupDialog::setTeam1Path() {
    teamLogoPathEdit[1].setText(pTempArguments->sTeamLogoFilePath[1]);
    QFile logoFile(pTempArguments->sTeamLogoFilePath[0]);
    if(logoFile.exists()) {
        teamLogoPathEdit[1].setStyleSheet("background:white;color:black;");
    }
    else {
        teamLogoPathEdit[1].setStyleSheet("background:red;color:white;");
    }
}
