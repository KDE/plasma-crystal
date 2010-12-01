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
#include <QGraphicsLinearLayout>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

// Plasma
#include <Plasma/Label>

//own
#include "resourcewidgets/videoresourcewidget.h"
#include "resourcewidgets/imageresourcewidget.h"
#include "imagewidget.h"
#include "utils.h"

// generated
#include "video.h"

using namespace Crystal;

VideoResourceWidget::VideoResourceWidget(QGraphicsWidget *parent)
    : ImageResourceWidget(parent),
      m_imageWidget(0)
{
    //setResource(result.resource());
}

VideoResourceWidget::~VideoResourceWidget()
{
}

void VideoResourceWidget::updateWidgets()
{
    ResourceWidget::updateWidgets();
    if ((m_width + m_height) > 0) {
        m_infoLabel->setText(i18nc("info in the video widget", "Resolution: %1x%2", m_width, m_height));
        kDebug() << "Size: " << m_width << m_height;
    } else {
        kDebug() << "=========> Size not found" << m_resource.properties().keys();
        //http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height
    }
}

void VideoResourceWidget::setResource(Nepomuk::Resource resource)
{
    m_video = Nepomuk::Video(resource);
    kDebug() << "VERT:" << m_video.heights();
    kDebug() << "WITH" << m_video.widths();
    m_width = resource.property(Nepomuk::Image().verticalResolutionUri()).toString().toInt();
    m_height = resource.property(Nepomuk::Image().horizontalResolutionUri()).toString().toInt();
    kDebug() << "------> video resolution wxh::" << m_width << m_height;
    ResourceWidget::setResource(resource);
}
void VideoResourceWidget::updateUrl()
{
    m_imageWidget->setMimeType(m_mimeType);
    m_imageWidget->setUrl(m_url);
}

QPixmap VideoResourceWidget::pixmap()
{
    return m_imageWidget->pixmap();
}

#include "videoresourcewidget.moc"
