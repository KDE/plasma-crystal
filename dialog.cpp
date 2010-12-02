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
#include <nepomuk/resource.h>
//#include <Nepomuk/Resource>
#include <nepomuk/resourcemanager.h>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTerm>
#include <soprano/queryresultiterator.h>
#include <Nepomuk/Tag>
#include <soprano/model.h>
#include <soprano/vocabulary.h>

#include <nepomuk/tag.h>
#include <nepomuk/andterm.h>
#include <nepomuk/orterm.h>
#include <nepomuk/comparisonterm.h>
#include <nepomuk/literalterm.h>
#include <nepomuk/resourcetypeterm.h>

//own
#include "dialog.h"


using namespace Crystal;

Dialog::Dialog(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_lister(0),
      m_navIcon(0),
      m_lineEdit(0),
      m_searchButton(0),
      m_statusBar(0),
      m_progress(0),
      m_historySize(4)
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
    connect(m_lister, SIGNAL(percent(int)), this, SLOT(progressChanged(int)));
    //connect(m_lister, SIGNAL(itemsAdded(const KUrl&, const KFileItemList&)),
    //            this, SLOT(entries(const KUrl&, const KFileItemList&)));

    m_queryClient = new Nepomuk::Query::QueryServiceClient(this);
    m_fileQueryClient = new Nepomuk::Query::QueryServiceClient(this);

    buildDialog();
}

