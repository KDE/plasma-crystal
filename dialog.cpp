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
#include <KLineEdit>
#include <KLocale>
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
      m_lister(0),
      m_navIcon(0),
      m_lineEdit(0),
      m_searchButton(0),
      m_statusBar(0),
      m_query(0),
      m_progress(0)
{
    m_time.start();
    m_timeout = 20000; // 20 seconds should be enough for everyone!
    m_iconSizes[0] = 16;
    m_iconSizes[1] = 22;
    m_iconSizes[2] = 32;
    m_iconSizes[3] = 48;
    m_iconSizes[4] = 64;
    m_iconSizes[5] = 128;
    m_iconSizes[6] = 256;

    m_lister = new KDirLister(this);
    connect(m_lister, SIGNAL(completed()), this, SLOT(searchFinished()));


    buildDialog();
}

Dialog::~Dialog()
{
}

void Dialog::updateQuery(const QString query)
{
    m_lineEdit->setText(query);
    m_lineEdit->nativeWidget()->selectAll(); // FIXME
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

    m_navIcon = new Plasma::IconWidget(this);
    m_navIcon->setIcon("go-next");
    m_navIcon->setMaximumSize(22, 22);
    m_navIcon->setMinimumSize(22, 22);
    gridLayout->addItem(m_navIcon, 0, 0);

    m_lineEdit = new Plasma::LineEdit(this);
    gridLayout->addItem(m_lineEdit, 0, 1);

    m_searchButton = new Plasma::IconWidget(this);
    m_searchButton->setIcon("system-search");
    m_searchButton->setMaximumSize(22, 22);
    m_searchButton->setMinimumSize(22, 22);
    gridLayout->addItem(m_searchButton, 0, 2);

    m_tabBar = new Plasma::TabBar(this);

    m_dashBoard = new DashBoard(m_tabBar);
    //m_dashBoard->setHtml(QString("<br /><br /><br /><center><i>This is your dashboard</i></center>"));
    m_tabBar->addTab(KIcon("nepomuk"), i18n("Dashboard"), m_dashBoard);

    //m_resultsView = new Crystal::ResultWebView(this);
    m_resultsView = new Crystal::ResultWidget(m_tabBar);

    m_tabBar->addTab(KIcon("system-search"), i18n("Results"), m_resultsView);
    m_tabBar->setTabBarShown(false);
    
    gridLayout->addItem(m_tabBar, 1, 0, 1, 3);

    m_statusBar = new Plasma::Label(this);
    m_statusBar->setText("status");
    m_statusBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_statusBar->setMaximumHeight(22);
    m_statusBar->setFont(KGlobalSettings::smallestReadableFont());
    gridLayout->addItem(m_statusBar, 2, 0, 1, 3);

    connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(search()));
    connect(m_lineEdit, SIGNAL(returnPressed()), m_searchButton, SLOT(setPressed()));
    connect(m_searchButton, SIGNAL(clicked()), SLOT(search()));
    connect(m_dashBoard, SIGNAL(search(const QUrl&)), SLOT(search(const QUrl&)));
    connect(m_tabBar, SIGNAL(currentChanged(int)), SLOT(updateNavIcon(int)));
    connect(m_navIcon, SIGNAL(clicked()), SLOT(toggleTab()));

    updateStatus(i18nc("no active search, no results shown", "Idle."));
    updateNavIcon(m_tabBar->currentIndex());
    setPreferredSize(400, 400);
    //setMaximumSize(400, 500);
}

void Dialog::updateStatus(const QString status)
{
    //KColorScheme colorTheme = KColorScheme(QPalette::Active, KColorScheme::View,Plasma::Theme::defaultTheme()->colorScheme());
    //QString text = QString("<font color=\"%1\">%2</font>").arg(colorTheme.foreground(KColorScheme::NormalText).color().name(), status);
    m_statusBar->setText(status);
}

