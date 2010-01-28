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

#ifndef CRYSTALAPPLET_H
#define CRYSTALAPPLET_H

//Plasma
#include <Plasma/PopupApplet>
#include <Plasma/ToolTipManager>

// own
#include "ui_crystalConfig.h"
#include "dialog.h"

class QGraphicsProxyWidget;

//desktop view
namespace Plasma
{
    class IconWidget;
    class ToolTipContent;
}

namespace Crystal
{
  class Applet : public Plasma::PopupApplet
  {
    Q_OBJECT

    public:
        Applet(QObject *parent, const QVariantList &args);
        ~Applet();
        void init();
        QGraphicsWidget *graphicsWidget();
        int timeout();
        int maxMatches();
        bool showFolders();
        int iconSize();
        QString defaultQuery();
        void updateToolTip(const QString query, const int matches);

    protected:
        void popupEvent(bool show);
        void createConfigurationInterface(KConfigDialog *parent);

    protected Q_SLOTS:
        void configAccepted();
        void openConfig();

    private:
        ///the icon used when the applet is in the taskbar
        Plasma::IconWidget *m_icon;

        ///The dialog displaying matches
        Dialog *m_dialog;

        Plasma::ToolTipContent m_toolTip;

        // Configuration dialog
        Ui::crystalConfig ui;

        ///the number of items displayed in the dialog
        //int m_numberItems;
        // maximum query execution time in milliseconds
        int m_timeout;
        // maximum sensible number of matches
        int m_maxMatches;
        // icon size of the result items
        int m_iconSize;
        // showing or hiding folders in results
        bool m_showFolders;
        // Use clipboard as query
        bool m_useClipboard;
        // pre-filled in query
        QString m_defaultQuery;

        QString m_arg;
  };
};

#endif