Dialog::~Dialog()
{
    m_queryClient->close();
    m_fileQueryClient->close();

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
    m_tabBar->addTab(KIcon("nepomuk"), i18n("Dashboard"), m_dashBoard);

    //m_resultsView = new Crystal::ResultWebView(this);
    m_resultsView = new Crystal::ResultWidget(m_tabBar);

    m_tabBar->addTab(KIcon("system-search"), i18n("Results"), m_resultsView);
    m_tabBar->setTabBarShown(false);

    gridLayout->addItem(m_tabBar, 1, 0, 1, 3);

    m_statusBar = new Plasma::Label(this);
    //m_statusBar->setText("status");
    m_statusBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_statusBar->setMaximumHeight(22);
    m_statusBar->setFont(KGlobalSettings::smallestReadableFont());
    gridLayout->addItem(m_statusBar, 2, 0, 1, 3);

    connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(search()));
    connect(m_lineEdit, SIGNAL(returnPressed()), m_searchButton, SLOT(setPressed()));
    connect(m_searchButton, SIGNAL(clicked()), SLOT(search()));
    connect(m_dashBoard, SIGNAL(search(const QString&)), SLOT(search(const QString&)));
    connect(m_tabBar, SIGNAL(currentChanged(int)), SLOT(updateNavIcon(int)));
    connect(m_navIcon, SIGNAL(clicked()), SLOT(toggleTab()));

    // connect query clients
    connect(m_queryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
            m_resultsView, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
    connect(m_queryClient, SIGNAL(finishedListing()),
            this, SLOT(searchFinished()));

    connect(m_fileQueryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
            m_resultsView, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
    connect(m_fileQueryClient, SIGNAL(finishedListing()),
            this, SLOT(searchFinished()));

    connect(m_resultsView, SIGNAL(matchFound()), this, SLOT(matchFound()));
    kDebug() << "CONNECTED!!!!!!!!";

    updateStatus(i18nc("no active search, no results shown", "Idle."));
    updateNavIcon(m_tabBar->currentIndex());
    setPreferredSize(400, 400);
}

void Dialog::updateStatus(const QString status)
{
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

    if (!(m_history.contains(m_query))) {
        m_history << m_query;
        kDebug() << "Inserting query into history" << m_query << m_history;
        if (m_historySize < m_history.size()) {
            m_history.removeFirst();
            kDebug() << "removed first entry";
        }
        m_dashBoard->setHistory(m_history);
        emit historyChanged(m_history);
    } else {
        kDebug() << "query already in history" << m_query << m_history;
    }

    // query syntax is at:
    // http://techbase.kde.org/Development/Tutorials/Metadata/Nepomuk/QueryService
    //search(QUrl(QString("nepomuksearch:/?query=%1").arg(m_query)));
    //search(QUrl(QString("filenamesearch:/home/sebas?search=%1&checkContent=yes").arg(m_query));
    //search(m_query);
    search(m_query);
}

void Dialog::setHistory(QStringList history)
{
    m_history = history;
    m_dashBoard->setHistory(m_history);
}

QStringList Dialog::history()
{
    return m_history;
}

void Dialog::search(const QString queryString)
{
    m_query = queryString;
    m_queryClient->close();
    m_fileQueryClient->close();
    m_lineEdit->setText(queryString);
    m_resultsView->clear();
    // searches emailAddress
    //QString name = "Marco Martin";
    kDebug() << queryString;
    //bool found = false;
    QUrl graphUri = QUrl();
    Nepomuk::Query::Query query;

    //Nepomuk::Query::OrTerm orTerm;
    //Nepomuk::Query::ResourceTypeTerm personTypeTerm(Vocabulary::NCO::PersonContact());
    //orTerm.addSubTerm(personTypeTerm);
/*
    Nepomuk::Query::AndTerm andTerm;
    Nepomuk::Query::ResourceTypeTerm personTypeTerm( Vocabulary::NCO::PersonContact() );
    andTerm.addSubTerm( personTypeTerm );

    const Nepomuk::Query::ComparisonTerm addrTerm( Vocabulary::NCO::emailAddress(),
            Nepomuk::Query::LiteralTerm( queryString ), Nepomuk::Query::ComparisonTerm::Contains );
    const Nepomuk::Query::ComparisonTerm mailTerm( Vocabulary::NCO::hasEmailAddress(), addrTerm );

    andTerm.addSubTerm( mailTerm );
    */
    kDebug() << "tagsearch?" << queryString << queryString.startsWith("hastag:", Qt::CaseInsensitive);
    Nepomuk::Query::FileQuery fileQuery;
    if (queryString.startsWith("hastag:", Qt::CaseInsensitive)) {
        QString tagName = queryString;
        tagName.remove("hastag:", Qt::CaseSensitive);
        kDebug() << "Searching tag" << tagName;
        //Nepomuk::Tag myTag = Nepomuk::Tag(tagName);
        QList<Nepomuk::Tag> allTags = Nepomuk::Tag::allTags();
        if (!allTags.count()) {
            return;
        }
        foreach (const Nepomuk::Tag &t, allTags) {

            if (QString::compare(tagName, t.genericLabel(), Qt::CaseInsensitive) == 0) {
                // term matching the tag
                kDebug() << "Found tag:" << t.genericLabel() << t.uri();
                Nepomuk::Query::ResourceTerm tagTerm( t );

                // term matching tagged resource
                Nepomuk::Query::ComparisonTerm term( Soprano::Vocabulary::NAO::hasTag(),
                                                    tagTerm,
                                                    Nepomuk::Query::ComparisonTerm::Equal );
                fileQuery.setTerm(term);
                query.setTerm(term);
                continue;
            }
        }

    } else {
        Nepomuk::Query::LiteralTerm nepomukTerm(queryString);
        query.setTerm( nepomukTerm );
        query.setLimit( 20 );


        // File search
        kDebug() << "file search for query:" << m_query;
        //Nepomuk::Query::LiteralTerm nepomukTerm(queryString);
        fileQuery.setTerm( nepomukTerm );

    }
    m_time.restart();

//     kDebug() << "SPARQL:" << fileQuery.toSparqlQuery();
    kDebug() << "SPARQL:" << query.toSparqlQuery();
    m_queryClient->query(query);
    fileQuery.addIncludeFolder(KUrl("/home/sebas/kde/articles/"), true);
    fileQuery.setLimit( 20 );
    m_fileQueryClient->query(fileQuery);
    //m_queryClient->sparqlQuery(query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql );


    /*
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                        query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql );
    kDebug() << query.toSparqlQuery();

    if ( it.next() ) {
        found = true;
        const QUrl uri = it.binding( 0 ).uri();
        it.close();
        kDebug() << "Yay, found someone ... " << uri;
        //return NepomukFast::PersonContact( uri, graphUri );
    } else {
        // create a new contact
        kDebug() << "Did not find " << queryString<< ", creating a new PersonContact";
        found = false;
    }
    */
}

/*
void Dialog::search(const QString &queryString)
{
    //m_query = nepomukUrl.toString().remove("nepomuksearch:/?query=");
    m_query = queryString;
    m_resultsView->setQuery(m_query);
    m_lineEdit->setText(m_query);
    kDebug() << "searching for ..." << queryString << m_query;

    m_resultsView->clear();
    m_time.restart();

    QUrl nepomukUrl = QUrl(QString("filenamesearch:/home/sebas/kde/articles?search=%1&checkContent=yes").arg(m_query));
    m_lister->openUrl(nepomukUrl);

    updateStatus(i18nc("status in the plasmoid's popup", "Searching for <i>\"%1\"</i>...", m_query));
}

void Dialog::entries(const KUrl &url, const KFileItemList &list)
{
    Q_UNUSED( url )
    if (!list.count()) {
        return;
    }
    kDebug() << "entries! :)";

    KFileItemList::ConstIterator it = list.begin();
    const KFileItemList::ConstIterator end = list.end();
    for (; it != end; ++it) {
        const KFileItem& item = *it;
        m_resultsView->addMatch(item);
        QEventLoop loop;
        loop.processEvents();
        loop.quit();
    }
    updateStatus(i18np("Found %1 result for \"<i>%2</i>\" in %3.",
                       "Found %1 results for \"<i>%2</i>\" in %3.",
                       m_resultsView->count(),
                       m_query,
                       KGlobal::locale()->formatDuration(m_time.elapsed())));
    m_tabBar->setCurrentIndex(1);

    m_resultsView->updateView();
    emit updateToolTip(m_query, m_resultsView->count());

}
*/

void Dialog::matchFound()
{
    updateStatus(i18np("Found %1 result for \"<i>%2</i>\" in %3.",
                       "Found %1 results for \"<i>%2</i>\" in %3.",
                       m_resultsView->count(),
                       m_query,
                       KGlobal::locale()->formatDuration(m_time.elapsed())));
    m_tabBar->setCurrentIndex(1);

    m_resultsView->updateView();
    emit updateToolTip(m_query, m_resultsView->count());
}

void Dialog::progressChanged(int percent)
{
    m_progress = percent;
    kDebug() << "!!! Progress now at:" << m_progress;
}

void Dialog::searchFinished()
{
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