void Dialog::search()
{
    if (Nepomuk::ResourceManager::instance()->initialized()) {
        kDebug() << "resource manager inited successfully";
    } else {
        kDebug() << "no resource manager";
    };
    m_query = m_lineEdit->text();
    if (m_query.isEmpty()) {
        m_tabBar->setCurrentIndex(0);
        return;
    }
    kDebug() << "Searching for ..." << m_query << " timeout after:" << m_timeout;

    // query syntax is at:
    // http://techbase.kde.org/Development/Tutorials/Metadata/Nepomuk/QueryService
    search(QUrl(QString("nepomuksearch:/%1").arg(m_query)));
}

void Dialog::search(const QUrl &nepomukUrl)
{
    m_query = nepomukUrl.toString().remove("nepomuksearch:/");
    m_resultsView->setQuery(m_query);
    m_lineEdit->setText(m_query);
    kDebug() << "searching for ..." << nepomukUrl;

    m_resultsView->clear();
    m_time.restart();

    m_lister->openUrl(nepomukUrl);
    /*
    KIO::ListJob* listJob = KIO::listDir(KUrl(nepomukUrl), KIO::HideProgressInfo);
    connect(listJob, SIGNAL(entries(KIO::Job *, const KIO::UDSEntryList&)), this, SLOT(entries(KIO::Job *, const KIO::UDSEntryList&)));
    connect(listJob, SIGNAL(finished(KJob*)), this, SLOT(searchFinished()));
    connect(listJob, SIGNAL(percent(KJob *, unsigned long)), this, SLOT(progressChanged(KJob*, unsigned long)));
    */


    // add a timeout in case something goes wrong (no user wants to wait more than N seconds)
    QTimer::singleShot( m_timeout, this, SLOT(searchFinished()) );
    //m_queryServiceClient->query( m_query );
    updateStatus(i18nc("status in the plasmoid's popup", "Searching for <i>\"%1\"</i>...", m_query));
    m_tabBar->setCurrentIndex(1);
}

void Dialog::entries(const KFileItemList &list)
{
    kDebug() << "entries! :)";
    // should look like this:
    KFileItemList::ConstIterator it = list.begin();
    const KFileItemList::ConstIterator end = list.end();
    for (; it != end; ++it) {
        const KFileItem& item = *it;
        m_resultsView->addMatch(item);
    }
    updateStatus(i18np("Searching for <i>\"%2\"</i>. %1 file found so far...",
        "Searching for <i>\"%2\"</i>. %1 files found so far...", m_resultsView->count(), m_query));
    //m_progress = (qreal)(job->percent());
    m_resultsView->updateView();
    emit updateToolTip(m_query, m_resultsView->count());

}

void Dialog::progressChanged(KJob *job, unsigned long percent)
{
    Q_UNUSED( job )
    m_progress = (qreal)(percent);
    kDebug() << "!!!!!!!!!!!!!!!!!!!! Progress now at:" << m_progress;
}

void Dialog::searchFinished()
{
    if (!m_resultsView->count()) {
        entries(m_lister->items());
    }
    updateStatus(i18np("Found %2 result in %1.",
                       "Found %2 results in %1.",
                       KGlobal::locale()->formatDuration(m_time.elapsed()),
                       m_resultsView->count()));
    emit updateToolTip(m_query, m_resultsView->count());
}

void Dialog::updateNavIcon(int tabIndex)
{
    if (tabIndex == 0) {
        m_navIcon->setEnabled((m_resultsView->count() > 0));
        m_navIcon->setIcon("go-next");
    } else {
        m_navIcon->setEnabled(true);
        m_navIcon->setIcon("go-previous");
    }
}

void Dialog::toggleTab()
{
    if (m_tabBar->currentIndex() == 0) {
        m_tabBar->setCurrentIndex(1);
    } else {
        m_tabBar->setCurrentIndex(0);
    }
}

#include "dialog.moc"
