#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDebug>

#include "mediawiki.h"

struct MediaWikiPrivate {
    QStringList results;
    QUrl apiUrl;
    QNetworkAccessManager *manager;
    int maxItems;
};

MediaWiki::MediaWiki( QObject *parent )
        : QObject( parent )
{
    d = new MediaWikiPrivate;
    d->apiUrl = QUrl("http://en.wikipedia.org/w/api.php");
    d->manager = new QNetworkAccessManager( this );
    d->maxItems = 10;
    connect( d->manager, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply *)) );
}

MediaWiki::~MediaWiki()
{
    delete d;
}

QStringList MediaWiki::results() const
{
    return d->results;
}

int MediaWiki::maxItems() const
{
    return d->maxItems;
}

void MediaWiki::setMaxItems( int limit )
{
    d->maxItems = limit;
}

QUrl MediaWiki::apiUrl() const
{
    return d->apiUrl;
}

void MediaWiki::setApiUrl( const QUrl &url )
{
    d->apiUrl = url;
}


void MediaWiki::search( const QString &searchTerm )
{
    QUrl url = d->apiUrl;
    url.addQueryItem( QString("action"), QString("query") );
    url.addQueryItem( QString("format"), QString("xml") );
    url.addQueryItem( QString("list"), QString("search") );
    url.addQueryItem( QString("srsearch"), searchTerm );
    url.addQueryItem( QString("srlimit"), QString::number(d->maxItems) );

    qDebug() << "Constructed search URL" << url;

    d->manager->get( QNetworkRequest(url) );
}

void MediaWiki::finished( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Request failed, " << reply->errorString();
        emit finished(false);
        return;
    }

    qDebug() << "Request succeeded";
    bool ok = processSearchResult( reply );
    emit finished( ok );
}

bool MediaWiki::processSearchResult( QIODevice *source )
{
    d->results.clear();

    QXmlStreamReader reader( source );
    while ( !reader.atEnd() ) {
        QXmlStreamReader::TokenType tokenType = reader.readNext();
        // qDebug() << "Token" << int(tokenType);
        if ( tokenType == QXmlStreamReader::StartElement ) {
            if ( reader.name() == QString("p") ) {
                QXmlStreamAttributes attrs = reader.attributes();
                //qDebug() << "Found page" << attrs.value( QString("title") );
                d->results << attrs.value( QString("title") ).toString();
            }
        }
        else if ( tokenType == QXmlStreamReader::Invalid )
            return false;
    }

    qDebug() << "Results" << d->results;
    return true;
}
