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

// Qt
#include <QApplication>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
//#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QGraphicsSceneMouseEvent>

// KDE
#include <KDebug>
#include <KFileItem>
#include <KGlobalSettings>
#include <KIcon>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Label>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include "video.h"
#include "photo.h"
#include "rasterimage.h"
// Own
#include "resourcewidget.h"
#include "resourcewidgets/contactwidget.h"
#include "resourcewidgets/emailwidget.h"
#include "resourcewidgets/imageresourcewidget.h"
#include "resourcewidgets/videoresourcewidget.h"
#include "ratingwidget.h"
#include "utils.h"


using namespace Crystal;

ResourceWidget::ResourceWidget(QGraphicsWidget *parent)
    : Plasma::IconWidget(parent),
    //: Plasma::Frame(parent),
      m_dumpProperties(true),
      m_result(),
      m_resource(),
      m_layout(0),
      m_iconWidget(0),
      m_nameLabel(0),
      m_infoLabel(0),
      m_ratingWidget(0),
      m_iconSize(48),
      m_icon("nepomuk")

{
    setDrawBackground(true);
    setMinimumHeight(64);

    connect(this, SIGNAL(activated()), this, SLOT(open()));
    m_layout = new QGraphicsGridLayout(this);
    m_layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_leftLayout = new QGraphicsLinearLayout(m_layout);
    m_leftLayout->setOrientation(Qt::Vertical);
    m_rightLayout = new QGraphicsLinearLayout(m_layout);
    m_rightLayout->setOrientation(Qt::Vertical);

    m_layout->setColumnFixedWidth(0, m_iconSize);
    m_layout->setRowStretchFactor(0, 1);
    m_layout->setRowStretchFactor(1, 100);
    m_layout->setRowStretchFactor(2, 1);

    m_iconWidget = new Plasma::IconWidget(this);
    m_iconWidget->setIcon("nepomuk");
    m_iconWidget->setMinimumSize(m_iconSize, m_iconSize);
    m_iconWidget->setMaximumSize(m_iconSize, m_iconSize);
    m_iconWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_iconWidget->setAcceptHoverEvents(false);
    m_iconWidget->setAcceptTouchEvents(false);
    m_iconWidget->setAcceptedMouseButtons(0);
    m_iconWidget->setAcceptDrops(false);
    m_leftLayout->addItem(m_iconWidget);
    connect(m_iconWidget, SIGNAL(activated()), this, SLOT(open()));

    m_ratingWidget = new RatingWidget(this);
    m_leftLayout->addItem(m_ratingWidget);

    m_nameLabel = new Plasma::Label(this);
    m_nameLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    m_nameLabel->setMaximumHeight(KGlobalSettings::generalFont().pointSize()*1.5);
    m_nameLabel->setText(i18nc("default label for result, will usually be overwritten", "Generic Resource"));
    m_rightLayout->addItem(m_nameLabel);

    m_infoLabel = new Plasma::Label(this);
    m_infoLabel->nativeWidget()->setWordWrap(true);
    m_infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_infoLabel->setOpacity(.5);
    m_infoLabel->setFont(KGlobalSettings::smallestReadableFont());
    m_rightLayout->addItem(m_infoLabel);
    m_layout->addItem(m_leftLayout, 0, 0);
    m_layout->addItem(m_rightLayout, 0, 1);

    m_layout->setContentsMargins(4, 4, 4, 4);
    m_leftLayout->setContentsMargins(4, 4, 4, 4);
    m_rightLayout->setContentsMargins(4, 4, 4, 4);

    //setResource(m_resource);
    //kDebug() << "GridLayout has rows, cols:" << m_layout->rowCount() << m_layout->columnCount();
}

ResourceWidget::~ResourceWidget()
{
}

void ResourceWidget::dumpProperties()
{
    kDebug() << "Types:" << m_resource.types() << m_resource.resourceType();
    if (!m_dumpProperties) {
        return;
    }
    foreach (const QUrl &prop, m_resource.properties().keys()) {

        QString v = m_resource.properties()[prop].toString();
        v.truncate(32);
        kDebug() << "\tP:" << prop.toString() << v;
    }
}

ResourceWidget* ResourceWidget::create(Nepomuk::Query::Result result)
{
    /*
    kDebug() << "-----------------------------------------------";
    kDebug() << "Resource:" << resource.uri() << resource.type() << resource.types();
    kDebug() << "Image         Type:" << Nepomuk::Image().uri() << Nepomuk::Image().type();
    kDebug() << "PersonContact Type:" << Nepomuk::PersonContact().uri();
    kDebug() << "email type:" << Nepomuk::Email().type();
    */
    ResourceWidget* rw = 0; 
    Nepomuk::Resource resource = result.resource();
    kDebug() << "==================" << resource.genericLabel() << "=====================";
    if (resource.types().contains(Nepomuk::Email().type())) {
        kDebug() << " MATCH --> This is an Email.";
        rw = new EmailWidget();
    } else if (resource.types().contains(Nepomuk::PersonContact().type())) {
        kDebug() << " MATCH --> This is a PersonContact.";
        rw = new ContactWidget();
    //} else if (resource.types().contains(Nepomuk::RasterImage().type())) {
    } else if (resource.types().contains(Nepomuk::RasterImage().type())
            || resource.types().contains(Nepomuk::Photo().type())) {
        kDebug() << " MATCH --> This is an Image.";
        rw = new ImageResourceWidget();
    } else if (resource.types().contains(Nepomuk::Video().type())) {
        kDebug() << " MATCH --> This is an VIDEO.";
        rw = new VideoResourceWidget();
    } else {
        kDebug() << " MATCH --> Unknown types.";
        rw = new ResourceWidget();
    }
    rw->setResource(result.resource());
    return rw;
    
}

