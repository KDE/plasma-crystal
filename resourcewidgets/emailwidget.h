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

#ifndef CRYSTALEMAILWIDGET_H
#define CRYSTALEMAILWIDGET_H

// own
#include "../resourcewidget.h"

//Qt
#include <QGraphicsWidget>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Frame>


class QGraphicsGridLayout;
class QGraphicsProxyWidget;

namespace Plasma
{
    class IconWidget;
    class Label;
}

namespace Nepomuk
{
    class Resource;
}

using namespace Nepomuk;

namespace Crystal
{
  /**
  * @short EmailWidget is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class EmailWidget : public ResourceWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the EmailWidget
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        EmailWidget(QGraphicsWidget *parent = 0);
        virtual ~EmailWidget();

        virtual QString info();
        virtual void setResource(Nepomuk::Resource resource);

    protected:
        virtual void updateWidgets();

    private:
        Email m_email;
  };
}

#endif

