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

#include <QObject>
#include <QMainWindow>
#include <QtGlobal>
#include <QTranslator>
#include <QTimer>


#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
    #define horizontalAdvance width
#endif


QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_END_NAMESPACE


class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    ScorePanel(QFile *myLogFile, QWidget *parent = Q_NULLPTR);
    ~ScorePanel();
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void panelClosed();

protected:
    virtual QGridLayout* createPanel();
    void buildLayout();

protected:
    bool               isMirrored;
    QFile*             pLogFile;
    QTranslator        Translator;

private:
    // Logging Messages
    QString            logFileName;
    QWidget*           pPanel;
};
