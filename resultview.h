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

#ifndef CRYSTALRESULTVIEW_H
#define CRYSTALRESULTVIEW_H

//Qt
#include <QGraphicsWidget>
#include <QLabel>
#include <QStringList>


namespace Nepomuk
{
    class Resource;
}

class KFileItem;

namespace Crystal
{
  /**
  * @short The panel used to display search results in a popup
  *
  */
  class ResultView : public QGraphicsWidget
  {
  Q_OBJECT

    public:
        /**
        * Constructor of the dialog
        * @param parent the parent of this object
        **/
        ResultView(QGraphicsWidget *parent = 0);

        virtual ~ResultView();

        virtual void setQuery(const QString&);

    Q_SIGNALS:
        void resourceAdded(Nepomuk::Resource*, const KFileItem&, const QString&);
        void matchFound();

    public Q_SLOTS:
        /** Call to update the view with new entries after inserting them.
         */
        virtual void updateView();

        virtual int count();
        virtual void clear();

        /**
         * @internal Gets called when a new match has been found
         */
        virtual void addMatch(const KFileItem& item);

    protected Q_SLOTS:
        /**
        * @internal update the color of the label to follow plasma theme
        *
        **/
        virtual void updateColors();
        void run(const QUrl&);

    protected:
        QList<Nepomuk::Resource*> m_results;

        // Last query ran
        QString m_query;
        int m_abstractSize;
  };
}

#endif

