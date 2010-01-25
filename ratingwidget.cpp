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

#include "ratingwidget.h"

//Qt
#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

// KDE
#include <KIcon>

// Nepomuk
#include <Nepomuk/KRatingPainter>


using namespace Crystal;

RatingWidget::RatingWidget(QGraphicsWidget* parent)
    : QGraphicsWidget(parent),
    m_rating(0)
{
    setMinimumSize(48,12);
    setMaximumSize(48,12);
}

RatingWidget::~RatingWidget()
{
}

void RatingWidget::setRating(int rating)
{
    m_rating = rating;
}

void RatingWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor("orange"));

    painter->setBrush(brush);

    painter->translate(.5, .5); // unblur (align to full pixel)

    int border = 2;
    //painter->drawRoundedRect(contentsRect(), border*2, border*2);
    //painter->translate(-.5, -.5); // unblur (align to full pixel)

    //paintRating (QPainter *p, const QRect &rect, Qt::Alignment align, int rating, int hoverRating=-1)

    QRect r = contentsRect().toRect();

    KRatingPainter::paintRating(painter, r, Qt::AlignHCenter, m_rating);

    //painter->drawPixmap(QPoint(border, border), m_scaledPixmap);

}


void RatingWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    Q_UNUSED(event)
}

#include "ratingwidget.moc"
