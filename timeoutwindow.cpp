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
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QMessageBox>

#include "timeoutwindow.h"
#include "utility.h"

#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
    #define horizontalAdvance width
#endif


TimeoutWindow::TimeoutWindow(QWidget *parent)
    : QWidget(parent)
{
    Q_UNUSED(parent);
    setMinimumSize(QSize(320, 240));

    QList<QScreen*> screens = QApplication::screens();
    QRect screenGeometry = screens.at(0)->geometry();
    if(screens.count() > 1) {
        screenGeometry = screens.at(1)->geometry();
    // Move the Panel on the Secondary Display
    }
    QPoint point = QPoint(screenGeometry.x(),
                          screenGeometry.y());
    move(point);
    int height = screenGeometry.height();
    int iFontSize = height/4;

    myLabel.setText(QString("No Text"));
    myLabel.setAlignment(Qt::AlignCenter);
    myLabel.setFont(QFont("Arial", iFontSize, QFont::Black));

    panelPalette = QWidget::palette();
    panelGradient = QLinearGradient(0.0, 0.0, 0.0, height);
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

    setWindowOpacity(0.7);
    myLabel.setText(tr("-- No Text --"));
    QVBoxLayout *panelLayout = new QVBoxLayout();
    panelLayout->addWidget(&myLabel);
    setLayout(panelLayout);

    TimerUpdate.setTimerType(Qt::PreciseTimer);
    connect(&TimerUpdate, SIGNAL(timeout()),
            this, SLOT(updateTime()));
    TimerTimeout.setTimerType(Qt::PreciseTimer);
    TimerTimeout.setSingleShot(true);
    connect(&TimerTimeout, SIGNAL(timeout()),
            this, SLOT(updateTime()));
}


TimeoutWindow::~TimeoutWindow() {
}


void
TimeoutWindow::updateTime() {
    int remainingTime = TimerTimeout.remainingTime();
    myLabel.setText(QString("%1").arg(1+(remainingTime/1000)));
    if(remainingTime > 0)
        update();
    else {
        TimerUpdate.stop();
        TimerTimeout.stop();
        emit doneTimeout();
    }
}


void
TimeoutWindow::startTimeout(int msecTime) {
    TimerTimeout.start(msecTime);
    TimerUpdate.start(100);
    myLabel.setText(QString("%1").arg(int(0.999+(TimerTimeout.remainingTime()/1000.0))));
}


void
TimeoutWindow::stopTimeout() {
    TimerUpdate.stop();
    TimerTimeout.stop();
    emit doneTimeout();
}

