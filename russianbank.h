/*
 * Copyright (C) 1995 Paul Olav Tvete <paul@troll.no>
 * Copyright (C) 2000-2009 Stephan Kulow <coolo@kde.org>
 * Copyright (C) 2010 Parker Coates <coates@kde.org>
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

#ifndef RUSSIANBANK_H
#define RUSSIANBANK_H

#include "dealer.h"
#include "player.h"

#include <KgDifficulty>

class QLabel;

class KToggleAction;
class KSelectAction;

class Krapette : public DealerScene
{
    Q_OBJECT

public:
    Krapette(const DealerInfo *di);
    virtual void initialize();
    QList<QAction*> configActions() const;
    KgDifficulty* getKgDifficulty() const;
    virtual void moveCardsToPile( const QList<KCard*> & cards, KCardPile * pile, int duration );
    
    bool isGameWon() const;
    bool isGameLost() const;

    enum AISpeed {
        AI_SLOW = 500,
        AI_NORMAL = 300,
        AI_FAST = 100
    };
    
public slots:
    virtual bool checkAdd(const PatPile *pile, const QList<KCard*> &oldCards, const QList<KCard*> &newCards) const;
    virtual bool checkRemove(const PatPile *pile, const QList<KCard*> &cards) const;
    virtual void cardsMoved(const QList<KCard*> &cards, KCardPile *oldPile, KCardPile *newPile);
    virtual void restart(const QList<KCard*> &cards);
    
protected:
    virtual QString getGameState() const;
    virtual void setGameState( const QString & state );
    
protected slots:
    virtual void animationDone();
    virtual bool newCards();
    
private:
    const int movesShortcuts[2][12] = { {0,1,2,3,3,4,5,5,5,5,6,6},{0,0,1,2,2,3,3,3,3,4,4,4} };
    const int aiSpeedTab[3] = {Krapette::AI_SLOW, Krapette::AI_NORMAL, Krapette::AI_FAST};

    void toggleCompulsoryMoves(bool enabled);
    void toggleMovesShortcuts(bool enabled);
    void setAISpeed();
    KSelectAction *m_aiSpeedSelectAction;
    KToggleAction *m_compulsoryMovesToggleAction;
    KToggleAction *m_movesShortcutsToggleAction;
    bool m_movesShortcutsEnabled;
    bool m_compulsoryMovesEnabled;
    int m_aiDurationMove;
    int m_aiTimeBetweenMoves;
    QLabel *m_player1StatusLabel;

    void changePlayer();
    Player* getOpponent() const;
    PatPile* getActiveReserve() const;
    PatPile* getActiveWaste() const;
    PatPile* getActiveTalon() const;
    int countEmptyPlayPiles() const;
    bool checkDrawActionPossible();
    bool tryMoveCardToPile(KCard *card);
    bool checkCompulsoryMoves() const;
    bool doCompulsoryMoves();
    bool isMoveUseless(PatPile *originPile, PatPile *destPile);
    bool putCardToPile(KCard *card, PatPile *destPile);
    bool putTabledCardToPlayerPile(PatPile *destPile);
    void startAI();
    void tryMoveAI();
    
    QTimer m_aiTimer;
    QVector<KCard*> m_cardsPlayed;
    KgDifficulty *m_difficulty;
    Player *m_currentPlayer;
    Player *m_player1;
    Player *m_player2;
    PatPile *m_play[8];
    PatPile *m_target[8];
};

#endif
