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
#include "resultview.h"

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
  * @short The webby view used to display search results.
  *
  */
  class ResultWebView : public ResultView
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the dialog
        * @param parent the parent of this object
        **/
        ResultWebView(QGraphicsWidget *parent);

        virtual ~ResultWebView();

    public Q_SLOTS:
        /** Call to update the view with new entries after inserting them.
         */
        virtual void updateView();

    protected Q_SLOTS:
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
        QString htmlHeader();

        Plasma::WebView *m_webView;
        // basedir for the webview, all content relative to this path
        StyleSheet *m_css;
        QString m_baseDir;
  };
}

#endif

