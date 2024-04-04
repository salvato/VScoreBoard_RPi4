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

#include "button.h"


Button::Button(const QString &text, int team, QWidget *parent)
    : QPushButton(text, parent)
    , myTeam(team)
{
    QSizePolicy p(sizePolicy());
    p.setHeightForWidth(true);
    p.setHorizontalPolicy(QSizePolicy::Expanding);
    p.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(p);
    setFlat(true);
    connect(this, SIGNAL(clicked()),
            this, SLOT(onButtonClicked()));
}


void
Button::onButtonClicked() {
    emit buttonClicked(myTeam);
}


bool
Button::hasHeightForWidth() const
{
    return true;
}


int
Button::heightForWidth(int w) const
{
    return w;
}
