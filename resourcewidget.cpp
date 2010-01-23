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
#include <QGraphicsProxyWidget>
#include <QLabel>

//KDE
#include <KDebug>
#include <KColorScheme>
#include <KGlobalSettings>
#include <KIO/Job>
#include <KMimeType>
#include <Nepomuk/KRatingWidget>
#include <KRun>

//plasma
#include <Plasma/Frame>
#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

//own
#include "resourcewidget.h"
#include "utils.h"


using namespace Crystal;

ResourceWidget::ResourceWidget(Nepomuk::Resource *resource, QGraphicsWidget *parent)
    : Plasma::Frame(parent),
      m_resource(resource),
      m_layout(0),
      m_iconWidget(0),
      m_nameLabel(0),
      m_infoLabel(0),
      m_ratingWidget(0)
{
    int iconSize = 48;
    
    m_layout = new QGraphicsGridLayout(this);
    QGraphicsLinearLayout *m_leftLayout = new QGraphicsLinearLayout(m_layout);
    m_leftLayout->setOrientation(Qt::Vertical);
    QGraphicsLinearLayout *m_rightLayout = new QGraphicsLinearLayout(m_layout);
    m_rightLayout->setOrientation(Qt::Vertical);
    
    
    m_layout->setColumnFixedWidth(0, iconSize);
    m_layout->setRowStretchFactor(0, 1);
    m_layout->setRowStretchFactor(1, 100);
    m_layout->setRowStretchFactor(2, 1);
    setLayout(m_layout);

    m_iconWidget = new Plasma::IconWidget(this);
    m_iconWidget->setIcon("nepomuk");
    m_iconWidget->setMinimumSize(iconSize, iconSize);
    m_iconWidget->setMaximumSize(iconSize, iconSize);
    m_iconWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //m_layout->addItem(m_iconWidget, 0, 0, 2, 1);
    m_leftLayout->addItem(m_iconWidget);
    /*
    KRatingWidget *kratingWidget = new KRatingWidget(0); // leaks!
    kratingWidget->setMaxRating(5);
    kratingWidget->setRating(3);
    m_ratingWidget = new QGraphicsProxyWidget(this);
    m_ratingWidget->setWidget(kratingWidget);

    */
    m_ratingWidget = new Plasma::Label(this);
    m_ratingWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_ratingWidget->setMaximumHeight(12);
    m_ratingWidget->setMaximumWidth(iconSize);
    m_ratingWidget->setFont(KGlobalSettings::smallestReadableFont());
    m_ratingWidget->setText("[rating]");
    m_ratingWidget->setOpacity(0.5);
    //m_layout->addItem(m_ratingWidget, 2, 0, 1, 1, Qt::AlignTop);
    //m_layout->addItem(m_ratingWidget, 2, 0, Qt::AlignTop);
    m_leftLayout->addItem(m_ratingWidget);


    m_nameLabel = new Plasma::Label(this);
    m_nameLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    m_nameLabel->setMaximumHeight(KGlobalSettings::generalFont().pointSize()*1.5);
    m_nameLabel->setText("Nepomuk Resource");
    //m_layout->addItem(m_nameLabel, 0, 1, 1, 1);
    //m_layout->addItem(m_nameLabel, 0, 1);
    m_rightLayout->addItem(m_nameLabel);
    
    m_infoLabel = new Plasma::Label(this);
    m_infoLabel->nativeWidget()->setWordWrap(true);
    m_infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //m_infoLabel->setMinimumHeight(iconSize);
    //m_infoLabel->setStyleSheet("font { opacity: 0.7; }");
    m_infoLabel->setOpacity(.5);
    m_infoLabel->setFont(KGlobalSettings::smallestReadableFont());
    //m_layout->addItem(m_infoLabel, 1, 1, 3, 1);
    m_rightLayout->addItem(m_infoLabel);
    m_layout->addItem(m_leftLayout, 0, 0);
    m_layout->addItem(m_rightLayout, 0, 1);
    

    setResource(m_resource);
    kDebug() << "GridLayout has rows, cols:" << m_layout->rowCount() << m_layout->columnCount();

}

void ResourceWidget::setQuery(const QString &query)
{
    m_query = query;
    updateWidgets();
}

void ResourceWidget::setResource(Nepomuk::Resource *resource)
{
    m_resource = resource;
    m_url = QUrl(resource->property(QUrl("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url")).toString());

    kDebug() << "============= Resource ===============";
    foreach(QUrl var, m_resource->properties().keys()) {
        //kDebug() << var << m_resource->properties()[var].variant();
        kDebug() << var;
    }
    kDebug() << "genericLabel:" << resource->genericLabel();
    kDebug() << "genericDescription:" << resource->genericDescription();
    kDebug() << "genericIcon:" << resource->genericIcon();
    kDebug() << "url" << m_url;
    //m_url = QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )

    //QString m_url = m_resource->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();

    // What to display on the namelabel?
    QString m_label = m_resource->genericLabel();
    if (m_label.isEmpty()) {
        m_label = "Name goes here...";
    }

    // What to display on the infolabel?
    m_info = Utils::abstract(m_resource, m_query);;
    QString _description = m_resource->genericDescription();

    if (m_info.isEmpty()) {
        m_info = _description;
    }
    if (m_info.isEmpty()) {
        m_info = m_url.toString();
    }

    m_ratingWidget->setText(i18n("Rating: %1", m_resource->rating()));
    updateWidgets();
}


void ResourceWidget::setUDSEntry(const KIO::UDSEntry &entry)
{
    kDebug() << "------------- UDSEntry -----------";
    kDebug() << "UDS_ICON_NAME" << entry.stringValue( KIO::UDSEntry::UDS_ICON_NAME );
    kDebug() << "UDS_MIME_TYPE" << entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );
    kDebug() << "UDS_NAME" << entry.stringValue( KIO::UDSEntry::UDS_NAME );;
    kDebug() << "UDS_LOCAL_PATH" << entry.stringValue( KIO::UDSEntry::UDS_LOCAL_PATH );
    foreach (uint i, entry.listFields()) {
        kDebug() << "Field" << i << entry.stringValue(i);
    }
    m_udsEntry = KIO::UDSEntry(entry);

    m_icon = entry.stringValue( KIO::UDSEntry::UDS_ICON_NAME );
    m_mimeType = entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );
    if (m_icon.isEmpty()) {
        if (!m_mimeType.isEmpty()) {
            m_icon = KMimeType::iconNameForUrl(m_url);
        } else {
            m_icon = "nepomuk";
        }
    }
    updateWidgets();
}

void ResourceWidget::updateWidgets()
{
    if (m_info.isEmpty()) {
        // dummy, only for testing
        m_infoLabel->setText(Utils::highlight(Utils::abstract("Here goes the information about this resource, an abstract for example, or tags, or something ... Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something", QString("example")), QString("example")));
    } else {
        QString _abstract = Utils::highlight(m_info, m_query);
        m_infoLabel->setText(_abstract);
    }
    //m_nameLabel->setText(m_label);
    m_nameLabel->setText("<strong>" + m_resource->genericLabel() + "</strong>");
    m_iconWidget->setIcon(m_icon);
    kDebug() << "==== StripShow:" << Utils::stripTags("<h1>This is my text, <br /> is it stripped?</h1>");
    //m_layout->invalidate();
}

ResourceWidget::~ResourceWidget()
{
}


#include "resourcewidget.moc"
