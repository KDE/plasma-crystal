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

using namespace Crystal;

ResultWidget::ResultWidget(QGraphicsWidget *parent)
    : ResultView(parent)
{
    m_scrollWidget = new Plasma::ScrollWidget(this);
    QGraphicsLinearLayout *mainlayout = new QGraphicsLinearLayout(this);
    mainlayout->addItem(m_scrollWidget);
    setLayout(mainlayout);

    m_scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // for testing
    //setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGraphicsWidget *_widget = new QGraphicsWidget(this);
    //m_widget->setMinimumSize(240, 50);
    m_layout = new QGraphicsLinearLayout(_widget);
    m_layout->setOrientation(Qt::Vertical);
    m_layout->setSpacing(1);
    m_scrollWidget->setWidget(_widget);

    for (int i = 0; i<2; i++) {
        addWidget(new Nepomuk::Resource());
    }

    connect(this, SIGNAL(resourceAdded(Nepomuk::Resource*)), this, SLOT(addWidget(Nepomuk::Resource*)));
    kDebug() << "ctor ran.";
}

void ResultWidget::addWidget(Nepomuk::Resource* resource)
{
    ResourceWidget *_widget = new ResourceWidget(resource, this);
    m_layout->addItem(_widget);
    m_widgets << _widget;
}

ResultWidget::~ResultWidget()
{
}


#include "resultwidget.moc"
