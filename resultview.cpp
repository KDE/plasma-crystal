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
#include <QGraphicsWidget>

//KDE
#include <KDebug>
#include <KFileItem>
#include <KRun>

// Nepomuk
#include <Nepomuk/Resource>

//own
#include "resultview.h"


using namespace Crystal;

ResultView::ResultView(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_abstractSize(200)
{
}

ResultView::~ResultView()
{
}

int ResultView::count()
{
    return m_results.count();
}

void ResultView::addMatch(const KFileItem& item)
{
    Nepomuk::Resource *res = new Nepomuk::Resource(item.nepomukUri());
    //kDebug() << "Result:" << _icon << _name << _mimeType << _nepomukUri;
    m_results << res;
    emit resourceAdded(res, m_query, item);
    //emit matchAdded();
}

void ResultView::setQuery(const QString& query)
{
    m_query = query;
}

void ResultView::clear()
{
    kDebug() << "Clear.";
    qDeleteAll(m_results);
    m_results.clear();
}

void ResultView::updateColors()
{
}

void ResultView::updateView()
{
}

void ResultView::run(const QUrl& uri)
{
    kDebug() << "Opening ... " << uri;
    if (uri.isValid()) {
        new KRun(uri, 0);
    }
}

#include "resultview.moc"
