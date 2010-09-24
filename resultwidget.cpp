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

//KDE
#include <KDebug>

//plasma
#include <Plasma/ScrollWidget>
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
//#include <nepomuk/resourcemanager.h>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Result>

//own
#include "resultwidget.h"
#include "resourcewidget.h"
#include "imageresourcewidget.h"

using namespace Crystal;

ResultWidget::ResultWidget(QGraphicsWidget *parent)
    : ResultView(parent),
    m_widget(0),
    m_outerLayout(0),
    m_scrollWidget(0),
    m_layout(0)
{
    m_outerLayout = new QGraphicsLinearLayout(this);
    m_outerLayout->setContentsMargins(0,0,0,0);
    setLayout(m_outerLayout);

    m_scrollWidget = new Plasma::ScrollWidget(this);
    m_scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_outerLayout->addItem(m_scrollWidget);
    buildDialog();
}

void ResultWidget::buildDialog()
{
    disconnect(this, SIGNAL(resourceAdded(Nepomuk::Resource*, const QString&, const KFileItem&)),
            this, SLOT(addWidget(Nepomuk::Resource*, const QString&, const KFileItem&)));

    m_widget = new QGraphicsWidget(m_scrollWidget);
    m_layout = new QGraphicsLinearLayout(m_widget);
    m_layout->setOrientation(Qt::Vertical);
    m_layout->setSpacing(1);
    m_scrollWidget->setWidget(m_widget);

    connect(this, SIGNAL(resourceAdded(Nepomuk::Resource*, const QString&, const KFileItem&)),
            this, SLOT(addWidget(Nepomuk::Resource*, const QString&, const KFileItem&)));

}

void ResultWidget::newEntries(const QList<Nepomuk::Query::Result> &entries)
{
    foreach (Nepomuk::Query::Result res, entries) {
        kDebug() << "Result!!!" << res.resource().genericLabel() << res.resource().type();
        addWidget(new Nepomuk::Resource(res.resource()));
    }
}

void ResultWidget::addWidget(Nepomuk::Resource* resource, const QString &query, const KFileItem &item)
{
    QString _mimeType;
    if (!item.isNull()) {
        _mimeType = item.mimetype();
    }

    ResourceWidget *_widget;
    if (_mimeType.startsWith("image")) {
        //kDebug() << "*** Creating an image widget!" << _mimeType;
        ImageResourceWidget *irw = new ImageResourceWidget(resource, m_widget);
        _widget = qobject_cast<ResourceWidget*>(irw);
    } else {
        //kDebug() << "Creating a generic widget." << _mimeType;
        _widget = new ResourceWidget(resource, m_widget);
    }
    _widget->setQuery(query);
    if (!item.isNull()) {
        _widget->setFileItem(item);
    }
    connect(_widget, SIGNAL(run(const QUrl&)), SLOT(run(const QUrl&)));
    m_layout->addItem(_widget);
    m_widgets << _widget;
}

void ResultWidget::updateView()
{
    m_layout->invalidate();
}

void ResultWidget::clear()
{
    kDebug() << "Clearing ..." << m_widgets.count();
    qDeleteAll(m_widgets);
    m_widgets.clear();
    ResultView::clear();
    //m_widget->setMinimumSize(0, 0);
    delete m_widget;
    m_widget = 0;

    buildDialog();
}

ResultWidget::~ResultWidget()
{
}


#include "resultwidget.moc"
