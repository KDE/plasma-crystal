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

#ifndef CRYSTALUTILS
#define CRYSTALUTILS


//Qt
class QString;

// Nepomuk
namespace Nepomuk
{
    class Resource;
}

namespace Crystal
{
  /**
  * @short Static helper functions
  *
  */
  class Utils
  {

    public:
        /**
        * Constructor of the dialog
        * @param parent the parent of this object
        **/
        static bool ratingLessThan(const Nepomuk::Resource *r1, const Nepomuk::Resource *r2);

        static QString abstract(Nepomuk::Resource *res, const QString &query, int size = 200);
        static QString abstract(const QString &text, const QString &query, int size = 200);

        static QString highlight(QString text, QString query);

        static QString stripTags(QString input);
  };
}

#endif