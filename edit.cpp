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
#include <QtWidgets>
#include <utility>
#include "edit.h"


Edit::Edit(int iTeam, QWidget *parent)
    : QLineEdit(parent)
    , myTeam(iTeam)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}


Edit::Edit(const QString &sString, int iTeam, QWidget *parent)
    : QLineEdit(sString, parent)
    , myTeam(iTeam)
{
    // To make a trasparent background
    setStyleSheet("background-color: rgba(0, 0, 0, 0); border: none");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setText(sString);
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}


QSize
Edit::sizeHint() const
{
    int length = maxLength();
    QFontMetrics f(font());
    QSize size = QLineEdit::sizeHint();
    QMargins margins = contentsMargins();
    size.setHeight(f.height() + f.descent() + f.ascent() + margins.bottom() + margins.top());
    size.setWidth(length*f.maxWidth() + margins.left() + margins.right());
    return size;
}


void
Edit::onTextChanged(QString sText) {
    emit teamTextChanged(std::move(sText), myTeam);
}

