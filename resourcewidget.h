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

#ifndef CRYSTALRESOURCEWIDGET_H
#define CRYSTALRESOURCEWIDGET_H

//Qt
#include <QGraphicsWidget>

// KDE
#include <KFileItem>

// Nepomuk
#include <Nepomuk/Resource>

// Plasma
#include <Plasma/IconWidget>

// own
#include "ratingwidget.h"

class QGraphicsGridLayout;
class QGraphicsProxyWidget;
class QGraphicsSceneMouseEvent;

namespace Plasma
{
    class Label;
}

namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
  /**
  * @short ResourceWidget is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class ResourceWidget : public Plasma::IconWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the ResourceWidget
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        ResourceWidget(Nepomuk::Resource resource, QGraphicsWidget *parent = 0);
        virtual ~ResourceWidget();

        static ResourceWidget* create(Nepomuk::Resource resource);

        virtual void setUrl(const QUrl &url);
        virtual void setQuery(const QString &query);
        virtual void setFileItem(const KFileItem &item);
        virtual void setResource(Nepomuk::Resource resource);
        virtual QPixmap pixmap();

    Q_SIGNALS:
        void run(const QUrl&);
        void resourceChanged();
        void urlChanged();

    protected Q_SLOTS:
        void open();
        void mousePressEvent(QGraphicsSceneMouseEvent * event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    protected:
        virtual void updateWidgets();
        virtual void startDrag();

        Nepomuk::Resource m_resource;

        QGraphicsGridLayout *m_layout;
        QGraphicsLinearLayout *m_leftLayout;
        QGraphicsLinearLayout *m_rightLayout;
        Plasma::IconWidget *m_iconWidget;
        Plasma::Label *m_nameLabel;
        Plasma::Label *m_infoLabel;
        RatingWidget *m_ratingWidget;

        QUrl m_url;
        QString m_query;
        KIO::UDSEntry m_udsEntry;

        int m_iconSize;
        QString m_icon;
        QString m_label;
        QString m_info;
        QString m_mimeType;

        QPointF m_startPos;
        KFileItem m_fileItem;
  };
}

#endif

