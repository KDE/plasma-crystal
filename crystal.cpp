/***************************************************************************
 *   Copyright 2008-2010 by Sebastian Kügler <sebas@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

//own
#include "crystal.h"
#include "crystaldialog.h"

//Qt
#include <QTimer>
#include <QClipboard>

//KDE
#include <KIcon>
#include <KConfigDialog>
#include <KStandardDirs>
#include <KToolInvocation>

//plasma
#include <Plasma/Dialog>
//use for desktop view
#include <Plasma/IconWidget>
#include <Plasma/Theme>
#include <Plasma/ToolTipManager>

// Nepomuk
#include <Nepomuk/Resource>
//#include <Nepomuk/Types/Class>
#include <nepomuk/resourcemanager.h>


using namespace Plasma;
using namespace Crystal;

K_EXPORT_PLASMA_APPLET(crystal, CrystalApplet)

CrystalApplet::CrystalApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
      m_icon(0),
      m_dialog(0)
{
    Nepomuk::ResourceManager::instance()->init();
    if (Nepomuk::ResourceManager::instance()->initialized()) {
        kDebug() << "resource manager inited successfully";
    } else {
        kDebug() << "no resource manager";
    };


    setBackgroundHints(StandardBackground);
    setAspectRatioMode(IgnoreAspectRatio);
    setHasConfigurationInterface(true);
    setAcceptsHoverEvents(true);

    // initialize the widget
    (void)graphicsWidget();

    //resize(graphicsWidget()->sizeHint());
    m_iconSize = 1;
}

CrystalApplet::~CrystalApplet()
{
    delete m_icon;
    delete m_dialog;
}

void CrystalApplet::init()
{
    KConfigGroup cg = config();

    m_icon = new Plasma::IconWidget(KIcon("nepomuk",NULL), QString());

    Plasma::ToolTipManager::self()->registerWidget(this);

    setPopupIcon(m_icon->icon());

    m_timeout = cg.readEntry("timeout", 20000);
    m_maxMatches = cg.readEntry("maxMatches", 25);
    m_showFolders = cg.readEntry("showFolders", false);
    m_iconSize = cg.readEntry("iconSize", 1);
    m_useClipboard = cg.readEntry("useClipboard", true);
    m_defaultQuery = cg.readEntry("defaultQuery", "kmix.png");

    kDebug() << "timeout, maxMatches, useclipboard, defaultquery:" << m_timeout << m_maxMatches << m_useClipboard << m_defaultQuery << m_iconSize;
    m_dialog->updateQuery(m_defaultQuery);
    m_dialog->updateIconSize();
    updateToolTip("", 0);
}

int CrystalApplet::timeout()
{
    return m_timeout;
}

int CrystalApplet::maxMatches()
{
    return m_maxMatches;
}

bool CrystalApplet::showFolders()
{
    return m_showFolders;
}

int CrystalApplet::iconSize()
{
    return m_iconSize;
}

QString CrystalApplet::defaultQuery()
{
    return m_defaultQuery;
}

QGraphicsWidget* CrystalApplet::graphicsWidget()
{
    if (!m_dialog) {
        m_dialog = new CrystalDialog(this);
    }
    return m_dialog;
}

void CrystalApplet::popupEvent(bool show)
{
    int maxLength = 30;
    if (show) {
        QString clip = QApplication::clipboard()->text();
        // TODO: magic number
        // Having more long texts in the buffer is useless for search, so we restrict it to 30 for now
        if (!clip.isEmpty() && clip.count() <= maxLength && m_useClipboard) {
            kDebug() << "Clipboard:" << clip;
            m_dialog->updateQuery(clip);
        }
        // At least warn in this case so people find out what's going on
        if (clip.count() > maxLength) {
            kWarning() << "clipboard contents is longer than" << maxLength << "chars. Not using it for the query";
        }
    }
}

void CrystalApplet::updateToolTip(const QString query, const int matches)
{

    if (!query.isEmpty()) {
        m_toolTip = Plasma::ToolTipContent(i18nc("Tooltip main title text", "Crystal Desktop Search"),
                        i18ncp("Tooltip sub text", "Last search: %2 (%1 match)", "Last search: %2 (%1 matches)", matches, query),
                        KIcon("nepomuk").pixmap(IconSize(KIconLoader::Desktop))
                    );
    } else {
        m_toolTip = Plasma::ToolTipContent(i18nc("No search has been done yet", "Crystal Desktop Search"),
                        i18nc("Tooltip sub text", "Click on the icon to start searching"),
                        KIcon("nepomuk").pixmap(IconSize(KIconLoader::Desktop))
                    );

    }
        Plasma::ToolTipManager::self()->setContent(this, m_toolTip);
}

void CrystalApplet::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget(parent);
    ui.setupUi(widget);
    parent->addPage(widget, i18n("Search Options"), Applet::icon());
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    ui.timeoutSpin->setValue((int)m_timeout/1000); // (conversion from msec)
    ui.matchesSpin->setValue(m_maxMatches);
    ui.sizeSlider->setValue(m_iconSize);
    ui.showFoldersCheck->setChecked(m_showFolders);
    ui.sizeSlider->setValue(m_iconSize);
    ui.clipboard->setChecked(m_useClipboard);
    ui.defaultQuery->setText(m_defaultQuery);
    ui.defaultQuery->setEnabled(!m_useClipboard);

    connect(ui.configButton, SIGNAL(clicked()), this, SLOT(openConfig()));
}

void CrystalApplet::configAccepted()
{
    KConfigGroup cg = config();

    if (m_timeout != ui.timeoutSpin->value()*1000) {
        m_timeout = ui.timeoutSpin->value() * 1000; // (conversion to msec)
        cg.writeEntry("timeout", m_timeout);
        kDebug() << "timeout Changed to" << m_timeout;
    }

    if (m_maxMatches != ui.matchesSpin->value()) {
        m_maxMatches = ui.matchesSpin->value();
        cg.writeEntry("timeout", m_maxMatches);
        kDebug() << "maxMatches changed to" << m_maxMatches;
    }

    if (m_showFolders != ui.showFoldersCheck->isChecked()) {
        m_showFolders = !m_showFolders;
        cg.writeEntry("showFolders", m_showFolders);
    }

    if (ui.sizeSlider->value() != m_iconSize) {
        m_iconSize = ui.sizeSlider->value();
        kDebug() << "iconSize changed" << m_iconSize;
        cg.writeEntry("iconSize", m_iconSize);
        m_dialog->updateIconSize();
    }

    if (m_useClipboard != ui.clipboard->isChecked()) {
        m_useClipboard = !m_useClipboard;
        cg.writeEntry("useClipboard", m_useClipboard);
    }

    if (ui.defaultQuery->text() != m_defaultQuery) {
        m_defaultQuery = ui.defaultQuery->text();
        kDebug() << "defaultQuery changed:" << m_defaultQuery;
        cg.writeEntry("defaultQuery", m_defaultQuery);
        if (!m_useClipboard) {
            m_dialog->updateQuery(m_defaultQuery);
        }
    }
    emit configNeedsSaving();
}

void CrystalApplet::openConfig()
{
    kDebug() << "opening Nepomuk configuration dialog";
    QStringList args;
    args << "kcm_nepomuk";
    KToolInvocation::kdeinitExec("kcmshell4", args);
}


#include "crystal.moc"
