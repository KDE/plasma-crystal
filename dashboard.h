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

#ifndef CRYSTALDASHBOARD_H
#define CRYSTALDASHBOARD_H

// own
#include "dashboard.h"

//Qt
#include <QGraphicsWidget>

// Plasma
#include <Plasma/WebView>

class StyleSheet;

namespace Crystal
{
  /**
  * @short DashBoard is a Plasma Widget to display a Nepomuk::Resource.
  *
  */
  class DashBoard : public Plasma::WebView
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the DashBoard
        * @param resource The Nepomuk::Resource this widget displays.
        * @param parent the parent of this object
        **/
        DashBoard(QGraphicsWidget *parent = 0);
        virtual ~DashBoard();

        void setHistory(const QStringList &history);

    Q_SIGNALS:
        void search(const QUrl nepomukUrl);

    public Q_SLOTS:
        void load(const QString &templatePath);
        void update();
        void linkClicked(const QUrl &clickedUrl);

    private:
        QString header();
        QString tags();
        QString recent();
        QString notice();

        StyleSheet *m_css;
        KDirWatch *m_templateWatch;

        QString m_baseDir;
        QString m_templatePath;
        QString m_template;

        QStringList m_history;
  };
}

#endif

