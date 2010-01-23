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
    m_infoLabel->setFont(KGlobalSettings::smallestReadableFont());
    //m_layout->addItem(m_infoLabel, 1, 1, 3, 1);
    m_rightLayout->addItem(m_infoLabel);
    m_layout->addItem(m_leftLayout, 0, 0);
    m_layout->addItem(m_rightLayout, 0, 1);
    
    updateWidgets();

    kDebug() << "GridLayout has rows, cols:" << m_layout->rowCount() << m_layout->columnCount();
    kDebug() << "============= Resource ===============";
    foreach(QUrl var, m_resource->properties().keys()) {
        //kDebug() << var << m_resource->properties()[var].variant();
        kDebug() << var;
    }

}

void ResourceWidget::setQuery(const QString &query)
{
    m_query = query;
    updateWidgets();
}

void ResourceWidget::setUDSEntry(const KIO::UDSEntry &entry)
{
    m_udsEntry = KIO::UDSEntry(entry);
    updateWidgets();
}

void ResourceWidget::updateWidgets()
{
    QString _description = m_resource->genericDescription();
    QString _label = m_resource->genericLabel();
    //QString _icon = 
    QString m_url = m_resource->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
    QString _abstract = m_resource->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();
    //m_iconWidget->setIcon(KMimeType());


    if (_label.isEmpty()) {
        m_nameLabel->setText("Name goes here...");
    } else {
        m_nameLabel->setText(_label);

    }
    if (_abstract.isEmpty()) {
        // dummy, only for testing
        m_infoLabel->setText(Utils::highlight(Utils::abstract("Here goes the information about this resource, an abstract for example, or tags, or something ... Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something Here goes the information about this resource, an abstract for example, or tags, or something", QString("example")), QString("example")));
    } else {
        _abstract = Utils::highlight(Utils::abstract(m_resource, m_query), m_query);
        m_infoLabel->setText(_abstract);
    }
    kDebug() << "==== StripShow:" << Utils::stripTags("<h1>This is my text, <br /> is it stripped?</h1>");
    //m_layout->invalidate();
}

ResourceWidget::~ResourceWidget()
{
}


#include "resourcewidget.moc"
