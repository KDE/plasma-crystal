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

#include "imagewidget.h"

//Qt
#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

// KDE
#include <KFileMetaInfo>
#include <KIcon>
#include <KIO/PreviewJob>
#include <KFileItem>

// Plasma
#include <Plasma/Theme>


using namespace Crystal;

ImageWidget::ImageWidget(QGraphicsWidget* parent)
    : QGraphicsWidget(parent),
    m_icon(QString("image-x-generic")),
    m_iconSize(48),
    m_previewJob(0)
{
    border = 1; // should be a power of two, otherwise we get blurry lines
    fg = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);
    bg = Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor);
    setIconSize(m_iconSize);
    setAcceptHoverEvents(false);
    setAcceptTouchEvents(false);
    setAcceptedMouseButtons(0);
    setAcceptDrops(false);

}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::setIcon(const QString& icon)
{
    m_icon = icon;
    //pixmapUpdated();
}


void ImageWidget::setIconSize(int iconSize)
{
    m_iconSize = iconSize;
    setMinimumSize(m_iconSize, m_iconSize);
    setMaximumSize(m_iconSize, m_iconSize);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ImageWidget::setUrl(const QUrl& url)
{
    m_url = QUrl(url.toString(QUrl::None));
    // apparently needed to make PreviewJob not fail for local files :/
    if (url.toString(QUrl::None).startsWith('/')) {
        m_url = QUrl(QString("file:%1").arg(url.toString(QUrl::None)));
    }
    kDebug() << url.toString(QUrl::None) << m_url << QString("file:/%1").arg(url.toString(QUrl::None));
}

void ImageWidget::setMimeType(const QString &mime)
{
    m_mimeType = mime;
}

void ImageWidget::previewJobFailed(const KFileItem &item)
{

    kDebug() << "preview failed for" << item.url();
}

void ImageWidget::previewResult(KJob* job)
{

    kDebug() << "job result:" << job->errorText() << "success?" << (job->error() == 0);
}

void ImageWidget::previewUpdated(const KFileItem &item, const QPixmap &preview)
{
    kDebug() << "preview for" << m_url << "is in.";
    Q_UNUSED( item )
    m_pixmap = preview;
    pixmapUpdated();
    update();
}

void ImageWidget::pixmapUpdated()
{
    QSize newSize = QSize(contentsRect().width() - (border * 2), contentsRect().height() - (border * 2));
    if (newSize.isEmpty()) {
        m_scaledPixmap = QPixmap();
        return;
    }
    if (!m_pixmap.isNull()) {
        if (newSize.width() > m_pixmap.width()) {
            newSize.setWidth(m_pixmap.width());
        }
        if (newSize.height() > m_pixmap.height()) {
            newSize.setHeight(m_pixmap.height());
            setMinimumHeight(newSize.height());
            setMaximumHeight(newSize.height());
        }
        m_scaledPixmap = m_pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        m_scaledPixmap = KIcon(m_icon).pixmap(newSize);
    }
}

void ImageWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!m_previewJob && m_url.isValid()) {
        kDebug() << "Starting previewjob" << m_url;
        // KIO::PreviewJob: http://api.kde.org/4.x-api/kdelibs-apidocs/kio/html/classKIO_1_1PreviewJob.html
        KFileItem kfile(KUrl(m_url), m_mimeType, KFileItem::Unknown);
        KFileItemList list;
        list << kfile;
        KIO::PreviewJob *job = new KIO::PreviewJob(list, m_iconSize, m_iconSize, m_iconSize, 128, true, true, 0);
        connect(job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(previewUpdated(const KFileItem&, const QPixmap&)));
        connect(job, SIGNAL(failed(const KFileItem&)), SLOT(previewJobFailed(const KFileItem&)));
        connect(job, SIGNAL(result(KJob*)), SLOT(previewResult(KJob*)));
        m_previewJob = job;
        job->start();
        pixmapUpdated();
    }
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);

    bg.setAlphaF(.3);
    fg.setAlphaF(.2);
    painter->setBrush(bg);
    painter->setPen(fg);
    painter->translate(.5, .5); // unblur (align to full pixel)

    // compute rect of the pixmap to paint the frame
    QRectF r = QRect(contentsRect().left(), contentsRect().top(), m_scaledPixmap.width() + border*2 - 1, m_scaledPixmap.height() + border*2 - 1);
    painter->drawRoundedRect(r, border*2, border*2);
    painter->translate(-.5, -.5); // unblur (align to full pixel)

    // paint our cached scaled version of the pixmap on top of that
    painter->drawPixmap(QPoint(border, border), m_scaledPixmap);
}

void ImageWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    Q_UNUSED(event)

    pixmapUpdated();
}

QPixmap ImageWidget::pixmap()
{
    return m_pixmap;
}

#include "imagewidget.moc"
