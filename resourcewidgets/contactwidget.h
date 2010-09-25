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

#ifndef CRYSTALCONTACTWIDGET_H
#define CRYSTALCONTACTWIDGET_H

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

using namespace NepomukFast;

namespace Crystal
{
  /**
  * @short ContactWidget is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class ContactWidget : public ResourceWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the ContactWidget
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        ContactWidget(Nepomuk::Resource resource, QGraphicsWidget *parent = 0);
        virtual ~ContactWidget();
        //virtual QPixmap pixmap();
        //virtual void setResource(Nepomuk::Resource resource);

        //void setUrl(const QUrl &url);

    protected:
        virtual void updateWidgets();
        //ImageWidget *m_imageWidget;

    /*
    private Q_SLOTS:
        void updateUrl();
    */
    private:
        PersonContact m_contact;
  };
}

#endif

