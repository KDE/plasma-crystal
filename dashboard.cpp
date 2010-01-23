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

// KDE
#include <KDirWatch>
#include <KStandardDirs>
#include <KDebug>
#include <KLocale>

// Nepomuk
#include <Nepomuk/Resource>

//own
#include "dashboard.h"
#include "stylesheet.h"


using namespace Crystal;

DashBoard::DashBoard(QGraphicsWidget *parent)
    : Plasma::WebView(parent)
{

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
/*
void DashBoard::setUrl(const QStringList &matches)
{
    
}
*/
void DashBoard::setMatches(const QStringList &matches)
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
    stringMap["%notice"] = QString();
    stringMap["%path"] = m_baseDir;
    

    _html = m_template;
    foreach(const QString &k, stringMap.keys()) {
        kDebug() << "replacing" << k;
        _html.replace(k, stringMap[k]);
    }
    kDebug() << "HTML:" << _html;
    kDebug() << "Base:" << mainFrame()->baseUrl();
    setHtml(_html);
}

QString DashBoard::tags()
{
    QString _html;
    QStringList _tags;
    _tags << "KDE" << "Wallpaper" << "Iceland";
    foreach (const QString &_t, _tags) {
        _html.append(QString("<li>%1</li>\n").arg(_t));
    }
    return _html;
}

QString DashBoard::recent()
{
    QString _html;
    QStringList _tags;
    _tags << "kauth" << "linux" << "akademy";
    foreach (const QString &_t, _tags) {
        _html.append(QString("<li>%1</li>\n").arg(_t));
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


#include "dashboard.moc"
