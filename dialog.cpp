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
#include "dialog.h"
//#include "helpers.cpp"


using namespace Crystal;

Dialog::Dialog(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_lineEdit(0),
      m_searchButton(0),
      m_statusBar(0),
      m_query(0)
{

    m_iconSizes[0] = 16;
    m_iconSizes[1] = 22;
    m_iconSizes[2] = 32;
    m_iconSizes[3] = 48;
    m_iconSizes[4] = 64;
    m_iconSizes[5] = 128;
    m_iconSizes[6] = 256;
    buildDialog();
}

Dialog::~Dialog()
{
}

void Dialog::updateQuery(const QString query)
{
    m_lineEdit->setText(query);
    //m_lineEdit->selectAll(); FIXME
    m_lineEdit->setFocus();
}

void Dialog::setTimeout(int timeout)
{
    m_timeout = timeout;
}

void Dialog::updateIconSize(int iconsize)
{
    int s = qMax(16, m_iconSizes[iconsize]);
    kDebug() << "FIXME: Setting iconsize" << s;
    //m_resultsView->setIconSize(QSize(s, s));
}

void Dialog::buildDialog()
{
    QGraphicsGridLayout *gridLayout = new QGraphicsGridLayout(this);
    setLayout(gridLayout);

    m_lineEdit = new Plasma::LineEdit(this);
    gridLayout->addItem(m_lineEdit, 0, 0);

    m_searchButton = new Plasma::IconWidget(this);
    m_searchButton->setIcon("nepomuk");
    connect(m_searchButton, SIGNAL(clicked()), SLOT(search()));
    gridLayout->addItem(m_searchButton, 0, 1);

    /* ScrollWidget */

    m_resultsView = new Crystal::ResultWebView(this);
    gridLayout->addItem(m_resultsView, 1, 0, 1, 2);

    m_statusBar = new Plasma::Label(this);
    m_statusBar->setText("status");
    gridLayout->addItem(m_statusBar, 2, 0, 1, 2);

    updateStatus(i18nc("no active search, no results shown", "Idle."));
    setPreferredSize(400, 300);
    //setMaximumSize(400, 500);
}

void Dialog::updateStatus(const QString status)
{
    KColorScheme colorTheme = KColorScheme(QPalette::Active, KColorScheme::View,Plasma::Theme::defaultTheme()->colorScheme());
    QString text = QString("<font color=\"%1\">%2</font>").arg(colorTheme.foreground(KColorScheme::NormalText).color().name()).arg(status);
    m_statusBar->setText(text);
}

void Dialog::search()
{
    if (Nepomuk::ResourceManager::instance()->initialized()) {
        kDebug() << "resource manager inited successfully";
    } else {
        kDebug() << "no resource manager";
    };

    m_query = m_lineEdit->text();
    m_resultsView->setQuery(m_query);
    kDebug() << "Searching for ..." << m_query << " timeout after:" << m_timeout;
    updateStatus(i18nc("status in the plasmoid's popup", "Searching for <b>\"%1\"</b>...", m_query));

    // query syntax is at:
    // http://techbase.kde.org/Development/Tutorials/Metadata/Nepomuk/QueryService
    QString queryUrl = QString("nepomuksearch:/?query=%1").arg(m_query);
    KIO::ListJob* listJob = KIO::listDir(KUrl(queryUrl), KIO::HideProgressInfo);
    connect(listJob, SIGNAL(entries(KIO::Job *, const KIO::UDSEntryList&)), this, SLOT(entries(KIO::Job *, const KIO::UDSEntryList&)));
    // add a timeout in case something goes wrong (no user wants to wait more than N seconds)
    QTimer::singleShot( m_timeout, this, SLOT(searchFinished()) );
    //m_queryServiceClient->query( m_query );
    updateStatus(i18n( "Searching for<b>\"%1\"</b> ...", m_query));
}

void Dialog::entries( KIO::Job *job, const KIO::UDSEntryList &list)
{
    Q_UNUSED( job )
    kDebug() << "entries! :)";
    // should look like this:
    KIO::UDSEntryList::ConstIterator it = list.begin();
    const KIO::UDSEntryList::ConstIterator end = list.end();
    for (; it != end; ++it) {
        const KIO::UDSEntry& entry = *it;
        m_resultsView->addMatch(entry);
    }
    m_resultsView->updateView();
    updateStatus(i18np("Searching for <b>\"%2\"</b>. %1 file found so far...",
                       "Searching for <b>\"%2\"</b>. %1 files found so far...", m_resultsView->count(), m_query));
    emit updateToolTip(m_query, m_resultsView->count());

}

void Dialog::searchFinished()
{
    updateStatus(i18np("Search for <b>\"%2\"</b> finished. %1 matching file found.",
                       "Search for <b>\"%2\"</b> finished. %1 matching files found.", m_resultsView->count(), m_query));
    //m_queryServiceClient->close();
    emit updateToolTip(m_query, m_resultsView->count());
}

void Dialog::run(const QUrl& uri)
{
    kDebug() << "Opening ... " << uri;
    if (uri.isValid()) {
        new KRun(uri, 0);
    }
}

#include "dialog.moc"
