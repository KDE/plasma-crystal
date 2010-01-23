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
    return abstract(res->property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent")).toString(), query, size);
}

QString Utils::abstract(const QString &text, const QString &query, int size)
{
    QString plainText = stripTags(text);
    //kDebug() << "mangling" << res->genericLabel() << ", query:" << query;
    if (plainText.isEmpty()) {
        return plainText;
    }
    if (query.isEmpty()) {
        return plainText.midRef(0, size).toString();
    }
    QString html;
    int _i = plainText.indexOf(query, Qt::CaseInsensitive);
    if (_i >= 0) {
        int _b = qMin(0, _i - (int)(size/2));
        int _l = qMin(size, plainText.count());
        html = plainText.midRef(_b, _l).toString();
        kDebug() << "Found query at " << _i << _b << _l;
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
