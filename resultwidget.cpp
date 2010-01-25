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
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QWebFrame>

//KDE
#include <KDebug>
#include <KColorScheme>
#include <KIcon>
#include <KIconLoader>
#include <KIO/Job>
//#include <KIO/UDSEntry>
#include <KMimeType>
#include <KRun>
#include <KStandardDirs>

//plasma
#include <Plasma/Dialog>
#include <Plasma/ScrollWidget>
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
#include <nepomuk/resourcemanager.h>
#include <Nepomuk/Variant>

//own
#include "utils.h"
#include "resultwidget.h"
#include "resourcewidget.h"
#include "imageresourcewidget.h"

using namespace Crystal;

ResultWidget::ResultWidget(QGraphicsWidget *parent)
    : ResultView(parent)
{
    buildDialog();
}

void ResultWidget::buildDialog()
{
    disconnect(this, SIGNAL(resourceAdded(Nepomuk::Resource*, const KIO::UDSEntry&, const QString&)),
            this, SLOT(addWidget(Nepomuk::Resource*, const KIO::UDSEntry&, const QString&)));
    m_scrollWidget = new Plasma::ScrollWidget(this);
    QGraphicsLinearLayout *mainlayout = new QGraphicsLinearLayout(this);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->addItem(m_scrollWidget);
    setLayout(mainlayout);

    m_scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // for testing
    //setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_widget = new QGraphicsWidget(m_scrollWidget);
    //_widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //m_widget->setMinimumSize(240, 50);
    m_layout = new QGraphicsLinearLayout(m_widget);
    m_layout->setOrientation(Qt::Vertical);
    m_layout->setSpacing(1);
    m_scrollWidget->setWidget(m_widget);

    connect(this, SIGNAL(resourceAdded(Nepomuk::Resource*, const KIO::UDSEntry&, const QString&)),
            this, SLOT(addWidget(Nepomuk::Resource*, const KIO::UDSEntry&, const QString&)));

#if 0
    QString q = "Fake";
    KIO::UDSEntry e;
    for (int i = 0; i<2; i++) {
        addWidget(new Nepomuk::Resource(), e, q);
    }
#endif
}

void ResultWidget::addWidget(Nepomuk::Resource* resource, const KIO::UDSEntry &entry, const QString &query)
{
    QString _mimeType = entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );

    ResourceWidget *_widget;
    if (_mimeType.startsWith("image")) {
        kDebug() << "******************** Creating an image!" << _mimeType;
        ImageResourceWidget *irw = new ImageResourceWidget(resource, this);
        _widget = qobject_cast<ResourceWidget*>(irw);
    } else {
        kDebug() << "Creating a generic." << _mimeType;
        _widget = new ResourceWidget(resource, this);
    }
    _widget->setQuery(query);
    _widget->setUDSEntry(entry);
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

    delete m_scrollWidget;
    buildDialog();
}

ResultWidget::~ResultWidget()
{
}


#include "resultwidget.moc"
