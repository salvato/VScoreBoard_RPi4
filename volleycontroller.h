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

#include "scorecontroller.h"
#include "panelorientation.h"

#include <QTimer>


QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(Edit)
QT_FORWARD_DECLARE_CLASS(myLabel)
QT_FORWARD_DECLARE_CLASS(Button)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(VolleyPanel)
QT_FORWARD_DECLARE_CLASS(ClientListDialog)
QT_FORWARD_DECLARE_CLASS(QFile)


class VolleyController : public ScoreController
{
    Q_OBJECT
public:
    VolleyController(QFile *myLogFile, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);

protected:
    void          GetSettings();
    void          SaveSettings();
    void          setWindowLayout();
    QGridLayout*  CreateGamePanel();
    QHBoxLayout*  CreateGameButtons();
    void          buildFontSizes();
    void          SaveStatus();
    void          GeneralSetup();

private slots:
    void closeEvent(QCloseEvent*);
    void onTimeOutIncrement(int iTeam);
    void onTimeOutDecrement(int iTeam);
    void onSetIncrement(int iTeam);
    void onSetDecrement(int iTeam);
    void onScoreIncrement(int iTeam);
    void onScoreDecrement(int iTeam);
    void onServiceClicked(int iTeam);
    void onTeamTextChanged(QString sText, int iTeam);
    void onButtonChangeFieldClicked();
    void onButtonNewSetClicked();
    void onButtonNewGameClicked();
    void onChangePanelOrientation(PanelOrientation orientation);

private:
    void          buildControls();
    void          setEventHandlers();
    void          sendAll();
    void          btSendAll();
    void          processBtMessage(QString sMessage);
    void          exchangeField();
    void          startNewSet();

private:
    VolleyPanel*  pVolleyPanel;
    int           iTimeout[2]{};
    int           iSet[2]{};
    int           iScore[2]{};
    int           iServizio{};
    int           lastService{};
    Edit*         pTeamName[2]{};
    Edit*         pTimeoutEdit[2]{};
    Edit*         pSetsEdit[2]{};
    Edit*         pScoreEdit[2]{};
    Button*       pTimeoutIncrement[2]{};
    Button*       pTimeoutDecrement[2]{};
    Button*       pSetsIncrement[2]{};
    Button*       pSetsDecrement[2]{};
    Button*       pScoreIncrement[2]{};
    Button*       pScoreDecrement[2]{};
    Button*       pService[2]{};
    QLabel*       pTimeoutLabel{};
    QLabel*       pSetsLabel{};
    QLabel*       pServiceLabel{};
    QLabel*       pScoreLabel{};

    QPushButton*  pNewSetButton{};
    QPushButton*  pNewGameButton{};
    QPushButton*  pChangeFieldButton{};

    bool            bFontBuilt;
    QPalette        panelPalette;
    QLinearGradient panelGradient;
    QBrush          panelBrush;
    int             maxTeamNameLen;
};

