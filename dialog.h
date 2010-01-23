/*
    Copyright 2010 by Sebastian Kügler <sebas@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef CRYSTALDIALOG_H
#define CRYSTALDIALOG_H

//Qt
#include <QLabel>
#include <QStringList>


// KDE
#include <KIO/ListJob>
#include <kio/jobclasses.h>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/TabBar>
#include <Plasma/WebView>

//own
#include "resultwebview.h"
#include "resultwidget.h"
#include "dashboard.h"

//class Applet;

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

namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
  /**
  * @short The panel used to display search results in a popup
  *
  */
  class Dialog : public QGraphicsWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the dialog
        * @param crystal the crystal attached to this dialog
        * @param area where the dialog is displayed
        * @param parent the parent of this object
        **/
        Dialog(QGraphicsWidget *parent);

        virtual ~Dialog();

        void updateIconSize(int iconsize);
        void updateQuery(const QString query);
        void setTimeout(int timeout);

    Q_SIGNALS:
        void updateView();
        void updateToolTip(const QString&, int);

    public Q_SLOTS:
        /**
         * Perform a search
         */
        void search();

        /**
         * @internal Gets called when a new match has been found
         */
        //void newMatches( const QList<Nepomuk::Search::Result>& results);
        void entries(KIO::Job *job, const KIO::UDSEntryList &list);

    private Q_SLOTS:
        void searchFinished();
        void progressChanged(KJob *job, unsigned long percent);

    private :
        /**
        * @internal build the dialog depending where it is
        **/
        void buildDialog();
        void updateStatus(const QString status);
        Plasma::LineEdit *m_lineEdit;
        Plasma::IconWidget *m_searchButton;
        Plasma::TabBar *m_tabBar;
        DashBoard *m_dashBoard;
        ResultView *m_resultsView;
        Plasma::Label *m_statusBar;

        QList<Nepomuk::Resource*> m_results;
        // All icon sizes, indexed
        QHash<int, int> m_iconSizes;
        int m_timeout;
        // Last query ran
        QString m_query;
        int m_abstractSize;
        qreal m_progress;
        QTime m_time;
  };
}

#endif

