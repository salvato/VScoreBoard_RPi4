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
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

#include "utility.h"


void
logMessage(QFile *logFile, QString sFunctionName, QString sMessage) {
    QDateTime dateTime;
    QString sDebugMessage = dateTime.currentDateTime().toString() +
                            QString(" - ") +
                            sFunctionName +
                            QString(" - ") +
                            sMessage;
    if(logFile) {
        if(logFile->isOpen()) {
            logFile->write(sDebugMessage.toUtf8().data());
            logFile->write("\n");
            logFile->flush();
        }
        else
            qDebug() << sDebugMessage;
    }
    else
        qDebug() << sDebugMessage;
}

