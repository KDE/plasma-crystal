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

#ifndef CRYSTALIMAGERESOURCEWIDGET_H
#define CRYSTALIMAGERESOURCEWIDGET_H

// own
#include "resourcewidget.h"

//Qt
#include <QGraphicsWidget>
class QGraphicsGridLayout;
class QGraphicsProxyWidget;

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Frame>


namespace Plasma
{
    class IconWidget;
    class Label;
}

namespace KIO
{
    class UDSEntry;
}

namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
    class ImageWidget;
  /**
  * @short ImageResourceWidget is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class ImageResourceWidget : public ResourceWidget
  //class ImageResourceWidget : public Plasma::Frame
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the ImageResourceWidget
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        ImageResourceWidget(Nepomuk::Resource *resource, QGraphicsWidget *parent = 0);
        virtual ~ImageResourceWidget();
        virtual QPixmap pixmap();

        //void setUrl(const QUrl &url);

    protected:
        ImageWidget *m_imageWidget;

    private Q_SLOTS:
        void updateUrl();
  };
}

#endif

