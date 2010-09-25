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

//own
#include "resultview.h"
#include "resourcewidget.h"

class CrystalApplet;

namespace Plasma
{
    class ScrollWidget;
}

namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
  /**
  * @short A plasma-widget view used to display search results.
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
        virtual void clear();

    public Q_SLOTS:
        void newEntries(const QList< Nepomuk::Query::Result > &entries);

    protected Q_SLOTS:
        void addWidget(Nepomuk::Resource, const QString& = QString(), const KFileItem& = KFileItem() );
        virtual void updateView();

    private:
        void buildDialog();
        QGraphicsWidget *m_widget;
        QGraphicsLinearLayout *m_outerLayout;
        Plasma::ScrollWidget *m_scrollWidget;
        QGraphicsLinearLayout *m_layout;
        QList<ResourceWidget*> m_widgets;
  };
}

#endif

