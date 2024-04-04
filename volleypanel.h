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
#include <QList>
#include <QVector>
#include <QFileInfoList>
#include <QUrl>
#include <QLabel>

#include "scorepanel.h"

QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(TimeoutWindow)

class VolleyPanel : public ScorePanel
{
    Q_OBJECT

public:
    VolleyPanel(QFile *myLogFile, QWidget *parent = nullptr);
    ~VolleyPanel();
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
    void setTeam(int iTeam, QString sTeamName);
    void setScore(int iTeam, int iScore);
    void setSets(int iTeam, int iSets);
    void setTimeout(int iTeam, int iTimeout);
    void startTimeout(int iTimeoutDuration);
    void stopTimeout();
    void setServizio(int iServizio);
    void setMirrored(bool isPanelMirrored);
    bool getMirrored();
    void setLogo(int iTeam, QString sFileLogo);

private slots:
    void onTimeoutDone();

private:
    void         createPanelElements();
    QGridLayout* createPanel();

private:
    QLabel*           pTeam[2];
    QLabel*           pScore[2];
    QLabel*           pScoreLabel;
    QLabel*           pSet[2];
    QLabel*           pSetLabel;
    QLabel*           pServizio[2];
    QLabel*           pTimeout[2];
    QLabel*           pTimeoutLabel;
    QLabel*           logoLabel[2];
    QLabel*           pCopyRight;
    QPixmap*          pPixmapLogo[2];

    QString           sFontName;
    int               fontWeight;
    QPalette          panelPalette;
    QLinearGradient   panelGradient;
    QBrush            panelBrush;
    int               iServizio;
    int               iTimeoutFontSize;
    int               iSetFontSize;
    int               iScoreFontSize;
    int               iTeamFontSize;
    int               iLabelsFontSize;
    int               maxTeamNameLen;
    QPixmap*          pPixmapService;
    TimeoutWindow*    pTimeoutWindow;
};
