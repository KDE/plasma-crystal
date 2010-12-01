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


#ifndef CRYSTALIMAGEWIDGET_H
#define CRYSTALIMAGEWIDGET_H

//Qt
#include <QtCore/QUrl>
#include <QtGui/QGraphicsWidget>


class KFileItem;
class KJob;

namespace Crystal
{
    class ImageWidget;
class ImageWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QGraphicsWidget* parent = 0);
    ~ImageWidget();

    void setUrl(const QUrl& url);
    void setMimeType(const QString &mime);
    void setIconSize(int iconSize);
    QPixmap pixmap();

private Q_SLOTS:
    void previewUpdated(const KFileItem &item, const QPixmap &preview);
    void previewJobFailed(const KFileItem &item);

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent* event);
    void pixmapUpdated();

    int border;
    QColor fg;
    QColor bg;

    QPixmap m_pixmap;
    QPixmap m_scaledPixmap;

    QUrl m_url;
    QString m_icon;
    QString m_mimeType;
    int m_iconSize;

    KJob *m_previewJob;
};
}

#endif
