/*
    Copyright 2008-2009 by Sebastian Kügler <sebas@kde.org>

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
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

//KDE
#include <KDebug>
#include <KColorScheme>
#include <KIcon>
#include <KIconLoader>
#include <KMimeType>
#include <KRun>

//plasma
#include <Plasma/Dialog>
#include <Plasma/Theme>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Variant>
#include <Soprano/Vocabulary/Xesam>
#include <nepomuk/queryserviceclient.h>
//#include <nepomuk/result.h>
#include <nepomuk/resource.h>
#include <nepomuk/resourcemanager.h>
#include <nepomuk/searchhitview.h>

//own
#include "crystaldialog.h"
#include "crystal.h"

using namespace Crystal;
using namespace Plasma;

Q_DECLARE_METATYPE( Nepomuk::Search::Result )

CrystalDialog::CrystalDialog(CrystalApplet * crystal,QObject *parent)
    : QObject(parent),
      m_widget(0),
      m_resultsView(0),
      m_lineEdit(0),
      m_crystal(crystal),
      m_matches(0),
      m_query(0)
{

    m_iconSizes[0] = 16;
    m_iconSizes[1] = 22;
    m_iconSizes[2] = 32;
    m_iconSizes[3] = 48;
    m_iconSizes[4] = 64;
    m_iconSizes[5] = 128;
    m_iconSizes[6] = 256;

    // initialize the query client
    m_queryServiceClient = new Nepomuk::Search::QueryServiceClient(this);
    buildDialog();

    // Connect for applet-internal handling of new matches
    // You can also pass the matches directly to the searchview, do not connect here,
    // but connect the queryclient to the searchview directly in buildDialog() (for debugging)
    connect( m_queryServiceClient, SIGNAL(newEntries( const QList<Nepomuk::Search::Result>& )),
             this, SLOT(newMatches( const QList<Nepomuk::Search::Result>& )) );

    connect( m_queryServiceClient, SIGNAL(finishedListing()),
             this, SLOT(searchFinished()));

    // clicking on items
    connect( m_resultsView, SIGNAL( resourceActivated(const QUrl&)),
             this, SLOT(run(const QUrl&)));
}

CrystalDialog::~CrystalDialog()
{
}

QWidget * CrystalDialog::dialog()
{
    return m_widget;
}

void CrystalDialog::updateQuery(const QString query)
{
    m_lineEdit->setText(query);
    m_lineEdit->selectAll();
    m_lineEdit->setFocus();
}

void CrystalDialog::updateIconSize()
{
    int s = qMax(16, m_iconSizes[m_crystal->iconSize()]);
    kDebug() << "Setting iconsize" << s;
    m_resultsView->setIconSize(QSize(s, s));
}

void CrystalDialog::buildDialog()
{
    m_widget = new QWidget();

    QVBoxLayout *l_layout = new QVBoxLayout(m_widget);
    l_layout->setSpacing(0);
    l_layout->setMargin(0);

    m_lineEdit = new KLineEdit(m_widget);
    m_lineEdit->setClearButtonShown(true);
    m_lineEdit->setText(m_crystal->defaultQuery());
    m_lineEdit->setFocus();

    connect(m_lineEdit, SIGNAL(returnPressed()),
            this, SLOT(search()));

    m_searchButton = new KPushButton(KIcon("nepomuk"), "", m_widget);
    connect(m_searchButton, SIGNAL(clicked()),
            this, SLOT(search()));

    QHBoxLayout *l_layout2 = new QHBoxLayout(m_widget);
    l_layout2->setSpacing(0);
    l_layout2->setMargin(0);

    l_layout2->addWidget(m_lineEdit);
    l_layout2->addWidget(m_searchButton);

    l_layout2->setAlignment(Qt::AlignCenter);
    l_layout->addLayout(l_layout2);

    m_resultsView = new Nepomuk::SearchHitView(m_widget);
    m_resultsView->setMinimumSize(QSize(300, 300));
    updateIconSize();
    m_resultsView->setTextElideMode(Qt::ElideMiddle); // From QAbstractItemView
    m_resultsView->setBatchSize(4); // Does this make the results show up early?
    m_resultsView->setViewMode( QListView::ListMode );
    //m_resultsView->setViewMode( QListView::IconMode );

    // Connect this to pass the matches directly to the view, for debugging purposes.
    //connect( m_queryServiceClient, SIGNAL(newEntries( const QList<Nepomuk::Search::Result>& )),
    //         m_resultsView, SLOT(updateResources( const QList<Nepomuk::Search::Result>& )) );

    l_layout->addWidget(m_resultsView);

    m_statusLabel = new QLabel(m_widget);
    m_statusLabel->setWordWrap(true);
    l_layout->addWidget(m_statusLabel);
    m_widget->setLayout(l_layout);

    connect(Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
    updateColors();
    updateStatus(i18nc("no active search, no results shown", "Idle."));
}

void CrystalDialog::updateColors()
{
    QPalette p = m_widget->palette();
    p.setColor(QPalette::Window, Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor));
    m_widget->setPalette(p);
    m_statusLabel->setPalette(p);
}

void CrystalDialog::updateStatus(const QString status)
{
    KColorScheme colorTheme = KColorScheme(QPalette::Active, KColorScheme::View,Plasma::Theme::defaultTheme()->colorScheme());
    QString text = QString("<font color=\"%1\">%2</font>").arg(colorTheme.foreground(KColorScheme::NormalText).color().name()).arg(status);
    m_statusLabel->setText(text);
}

void CrystalDialog::search()
{
    if (Nepomuk::ResourceManager::instance()->initialized()) {
        kDebug() << "resource manager inited successfully";
    } else {
        kDebug() << "no resource manager";
    };

    m_resultsView->clear();
    m_matches = 0;
    m_query = m_lineEdit->text();
    kDebug() << "Searching for ..." << m_query << " timeout after:" << m_crystal->timeout();
    updateStatus(i18nc("status in the plasmoid's popup", "Searching for <b>\"%1\"</b>...", m_query));

    // add a timeout in case something goes wrong (no user wants to wait more than N seconds)
    QTimer::singleShot( m_crystal->timeout(), this, SLOT(searchFinished()) );
    m_queryServiceClient->blockingQuery( m_query );
    updateStatus(i18np("Search for <b>\"%2\"</b> finished. %1 matching file found.", "Search for <b>\"%2\"</b> finished. %1 matching files found.", m_matches, m_query));
    kDebug() << m_query << "done.";
}

void CrystalDialog::newMatches( const QList<Nepomuk::Search::Result>& results)
{
    foreach( const Nepomuk::Search::Result& result, results ) {
        Nepomuk::Resource res(result.resourceUri());

        QString type;
        if( res.hasType( Soprano::Vocabulary::Xesam::File() ) ||
            res.resourceUri().scheme() == "file" ) {
            type = KMimeType::findByUrl( res.resourceUri() )->comment();
        }
        else {
            type = Nepomuk::Types::Class( res.resourceType() ).label();
        }
        kDebug() << "Desc:" << res.genericDescription();
        kDebug() << "Score:         " << result.score() << result.resourceUri();
        kDebug() << "GenericLabel:  " << res.genericLabel() << type;
        kDebug() << "Type:          " << type;
        kDebug() << "Icon:          " << res.genericIcon();
        kDebug() << "requestProps:  " << res.properties();
        kDebug() << "Camera Model:  " << res.property("http://freedesktop.org/standards/xesam/1.0/core#cameraModel");

        QHash<QUrl, Nepomuk::Variant> props = res.properties();
        foreach (QUrl u, props.keys()) {
            //kDebug() << "u, v" << u.toString() << props[u].toStringList();
        }
        //kDebug() << "Variant:   " << qVariantFromValue( res );
        if (!result.resourceUri().isValid()) {
            kDebug() << "Skipping invalid URI";
            continue;
        }
        if (!m_crystal->showFolders() && type == "folder") {
           continue;
        }
        //res.setDescription(QString("%1 \nA file found by Crystal").arg(type));
        m_resultsView->updateResource(result);
        m_matches++;
        if (m_matches == m_crystal->maxMatches()) {
            kDebug() << "Maximum number of matches (" << m_crystal->maxMatches() << ") reached, stopping search.";
            m_queryServiceClient->close();
        }
        kDebug() << "\nNew Match:" << res.genericLabel() << type << res.genericIcon() << result.score();
    }
    updateStatus(i18np("Searching for <b>\"%2\"</b>. %1 file found so far...", "%1 files found so far.", m_matches, m_query));
}

void CrystalDialog::searchFinished()
{
    updateStatus(i18np("Search for <b>\"%2\"</b> finished. %1 matching file found.",
                       "Search for <b>\"%2\"</b> finished. %1 matching files found.", m_matches, m_query));
    m_queryServiceClient->close();
}

void CrystalDialog::run(const QUrl& uri)
{
    kDebug() << "Opening ... " << uri;
    if (uri.isValid()) {
        new KRun(uri, m_widget);
    }
}

#include "crystaldialog.moc"
