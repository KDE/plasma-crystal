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
#include "resourcewidgets/contactwidget.h"
//#include "imagewidget.h"
#include "../utils.h"


using namespace Crystal;

ContactWidget::ContactWidget(QGraphicsWidget *parent)
    : ResourceWidget(parent)
{
    kDebug() << "Creating contact widget";
    m_icon = "x-office-contact";
    kDebug() << "need update?";

    updateWidgets();
}

ContactWidget::~ContactWidget()
{
}

void ContactWidget::updateWidgets()
{
    ResourceWidget::updateWidgets();
    //m_infoLabel->setText(i18nc("info in the image widget", "Image Size: %1x%2", m_width, m_height));
}

QString ContactWidget::info()
{
    return QString("A Contact");
}

void ContactWidget::setResource(Nepomuk::Resource resource)
{
    m_contact = PersonContact(resource.uri());
    ResourceWidget::setResource(resource);
}

#include "contactwidget.moc"
