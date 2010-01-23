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
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
#include <nepomuk/resourcemanager.h>
#include <Nepomuk/Variant>

//own
#include "utils.h"
#include "resultwebview.h"

using namespace Crystal;
using namespace Plasma;


ResultWebView::ResultWebView(QGraphicsWidget *parent)
    : ResultView(parent),
      m_baseDir(QString())
{
    m_webView = new Plasma::WebView(this);
    QGraphicsLinearLayout *mainlayout = new QGraphicsLinearLayout(this);
    mainlayout->addItem(m_webView);
    setLayout(mainlayout);

    m_baseDir = QUrl(KStandardDirs::locate("data", "plasma-applet-crystal/user.css")).path();

    kDebug() << "BaseDir:" << m_baseDir;
    m_css = new StyleSheet(this);
    m_css->setFileName(m_baseDir);
    connect(m_css, SIGNAL(styleSheetChanged(const QString&)), this, SLOT(updateView()));

    m_webView->setDragToScroll(true);
    m_webView->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_webView->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_webView->setMinimumSize(160, 200);
    m_webView->setHtml(QString("%1<h1>Crystal Desktop Search:</h1>This is the startpage and should get bookmarked queries and the past ones.").arg(htmlHeader()));

    //connect(Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
    //updateColors();
    //setPreferredSize(400, 300);
}

ResultWebView::~ResultWebView()
{
}
void ResultWebView::updateColors()
{
    QString js = "document.getElementById('cssfile').href = 'user.css'";
    m_webView->mainFrame()->evaluateJavaScript(js);
}

void ResultWebView::updateView()
{
    QString _html = htmlHeader();
    qSort(m_results.begin(), m_results.end(), Utils::ratingLessThan);
    foreach(Nepomuk::Resource* res, m_results) {
        _html.append(renderItem(res));
        _html.append("\n\n");
    }
    m_webView->setHtml(_html);
}

QString ResultWebView::renderItem(Nepomuk::Resource *res)
{
    QString _description = res->genericDescription();
    QString _label = res->genericLabel();
    QString _url = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
    QString _abstract = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();

    foreach(QUrl var, res->properties().keys()) {
        kDebug() << var << res->properties()[var].variant();
    }
    QString html = QString("\n<li><div class=\"result\"><div class=\"link\"><a href=\"%1\">%2</a></div><br /><div class=\"description\">%3</div><div class=\"abstract\">%4</div></div></li>").arg(_url, _label, _description, Utils::abstract(res, m_query));
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
