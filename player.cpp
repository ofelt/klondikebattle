/*
 * Copyright (C) 2015 Olivier Felt <olivier.felt@kdemail.net>
 *
 * License of original code:
 * -------------------------------------------------------------------------
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation.
 *
 *   This file is provided AS IS with no warranties of any kind.  The author
 *   shall have no liability with respect to the infringement of copyrights,
 *   trade secrets or any patents by this file or any part thereof.  In no
 *   event will the author be liable for any lost revenue or profits or
 *   other special, indirect and consequential damages.
 * -------------------------------------------------------------------------
 *
 * License of modifications/additions made after 2009-01-01:
 * -------------------------------------------------------------------------
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------
 */

#include "player.h"

Player::Player(PatPile *reserve, PatPile *talon, PatPile *waste, bool human, QString name)
{
    m_reserve = reserve;
    m_talon = talon;
    m_waste = waste;
    m_human = human;
    m_name = name;
}
PatPile* Player::reserve() const
{
    return m_reserve;
}
PatPile* Player::talon() const
{
    return m_talon;
}
PatPile* Player::waste() const
{
    return m_waste;
}
bool Player::isHuman() const
{
    return m_human;
}
QString Player::name() const
{
    return m_name;
}
int Player::getTotalCards() const
{
    return m_talon->count() + m_reserve->count() + m_waste->count();
}