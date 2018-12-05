/*
 * Copyright (C) 1995 Paul Olav Tvete <paul@troll.no>
 * Copyright (C) 2000-2009 Stephan Kulow <coolo@kde.org>
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

#include "dealer.h"
#include "dealerinfo.h"
#include "mainwindow.h"
#include "version.h"

#include "KCardTheme"
#include "KCardDeck"

#include <KAboutData>


#include <QDebug>
#include <KLocalizedString>
#include <KDBusService>

#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QResizeEvent>
#include <QtXml/QDomDocument>

#include <climits>
#include <time.h>
#include <QStandardPaths>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

static DealerScene *getDealer( int wanted_game )
{
    foreach ( DealerInfo * di, DealerInfoList::self()->games() )
    {
        if ( di->providesId( wanted_game ) )
        {
            DealerScene * d = di->createGame();
            Q_ASSERT( d );
            d->setDeck( new KCardDeck( KCardTheme(), d ) );
            d->initialize();

            if ( !d->solver() )
            {
                qCritical() << "There is no solver for" << di->nameForId( wanted_game );;
                return 0;
            }

            return d;
        }
    }
    return 0;
}

// A function to remove all nonalphanumeric characters from a string
// and convert all letters to lowercase.
QString lowerAlphaNum( const QString & string )
{
    QString result;
    for ( int i = 0; i < string.size(); ++i )
    {
        QChar c = string.at( i );
        if ( c.isLetterOrNumber() )
            result += c.toLower();
    }
    return result;
}

int main( int argc, char **argv )
{
    QApplication app(argc, argv);

    KAboutData aboutData( "klondikebattle",
                          i18n("Klondike Battle"),
                          KLONDIKEBATTLE_VERSION,
                          i18n("KDE Russian Bank"),
                          KAboutLicense::GPL_V2,
                          i18n("© 2016 Olivier Felt"),
                          QString(),
                          "http://games.kde.org/kpat" );

    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.addAuthor( i18n("Olivier Felt"),
                         i18n("Author of original version"),
                         "olivier.felt@kdemail.net" );

    // Create a KLocale earlier than normal so that we can use i18n to translate
    // the names of the game types in the help text.
    QMap<QString, int> indexMap;
    QStringList gameList;
    foreach ( const DealerInfo *di, DealerInfoList::self()->games() )
    {
        KLocalizedString localizedKey = ki18n( di->untranslatedBaseName().constData() );
        //QT5 const QString translatedKey = lowerAlphaNum( localizedKey.toString( tmpLocale ) );
        //QT5 gameList << translatedKey;
        //QT5 indexMap.insert( translatedKey, di->baseId() );
        indexMap.insert( di->baseIdString(), di->baseId() );
    }
    gameList.sort();
    const QString listSeparator = i18nc( "List separator", ", " );

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();

    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("solvegame"), i18n( "Try to find a solution to the given savegame" ), QLatin1String("file")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("solve"), i18n("Dealer to solve (debug)" ), QLatin1String("num")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("start"), i18n("Game range start (default 0:INT_MAX)" ), QLatin1String("num")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("end"), i18n("Game range end (default start:start if start given)" ), QLatin1String("num")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("testdir"), i18n( "Directory with test cases" ), QLatin1String("directory")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("generate"), i18n( "Generate random test cases" )));
    parser.addPositionalArgument(QLatin1String("file"), i18n("File to load"));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    app.setWindowIcon(QIcon::fromTheme(QLatin1String("kpat")));

    QString savegame = parser.value( "solvegame" );
    if ( !savegame.isEmpty() )
    {
        QFile of(savegame);
        of.open(QIODevice::ReadOnly);
        QDomDocument doc;
        doc.setContent(&of);

        DealerScene *f = getDealer( doc.documentElement().attribute("id").toInt() );

        f->loadLegacyFile( &of );
        f->solver()->translate_layout();
        int ret = f->solver()->patsolve();
        if ( ret == Solver::SolutionExists )
            fprintf( stdout, "won\n");
        else if ( ret == Solver::NoSolutionExists )
            fprintf( stdout, "lost\n" );
        else
            fprintf( stdout, "unknown\n");

        return 0;
    }

    QString testdir = parser.value("testdir");
    if ( !testdir.isEmpty() ) {
       qsrand(time(0));
       if ( parser.isSet("generate") ) {
          for (int dealer = 0; dealer < 20; dealer++) {
              DealerScene *f = getDealer( dealer );
              if (!f) continue;
              int count = 100;
              QTime mytime;
              while (count) {
                if (f->deck()) f->deck()->stopAnimations();
                int i = qrand() % INT_MAX;
                f->startNew( i );
                mytime.start();
                f->solver()->translate_layout();
                int ret = f->solver()->patsolve();
                if ( ret == Solver::SolutionExists ) {
                   fprintf( stdout, "%d: %d won (%d ms)\n", dealer, i, mytime.elapsed() );
                   count--;
                   QFile file(QString("%1/%2-%3-1").arg(testdir).arg(dealer).arg(i));
                   file.open( QFile::WriteOnly );
                   f->saveLegacyFile( &file );
                }
                else if ( ret == Solver::NoSolutionExists ) {
                   fprintf( stdout, "%d: %d lost (%d ms)\n", dealer, i, mytime.elapsed()  );
                   count--;
                   QFile file(QString("%1/%2-%3-0").arg(testdir).arg(dealer).arg(i));
                   file.open( QFile::WriteOnly );
                   f->saveLegacyFile( &file );
                } else {
                   fprintf( stdout, "%d: %d unknown (%d ms)\n", dealer, i, mytime.elapsed() );
                }
             }
          }
       } 
       return 0;
    }

    bool ok = false;
    int wanted_game = -1;
    if ( parser.isSet( "solve" ) )
        wanted_game = parser.value("solve").toInt( &ok );
    if ( ok )
    {
        ok = false;
        int end_index = -1;
        if ( parser.isSet( "end" ) )
            end_index = parser.value("end").toInt( &ok );
        if ( !ok )
            end_index = -1;
        ok = false;
        int start_index = -1;
        if ( parser.isSet( "start" ) )
            start_index = parser.value("start").toInt( &ok );
        if ( !ok ) {
            start_index = 0;
            end_index = INT_MAX;
        } else {
            if ( end_index == -1 )
                end_index = start_index;
        }
        DealerScene *f = getDealer( wanted_game );
        if ( !f )
            return 1;

        QTime mytime;
        for ( int i = start_index; i <= end_index; i++ )
        {
            mytime.start();
            f->deck()->stopAnimations();
            f->startNew( i );
            f->solver()->translate_layout();
            int ret = f->solver()->patsolve();
            if ( ret == Solver::SolutionExists )
                fprintf( stdout, "%d won (%d ms)\n", i, mytime.elapsed() );
            else if ( ret == Solver::NoSolutionExists )
                fprintf( stdout, "%d lost (%d ms)\n", i, mytime.elapsed()  );
            else
                fprintf( stdout, "%d unknown (%d ms)\n", i, mytime.elapsed() );
        }
        fprintf( stdout, "all_moves %ld\n", all_moves );
        return 0;
    }

    QString gametype = parser.value("gametype").toLower();
    QFile savedState( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + saved_state_file) ;

    MainWindow *w = new MainWindow;
    if (parser.positionalArguments().count())
    {
        if ( !w->loadGame( QUrl::fromLocalFile(parser.positionalArguments().at( 0 )), true ) )
            w->slotShowGameSelectionScreen();
    }
    else if (indexMap.contains(gametype))
    {
        w->slotGameSelected(indexMap.value(gametype));
    }
    else if (savedState.exists())
    {
        if ( !w->loadGame( QUrl::fromLocalFile(savedState.fileName()), false ) )
            w->slotShowGameSelectionScreen();
    }
    else
    {
        w->slotShowGameSelectionScreen();
    }
    w->show();

    const KDBusService dbusService(KDBusService::Multiple);
    
    return app.exec();
}
