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

namespace Crystal
{
  class ImageWidget;
  /**
  * @short ImageResourceWidget is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class ImageResourceWidget : public ResourceWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the ImageResourceWidget
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        ImageResourceWidget(QGraphicsWidget *parent = 0);
        virtual ~ImageResourceWidget();
        virtual QPixmap pixmap();
        virtual void setResource(Nepomuk::Resource resource);

        //void setUrl(const QUrl &url);

    protected:
        virtual void updateWidgets();
        ImageWidget *m_imageWidget;
        int m_width;
        int m_height;

    protected Q_SLOTS:
        void updateUrl();

  };
}

#endif

