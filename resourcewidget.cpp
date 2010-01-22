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

//KDE
#include <KDebug>
#include <KColorScheme>
#include <KGlobalSettings>
#include <KMimeType>
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
//#include "helpers.cpp"


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

    m_layout = new QGraphicsGridLayout(this);
    setLayout(m_layout);

    m_iconWidget = new Plasma::IconWidget(this);
    m_iconWidget->setIcon("nepomuk");
    m_iconWidget->setPreferredSize(64, 64);
    m_layout->addItem(m_iconWidget,0, 0, 1, 2);

    m_nameLabel = new Plasma::Label(this);
    m_nameLabel->setText("Nepomuk Resource");
    m_layout->addItem(m_nameLabel, 0, 1, 1, 1);

    m_infoLabel = new Plasma::Label(this);
    m_infoLabel->setFont(KGlobalSettings::smallestReadableFont());
    m_infoLabel->setText("Here goes the information about this resource, an abstract for example, or tags, or something");
    m_layout->addItem(m_infoLabel, 1, 1, 2, 1);

    m_ratingWidget = new Plasma::Label(this);
    m_ratingWidget->setFont(KGlobalSettings::smallestReadableFont());
    m_ratingWidget->setText("[rating]");
    m_layout->addItem(m_ratingWidget, 2, 0, 1, 1);

}

ResourceWidget::~ResourceWidget()
{
}


#include "resourcewidget.moc"
