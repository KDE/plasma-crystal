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

#ifndef CRYSTALRESULTWIDGET_H
#define CRYSTALRESULTWIDGET_H

//Qt
#include <QGraphicsLinearLayout>
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
#include "resourcewidget.h"

class CrystalApplet;

//desktop view
namespace Plasma
{
    class Icon;
    class ScrollWidget;
    class WebView;
}

namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
  /**
  * @short A webby view used to display search results.
  *
  */
  class ResultWidget : public ResultView
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the dialog
        * @param parent the parent of this object
        **/
        ResultWidget(QGraphicsWidget *parent);

        virtual ~ResultWidget();

    protected Q_SLOTS:
        void addWidget(Nepomuk::Resource*);

    private:
        Plasma::ScrollWidget *m_scrollWidget;
        QGraphicsLinearLayout *m_layout;
        QList<ResourceWidget*> m_widgets;
  };
}

#endif

