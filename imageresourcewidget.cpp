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

//Qt
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QLabel>

// Nepomuk
#include <Nepomuk/Resource>

//own
#include "imageresourcewidget.h"
#include "imagewidget.h"
#include "utils.h"


using namespace Crystal;

ImageResourceWidget::ImageResourceWidget(Nepomuk::Resource *resource, QGraphicsWidget *parent)
    : ResourceWidget(resource, parent),
      m_imageWidget(0)
{
    delete m_iconWidget;
    m_iconWidget = 0;
    m_imageWidget = new ImageWidget(this);
    m_imageWidget->setIconSize(m_iconSize);
    m_leftLayout->insertItem(0, m_imageWidget);
    updateUrl();
    connect(this, SIGNAL(urlChanged()), this, SLOT(updateUrl()));
}

ImageResourceWidget::~ImageResourceWidget()
{
}

void ImageResourceWidget::updateUrl()
{
    kDebug() << "SET URL!!!!!!!!" << m_url;
    m_imageWidget->setMimeType(m_mimeType);
    m_imageWidget->setUrl(m_url);
}

QPixmap ImageResourceWidget::pixmap()
{
    return m_imageWidget->pixmap();
}
#include "imageresourcewidget.moc"
