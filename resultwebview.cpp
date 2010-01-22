/*
    Copyright 2008-2010 by Sebastian Kügler <sebas@kde.org>

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
#include "resultwebview.h"

using namespace Crystal;
using namespace Plasma;


ResultWebView::ResultWebView(QGraphicsWidget *parent)
    : Plasma::WebView(parent),
      m_query(0),
      m_abstractSize(200),
      m_baseDir(QString())
{

    m_baseDir = QUrl(KStandardDirs::locate("data", "plasma-applet-crystal/user.css")).path();

    kDebug() << "BaseDir:" << m_baseDir;
    m_css = new StyleSheet(this);
    m_css->setFileName(m_baseDir);
    connect(m_css, SIGNAL(styleSheetChanged(const QString&)), this, SLOT(updateView()));

    setDragToScroll(true);
    mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    setMinimumSize(160, 200);
    setHtml(QString("%1<h1>Crystal Desktop Search:</h1>This is the startpage and should get bookmarked queries and the past ones.").arg(htmlHeader()));

    //connect(Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
    //updateColors();
    //setPreferredSize(400, 300);
}

ResultWebView::~ResultWebView()
{
}

int ResultWebView::count()
{
    return m_results.count();
}



void ResultWebView::addMatch(const KIO::UDSEntry& entry)
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
}

void ResultWebView::setQuery(const QString& query)
{
    m_query = query;
}

void ResultWebView::updateColors()
{
    QString js = "document.getElementById('cssfile').href = 'user.css'";
    mainFrame()->evaluateJavaScript(js);
}

void ResultWebView::updateView()
{
    QString _html = htmlHeader();
    //qSort(m_results.begin(), m_results.end(), Crystal::ratingLessThan);
    foreach(Nepomuk::Resource* res, m_results) {
        _html.append(renderItem(res));
        _html.append("\n\n");
    }
    setHtml(_html);
}

QString ResultWebView::abstract(Nepomuk::Resource *res)
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


QString ResultWebView::renderItem(Nepomuk::Resource *res)
{
    /*
    QString _name = entry.stringValue( KIO::UDSEntry::UDS_NAME );
    //bool isDir = entry.isDir();
    //KIO::filesize_t size = entry.numberValue( KIO::UDSEntry::UDS_SIZE, -1 );
    QString _mimeType = entry.stringValue( KIO::UDSEntry::UDS_MIME_TYPE );
    QString _icon = entry.stringValue( KIO::UDSEntry::UDS_ICON_NAME );
    QString _nepomukUri = entry.stringValue( KIO::UDSEntry::UDS_NEPOMUK_URI );
    //QString _fileSize =
    */
    QString _description = res->genericDescription();
    QString _label = res->genericLabel();
    QString _url = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
    QString _abstract = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();

    foreach(QUrl var, res->properties().keys()) {
        kDebug() << var << res->properties()[var].variant();
    }
    QString html = QString("\n<li><div class=\"result\"><div class=\"link\"><a href=\"%1\">%2</a></div><br /><div class=\"description\">%3</div><div class=\"abstract\">%4</div></div></li>").arg(_url, _label, _description, abstract(res));
    kDebug() << html;
    return html;
}

QString ResultWebView::htmlHeader()
{
    QString _html = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\
<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">\
<head>\
  <title></title>\
  <style type=\"text/css\">\
  %1 \
</style>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> \
<meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\
</head>\
<body>").arg(m_css->styleSheet());
    _html = QString("<style>%1</style>\n\n<ul>").arg(m_css->styleSheet());
    kDebug() << "CSS:" << _html;
    return _html;
}

#include "resultwebview.moc"
