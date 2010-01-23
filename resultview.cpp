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
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
#include <nepomuk/resourcemanager.h>
#include <Nepomuk/Variant>

//own
#include "helpers.cpp"
#include "resultview.h"

using namespace Crystal;
using namespace Plasma;


ResultView::ResultView(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_query(0),
      m_abstractSize(200)
{
    // default constructor
    kDebug() << "Defaulf CTor, shouldn't be called!";
}

ResultView::~ResultView()
{
}

int ResultView::count()
{
    return m_results.count();
}



void ResultView::addMatch(const KIO::UDSEntry& entry)
{
    QString _nepomukUri = entry.stringValue( KIO::UDSEntry::UDS_NEPOMUK_URI );
    Nepomuk::Resource *res = new Nepomuk::Resource(_nepomukUri);
    /*
    QString _name = entry.stringValue( KIO::UDSEntry::UDS_NAME );
    //bool isDir = entry.isDir();
    //KIO::filesize_t size = entry.numberValue( KIO::UDSEntry::UDS_SIZE, -1 );
    QString _mimeType = entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );
    QString _icon = entry.stringValue( KIO::UDSEntry::UDS_ICON_NAME );
    QString _nepomukUri = entry.stringValue( KIO::UDSEntry::UDS_NEPOMUK_URI );
    m_resultsView->setHtml(QString("%1<div>name: %2<br />info: %3</div>").arg(m_resultsView->html(), _name, _mimeType));
    */
    //kDebug() << "Result:" << _icon << _name << _mimeType << _nepomukUri;
    m_results << res;
    emit resourceAdded(res);
}

void ResultView::setQuery(const QString& query)
{
    m_query = query;
}

void ResultView::updateColors()
{
}

void ResultView::updateView()
{
    kDebug() << "Should not be called in the base class!";
}

QString ResultView::abstract(Nepomuk::Resource *res)
{
    QString fulltext = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();

    kDebug() << "mangling" << res->genericLabel() << ", query:" << m_query;
    if (fulltext.isEmpty()) {
        return QString();
    }
    QString html = "not found";
    int _i = fulltext.indexOf(m_query, Qt::CaseInsensitive);
    if (_i >= 0) {
        int _b = qMin(0, _i - (int)(m_abstractSize/2));
        int _l = qMin(m_abstractSize, fulltext.count());
        html = fulltext.midRef(_b, _l).toString();
        kDebug() << "Found query at " << _i << _b << _l;
        kDebug() << "Abstract:" << html;
        html.replace(m_query, QString("<font color=\"red\"><strong>%1</strong</font>").arg(m_query), Qt::CaseInsensitive);
    }
    return html;
}

#include "resultview.moc"