void ResourceWidget::setQuery(const QString &query)
{
    m_query = query;
    m_info = Utils::abstract(m_resource, m_query);
    updateWidgets();
}

void ResourceWidget::setUrl(const QUrl &url)
{
    m_url = url;
    emit urlChanged();
}

void ResourceWidget::open()
{
    kDebug() << "open:" << m_url;
    if (m_url.isEmpty()) {
        kDebug() << "Empty URL";
        return;
    }
    emit run(m_url);
}

void ResourceWidget::setResource(Nepomuk::Resource resource)
{
    m_resource = resource;
    setUrl(QUrl(resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url")).toString()));

#if 0
    kDebug() << "============= Resource ===============";
    foreach(QUrl var, m_resource.properties().keys()) {
        //kDebug() << var << m_resource.properties()[var].variant();
        kDebug() << var;
    }
    kDebug() << "genericLabel:" << resource.genericLabel();
    kDebug() << "genericDescription:" << resource.genericDescription();
    kDebug() << "genericIcon:" << resource.genericIcon();
    kDebug() << "url" << m_url;
    //m_url = QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )

    //QString m_url = m_resource.property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
#endif
    // What to display on the namelabel?
    QString m_label = m_resource.genericLabel();
    if (m_label.isEmpty()) {
        m_label = "Name goes here...";
    }

    // What to display on the infolabel?
    QString _description = m_resource.genericDescription();

    if (m_info.isEmpty()) {
        m_info = _description;
    }
    if (m_info.isEmpty()) {
        m_info = m_url.toString();
    }

    updateWidgets();
}

/*
void ResourceWidget::setFileItem(const KFileItem &item)
{
    m_fileItem = item;

    m_icon = item.iconName();
    //m_mimeType = entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );
    / *
    if (m_icon.isEmpty()) {
        if (!m_mimeType.isEmpty()) {
            m_icon = KMimeType::iconNameForUrl(m_url);
        } else {
            m_icon = "nepomuk";
        }
    }
    * /
    updateWidgets();
}
*/
void ResourceWidget::updateWidgets()
{
    // TODO: this first part shouldn't be done whenever a parent widget updates
    if (m_info.isEmpty()) {
        // dummy, only for testing
        //m_infoLabel->setText(Utils::highlight(Utils::abstract("Here goes the information about this resource, an abstract for example, or tags, or something ... Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something", QString("example")), QString("example")));
    } else {
        //QString _abstract = Utils::highlight(m_info, m_query);
        kDebug() << "Setting Info:" << info();
        m_infoLabel->setText(info());
    }
    //m_nameLabel->setText(m_label);
    m_nameLabel->setText("<strong>" + label() + "</strong>");
    if (m_iconWidget) {
        m_iconWidget->setIcon(m_icon);
    }

    // This part, however, is interesting
    m_ratingWidget->setRating(m_resource.rating());

    setMinimumHeight(qMax(m_leftLayout->minimumHeight(), m_rightLayout->minimumHeight() + 12));

    m_layout->invalidate();

    dumpProperties();
}

void ResourceWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPos = event->pos();
    }
    Plasma::IconWidget::mousePressEvent(event);
}

void ResourceWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_startPos).toPoint().manhattanLength();
        if ((event->pos().x() < m_iconSize + 8) && distance >= QApplication::startDragDistance()) {
            startDrag();
        }
    }
    Plasma::IconWidget::mouseMoveEvent(event);
}

void ResourceWidget::startDrag()
{

    QMimeData* mimeData = new QMimeData();
    kDebug() << "Starting drag!";
    QList<QUrl> urls;
    urls << m_url;
    kDebug() << "url:" << urls;
    mimeData->setUrls(urls);

    // This is a bit random, but we need a QWidget for the constructor
    QDrag* drag = new QDrag(m_nameLabel->nativeWidget());
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap());
    if (drag->start(Qt::LinkAction)) {
        kDebug() << "dragging starting";
    }
}

QPixmap ResourceWidget::pixmap()
{
    return KIcon(m_icon).pixmap(64,64);
}

QString ResourceWidget::label()
{
    return m_resource.genericLabel();
}

QString ResourceWidget::info()
{
    QString ex = m_result.excerpt();
    if (ex.isEmpty()) {
        m_result.resource().property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();
    }
    if (ex.isEmpty()) {
        ex = i18n("Generic Result");
    }
    // FIXME: could be more sensible, but this seems to work for plain text files
    return ex;
    //return m_result.resource().property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();
}

#include "resourcewidget.moc"
