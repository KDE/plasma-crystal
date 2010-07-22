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
#include <QFile>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QLabel>
#include <QWebFrame>
#include <QWebPage>
#include <QWebSettings>

// KDE
#include <KDirWatch>
#include <KStandardDirs>
#include <KDebug>
#include <KLocale>
#include <KRun>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Tag>

//own
#include "dashboard.h"
#include "stylesheet.h"


using namespace Crystal;

DashBoard::DashBoard(QGraphicsWidget *parent)
    : Plasma::WebView(parent)
{

    QWebSettings::globalSettings()->setAttribute( QWebSettings::PluginsEnabled, false );
    QWebSettings::globalSettings()->setAttribute( QWebSettings::LinksIncludedInFocusChain, true);

    setDragToScroll(true);
    mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

    QWebPage *_page = mainFrame()->page();
    _page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(_page, SIGNAL(linkClicked(const QUrl&)), SLOT(linkClicked(const QUrl&)));

    // The stylesheet
    QString css = QUrl(KStandardDirs::locate("data", "plasma-applet-crystal/dashboard.css")).path();

    m_baseDir = QString("file://").append(KUrl(css).directory());
    kDebug() << "base directory:" << m_baseDir;

    //mainFrame()->setUrl(QUrl(_baseDir));

    // debugging...
    css = "/home/sebas/kdesvn/src/playground-plasma/applets/crystal/dashboard.css";
    //kDebug() << "BaseDir:" << m_baseDir;
    m_css = new StyleSheet(this);
    m_css->setFileName(css);
    connect(m_css, SIGNAL(styleSheetChanged(const QString&)), this, SLOT(update()));

    // The template
    QString m_templateFile = QUrl(KStandardDirs::locate("data", "plasma-applet-crystal/dashboard.html")).path();
    m_templateFile = "/home/sebas/kdesvn/src/playground-plasma/applets/crystal/dashboard.html";

    load(m_templateFile);
    m_templateWatch = new KDirWatch(this);
    m_templateWatch->addFile(m_templateFile);
    connect(m_templateWatch,SIGNAL(dirty(QString)),this,SLOT(load(QString)));
    connect(m_templateWatch,SIGNAL(created(QString)),this,SLOT(load(QString)));
    connect(m_templateWatch,SIGNAL(deleted(QString)),this,SLOT(load(QString)));

}

DashBoard::~DashBoard()
{
}

void DashBoard::update()
{
    QString _html;

    QHash<QString, QString> stringMap;

    stringMap["%style"] = m_css->styleSheet();
    stringMap["%title"] = i18nc("dashboard title", "Crystal Desktop Search");
    stringMap["%introduction"] = i18nc("dashboard introduction", "Type in a word to search for it in your files. For more complex queries, you can prefix your query with \"<strong>?</strong>\" to use the more powerful <a href=\"http://techbase.kde.org/Development/Tutorials/Metadata/Nepomuk/QueryService#Simple_Search_Syntax\" target=\"_new\">Nepomuk search syntax</a>.");
    stringMap["%headerTags"] = i18nc("dashboard header tags", "My Tags");
    stringMap["%headerRecent"] = i18nc("dashboard header recent", "Recent Searches");
    stringMap["%tags"] = tags();
    stringMap["%recent"] = recent();
    stringMap["%footer"] = QString();
    stringMap["%notice"] = notice();
    stringMap["%path"] = m_baseDir;
    stringMap["%refreshDashboard"] = QString("crystal:/refreshDashboard");

    _html = m_template;
    foreach(const QString &k, stringMap.keys()) {
        _html.replace(k, stringMap[k]);
    }
    setHtml(_html);
}

QString DashBoard::notice()
{
    if (!(Nepomuk::ResourceManager::instance()->initialized())) {
        return i18n("Desktop search is not activated. <a href=\"crystal:/opensettings\">Click here</a> to enable it in System Settings.");
    }
    return QString();
}

QString DashBoard::tags()
{
    QString _html;

    QList<Nepomuk::Tag> allTags = Nepomuk::Tag::allTags();
    if (!allTags.count()) {
        return i18n("No tags found.");
    }
    int maxTags = 16;
    int _i = 0;
    foreach (Nepomuk::Tag t, allTags) {

        _i++;

        int high = 10;
        int low = 1;
        int weight = qrand() % ((high + 1) - low) + low;

        _html.append(QString("<li><a  class=\"tag%2\" href=\"nepomuksearch:/?query=hastag:%1\">%1</a></li>\n").arg(t.genericLabel(), QString::number(weight)));
        kDebug() << "Tag!" << t.genericLabel() << weight;
        if (_i >= maxTags) {
            break;
        }
    }
    return _html;
}

void DashBoard::setHistory(const QStringList &history)
{
    m_history = history;
    update();
}

QString DashBoard::recent()
{
    QString _html;
    QStringList _tags;
    _tags << "Benachrichtigungsmechanismus" << "kauth" << "linux" << "hastag:Wallpaper" << "akademy" << "crystal";
    foreach (const QString &_t, m_history) {
        _html.append(QString("<li><a href=\"nepomuksearch:/%1\">%1</a></li>\n").arg(_t));
    }
    return _html;
}

QString DashBoard::header()
{
    return QString();
}


void DashBoard::load(const QString &templatePath)
{
    QFile f(this);

    if (templatePath.isEmpty()) {
        f.setFileName(m_template);
    } else {
        f.setFileName(templatePath);
    }
    kDebug() << "(Re)loading template" << templatePath;
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream t(&f);
        m_template = t.readAll();
        f.close();
        update();
    } else {
        kDebug() << "Template File not loaded, error reading file";
    }
    update();
}

void DashBoard::linkClicked(const QUrl &clickedUrl)
{
    if (KUrl(clickedUrl).protocol() == "nepomuksearch") {
        kDebug() << "Nepomuk URL, we can handle that!" << clickedUrl;
        emit search(clickedUrl);
    } else if (KUrl(clickedUrl).protocol() == "crystal") {
        QString cmd = clickedUrl.toString().remove("crystal:/");
        if (cmd == "refreshDashboard") {
            update();
        } else if (cmd == "opensettings") {
            KRun::runCommand("kcmshell4 nepomuk", 0);
        } else {
            kWarning() << "unknown crystal:/ URL:" << clickedUrl;
        }
    } else {
        kDebug() << "opening external" << clickedUrl;
        new KRun(clickedUrl, 0);
    }
}
#include "dashboard.moc"
