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


#ifndef CRYSTALRATINGWIDGET_H
#define CRYSTALRATINGWIDGET_H

//Qt
#include <QtGui/QGraphicsWidget>

namespace Crystal
{
    class RatingWidget;
class RatingWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    explicit RatingWidget(QGraphicsWidget* parent = 0);
    ~RatingWidget();

    void setRating(int rating);

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent* event);

    int m_rating;
};
}

#endif
