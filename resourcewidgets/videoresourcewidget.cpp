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
#include "ontologies/video.h"

using namespace Crystal;

VideoResourceWidget::VideoResourceWidget(Nepomuk::Query::Result result, QGraphicsWidget *parent)
    : ImageResourceWidget(result, parent),
      m_imageWidget(0)
{
    setResource(result.resource());
    /*
    m_width = 0;
    m_height = 0;
    delete m_iconWidget;
    m_iconWidget = 0;
    m_imageWidget = new ImageWidget(this);
    m_imageWidget->setIconSize(m_iconSize);
    m_leftLayout->insertItem(0, m_imageWidget);
    updateUrl();
    connect(this, SIGNAL(urlChanged()), this, SLOT(updateUrl()));
    /*
    // Fade in when this widget appears
    Plasma::Animation* fadeAnimation = Plasma::Animator::create(Plasma::Animator::FadeAnimation);
    fadeAnimation->setTargetWidget(this);
    fadeAnimation->setProperty("startOpacity", 0.0);
    fadeAnimation->setProperty("targetOpacity", 1.0);
    fadeAnimation->setProperty("Duration", 2000);

    fadeAnimation->start();
    */
}

VideoResourceWidget::~VideoResourceWidget()
{
}

void VideoResourceWidget::updateWidgets()
{
    ResourceWidget::updateWidgets();
    if ((m_width + m_height) > 0) {
        m_infoLabel->setText(i18nc("info in the video widget", "Image Size: %1x%2", m_width, m_height));
        kDebug() << "Size: " << m_width << m_height;
    } else {
        kDebug() << "=========> Size not found" << m_resource.properties().keys();
        kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/05/10/nexif#model")).toString().toInt();
        kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#width")).toString().toInt();
        kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height")).toString().toInt();
        //http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height
    }
}

void VideoResourceWidget::setResource(Nepomuk::Resource resource)
{
    //http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height
    //QString m_url = m_resource.property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
    kDebug() << NepomukFast::Video().widthUri();
    kDebug() << NepomukFast::Video().heightUri();
    m_width = resource.property(NepomukFast::Image().verticalResolutionUri()).toString().toInt();
    m_height = resource.property(NepomukFast::Image().horizontalResolutionUri()).toString().toInt();
    //m_height = resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height")).toString().toInt();
    kDebug() << "------> wxh::" << m_width << m_height;
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
