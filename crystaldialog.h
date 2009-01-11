/***************************************************************************
 *   Copyright 2008-2009 by Sebastian Kügler <sebas@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef CRYSTALDIALOG_H
#define CRYSTALDIALOG_H

//Qt
#include <QLabel>
#include <QStringList>


// KDE
#include <KLineEdit>
#include <KPushButton>

// Nepomuk
#include <Nepomuk/Types/Class>
#include <Soprano/Vocabulary/Xesam>
#include <nepomuk/queryserviceclient.h>
#include <nepomuk/result.h>
#include <nepomuk/searchhitview.h>


//own
#include <mediawiki.h>
class CrystalApplet;

//desktop view
namespace Plasma
{
    class Icon;
    class Dialog;
}

namespace Solid
{
    class Device;
}


namespace Crystal
{
  /**
  * @short The panel used to display search results in a popup
  *
  */
  class CrystalDialog : public QObject
  {
  Q_OBJECT

      public:
          /**
          * Constructor of the dialog
          * @param crystal the crystal attached to this dialog
          * @param area where the dialog is displayed
          * @param parent the parent of this object
          **/
          CrystalDialog(CrystalApplet * crystal,QObject *parent = 0);

          virtual ~CrystalDialog();

          /**
          * Returns the related QWidget.
          **/
          QWidget * dialog();
          void updateIconSize();
          void updateQuery(const QString query);

      public Q_SLOTS:
        /**
         * Perform a search
         */
        void search();


        void wikiFinished(bool done);
        /**
         * @internal Gets called when a new match has been found
         */
        void newMatches( const QList<Nepomuk::Search::Result>& results);

      private Q_SLOTS:
          /**
          * @internal update the color of the label to follow plasma theme
          *
          **/
          void updateColors();
          void searchFinished();
          void run( const QUrl& );

      private :
          /**
          * @internal build the dialog depending where it is
          **/
          void buildDialog();
          void updateStatus(const QString status);

          void setupWiki();

          // The widget which display the panel
          QWidget *m_widget;

          Nepomuk::SearchHitView *m_resultsView;
          Nepomuk::Search::QueryServiceClient *m_queryServiceClient;

          KLineEdit * m_lineEdit;
          KPushButton * m_searchButton;
          QLabel * m_statusLabel;

          ///The applet attached to this item
          CrystalApplet * m_crystal;

          // Matches found for the current query
          int m_matches;
          // Last query ran
          QString m_query;
          // All icon sizes, indexed
          QHash<int, int> m_iconSizes;
          MediaWiki* m_wiki;
  };

}

#endif

