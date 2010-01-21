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
//#include <Nepomuk/Types/Class>
//#include <Nepomuk/Variant>
//#include <Soprano/Vocabulary/Xesam>
//#include <nepomuk/queryserviceclient.h>
//#include <nepomuk/result.h>
//#include <nepomuk/resource.h>
#include <nepomuk/resourcemanager.h>
//#include <nepomuk/searchhitview.h>

//own
#include "crystaldialog.h"
#include "crystal.h"


using namespace Crystal;
using namespace Plasma;

//Q_DECLARE_METATYPE( Nepomuk::Search::Result )

CrystalDialog::CrystalDialog(CrystalApplet *crystal)
    : QGraphicsWidget(crystal),
      m_lineEdit(0),
      m_searchButton(0),
      m_statusBar(0),
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
    buildDialog();
}

CrystalDialog::~CrystalDialog()
{
}

void CrystalDialog::updateQuery(const QString query)
{
    m_lineEdit->setText(query);
    //m_lineEdit->selectAll(); FIXME
    m_lineEdit->setFocus();
}

void CrystalDialog::updateIconSize()
{
    int s = qMax(16, m_iconSizes[m_crystal->iconSize()]);
    kDebug() << "FIXME: Setting iconsize" << s;
    //m_resultsView->setIconSize(QSize(s, s));
}

void CrystalDialog::buildDialog()
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

    m_resultsView = new Plasma::WebView(this);
    m_resultsView->setMinimumSize(160, 200);
    m_resultsView->setHtml(QString("<h1>Teh ressults:</h1>"));
    gridLayout->addItem(m_resultsView, 1, 0, 1, 2);

    m_statusBar = new Plasma::Label(this);
    m_statusBar->setText("status");
    gridLayout->addItem(m_statusBar, 2, 0, 1, 2);

    connect(Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
    updateColors();
    updateStatus(i18nc("no active search, no results shown", "Idle."));
    setPreferredSize(200, 300);
    setMaximumSize(400, 500);
}

void CrystalDialog::updateColors()
{
    /*
    QPalette p = m_widget->palette();
    p.setColor(QPalette::Window, Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor));
    m_widget->setPalette(p);
    m_statusLabel->setPalette(p);
    m_resultsView->setPalette(p);
    */
}

void CrystalDialog::updateStatus(const QString status)
{
    KColorScheme colorTheme = KColorScheme(QPalette::Active, KColorScheme::View,Plasma::Theme::defaultTheme()->colorScheme());
    QString text = QString("<font color=\"%1\">%2</font>").arg(colorTheme.foreground(KColorScheme::NormalText).color().name()).arg(status);
    m_statusBar->setText(text);
}

void CrystalDialog::search()
{
    if (Nepomuk::ResourceManager::instance()->initialized()) {
        kDebug() << "resource manager inited successfully";
    } else {
        kDebug() << "no resource manager";
    };

    //m_resultsView->clear();
    m_matches = 0;
    m_query = m_lineEdit->text();
    kDebug() << "Searching for ..." << m_query << " timeout after:" << m_crystal->timeout();
    updateStatus(i18nc("status in the plasmoid's popup", "Searching for <b>\"%1\"</b>...", m_query));

    // add a timeout in case something goes wrong (no user wants to wait more than N seconds)
    QTimer::singleShot( m_crystal->timeout(), this, SLOT(searchFinished()) );
    //m_queryServiceClient->query( m_query );
    updateStatus(i18n( "Searching for<b>\"%2\"</b> ...", m_query));
}

/*
void CrystalDialog::newMatches( const QList<Nepomuk::Search::Result>& results)
{
    / *
    foreach( const Nepomuk::Search::Result& result, results ) {
        if (!result.resourceUri().isValid()) {
            kDebug() << "Skipping invalid URI";
            continue;
        }
        Nepomuk::Resource res(result.resourceUri());
        / *
            Here, we  have our chance to make sense of a random URL for Resource.
            We should retrieve data from the MediaWiki class and then for example:

            res.setDescription(QString("%1 \nA file found by Crystal").arg(type));

            ... but only if we have the resource newly created. This way, we get those
            hits from the MediaWiki stored in Nepomuk.

            Need to investigate what impact this has on performance. (I guess we
            can make it async, as long as we don't throw away our temporary data
            and not re-retrieve it from the Nepomukstore)
        * /

        QString type;
        if( res.hasType( Soprano::Vocabulary::Xesam::File() ) ||
            res.resourceUri().scheme() == "file" ) {
            type = KMimeType::findByUrl( res.resourceUri() )->comment();
        }
        else {
            type = Nepomuk::Types::Class( res.resourceType() ).label();
        }
        / *
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
        * /
        if (!m_crystal->showFolders() && type == "folder") {
           continue;
        }
        m_resultsView->updateResource(result);
        m_matches++;
        if (m_matches == m_crystal->maxMatches()) {
            kDebug() << "Maximum number of matches (" << m_crystal->maxMatches() << ") reached, stopping search.";
            m_queryServiceClient->close();
        }
        kDebug() << "\nNew Match:" << res.genericLabel() << type << res.genericIcon() << result.score();
    }
    * /
    updateStatus(i18np("Searching for <b>\"%2\"</b>. %1 file found so far...",
                       "Searching for <b>\"%2\"</b>. %1 files found so far...", m_matches, m_query));
    m_crystal->updateToolTip(m_query, m_matches);

}
*/
void CrystalDialog::searchFinished()
{
    updateStatus(i18np("Search for <b>\"%2\"</b> finished. %1 matching file found.",
                       "Search for <b>\"%2\"</b> finished. %1 matching files found.", m_matches, m_query));
    //m_queryServiceClient->close();
    m_crystal->updateToolTip(m_query, m_matches);
}

void CrystalDialog::run(const QUrl& uri)
{
    kDebug() << "Opening ... " << uri;
    if (uri.isValid()) {
        new KRun(uri, 0);
    }
}

#include "crystaldialog.moc"
