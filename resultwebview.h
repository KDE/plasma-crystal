/***************************************************************************
 *   Copyright 2008-2010 by Sebastian Kügler <sebas@kde.org>               *
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

#ifndef CRYSTALRESULTWEBVIEW_H
#define CRYSTALRESULTWEBVIEW_H

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
#include <Plasma/WebView>

//own
#include "stylesheet.h"

class CrystalApplet;

//desktop view
namespace Plasma
{
    class Icon;
    class WebView;
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
  class ResultWebView : public Plasma::WebView
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the dialog
        * @param crystal the crystal attached to this dialog
        * @param area where the dialog is displayed
        * @param parent the parent of this object
        **/
        ResultWebView(QGraphicsWidget *parent);

        virtual ~ResultWebView();

        void setQuery(const QString&);

    Q_SIGNALS:
        void run(const QUrl&);

    public Q_SLOTS:
        /** Call to update the view with new entries after inserting them.
         */
        void updateView();

        int count();

        /**
         * @internal Gets called when a new match has been found
         */
        void addMatch(const KIO::UDSEntry& entry);

    private Q_SLOTS:
        /**
        * @internal update the color of the label to follow plasma theme
        *
        **/
        void updateColors();

    private:
        /**
        * @internal build the dialog depending where it is
        **/
        QString renderItem(Nepomuk::Resource *res);
        QString abstract(Nepomuk::Resource *res);
        QString htmlHeader();

        ///The applet attached to this item
        QList<Nepomuk::Resource*> m_results;

        // Last query ran
        QString m_query;
        int m_abstractSize;
        // basedir for the webview, all content relative to this path
        StyleSheet *m_css;
        QString m_baseDir;
  };
}

#endif

