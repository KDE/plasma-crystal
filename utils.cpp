#include "utils.h"

#include <kdebug.h>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
//using namespace Crystal
namespace Crystal
{
// For sorting results
bool Utils::ratingLessThan(const Nepomuk::Resource *r1, const Nepomuk::Resource *r2)
{
    return r1->rating() < r2->rating();
}

QString Utils::abstract(Nepomuk::Resource *res, const QString &query, int size)
{
    QString fulltext = res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString();

    kDebug() << "mangling" << res->genericLabel() << ", query:" << query;
    if (fulltext.isEmpty()) {
        return QString();
    }
    QString html = "not found";
    int _i = fulltext.indexOf(query, Qt::CaseInsensitive);
    if (_i >= 0) {
        int _b = qMin(0, _i - (int)(size/2));
        int _l = qMin(size, fulltext.count());
        html = fulltext.midRef(_b, _l).toString();
        kDebug() << "Found query at " << _i << _b << _l;
        kDebug() << "Abstract:" << html;
    }
    return html;
}

QString Utils::highlight(QString &text, const QString &query)
{
    text.replace(query, QString("<font color=\"red\"><strong>%1</strong</font>").arg(query), Qt::CaseInsensitive);
    return text;
}


}
