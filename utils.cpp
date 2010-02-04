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

#include "utils.h"

#include <kdebug.h>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

namespace Crystal
{
// For sorting results
bool Utils::ratingLessThan(const Nepomuk::Resource *r1, const Nepomuk::Resource *r2)
{
    return r1->rating() < r2->rating();
}

QString Utils::abstract(Nepomuk::Resource *res, const QString &query, int size)
{
    return abstract(res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString(), query, size);
}

QString Utils::abstract(const QString &text, const QString &query, int size)
{
    QString plainText = stripTags(text);
    //kDebug() << "========================== mangling" << text.count() << query << size; // << res->genericLabel() << ", query:" << query;
    if (plainText.isEmpty()) {
        return plainText;
    }
    if (query.isEmpty()) {
        //kDebug() << "query empty, no higlighting";
        return plainText.midRef(0, size).toString();
    }
    QString html;
    int _i = plainText.indexOf(query, Qt::CaseInsensitive);
    if (_i >= 0) {
        int _b = qMax(0, _i - (int)(size/2));
        int _l = qMin(size, plainText.count());
        html = plainText.midRef(_b, _l).toString();
        //kDebug() << "...............................................................";
        //kDebug() << plainText;
        //kDebug() << "...............................................................";
        kDebug() << "Found query at " << _i << _b << _l;
        // display "..."?
        if (_b) {
            html = QString("... %1").arg(html);
        }
        if ((_b+_l) < plainText.count()) {
            html = QString("%1 ...").arg(html);
        }
        kDebug() << "Abstract:" << html;
    } else {
        html = html.midRef(0, size).toString();
    }
    return html;
}

QString Utils::highlight(QString text, QString query)
{
    text.replace(query, QString("<font color=\"red\"><strong>%1</strong></font>").arg(query), Qt::CaseInsensitive);
    return text;
}

QString Utils::stripTags(QString input)
{
    if (input.isEmpty()) {
        return input;
    }
    QString txt = input;
    QRegExp regex("<.*>");
    regex.setMinimal(true);
    txt = txt.remove(regex);
    txt.replace("\n", " ");
    txt.replace("\t", " ");
    return txt;
}


}

