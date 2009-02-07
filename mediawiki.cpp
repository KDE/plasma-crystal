/*
 *   Copyright 2009 by Richard Moore <rich@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QTimer>
#include <QDebug>

#include "mediawiki.h"

enum State {
    StateApiChanged,
    StateApiUpdating,
    StateReady
};

struct MediaWikiPrivate {
    int state;
    QList<MediaWiki::Result> results;
    QUrl apiUrl;
    QUrl baseUrl;
    QNetworkAccessManager *manager;
    int maxItems;
    QNetworkReply *reply;
    int timeout;
    QUrl query;
};

MediaWiki::MediaWiki( QObject *parent )
        : QObject( parent )
{
    d = new MediaWikiPrivate;
    d->state = StateApiChanged;
    d->apiUrl = QUrl("http://en.wikipedia.org/w/api.php");
    d->manager = new QNetworkAccessManager( this );
    d->maxItems = 10;
    d->timeout = 30 * 1000; // 30 second
    d->reply = 0;

    connect( d->manager, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply *)) );
}

MediaWiki::~MediaWiki()
{
    delete d;
}

QList<MediaWiki::Result> MediaWiki::results() const
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
    if ( d->apiUrl == url )
	return;

    d->apiUrl = url;
    d->state = StateApiChanged;
}

int MediaWiki::timeout() const
{
    return d->timeout;
}

void MediaWiki::setTimeout( int millis )
{
    d->timeout = millis;
}

void MediaWiki::abort()
{
    if ( !d->reply )
	return;

    d->reply->abort();
    d->reply = 0;
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

    if ( d->state == StateReady ) {
	d->reply = d->manager->get( QNetworkRequest(url) );
	QTimer::singleShot( d->timeout, this, SLOT( abort() ) );
    }
    else if ( d->state == StateApiChanged ) {
	d->query = url;
	findBase();
    }
}

void MediaWiki::findBase()
{
    // http://en.wikipedia.org/w/api.php?action=query&meta=siteinfo
    QUrl url = d->apiUrl;
    url.addQueryItem( QString("action"), QString("query") );
    url.addQueryItem( QString("format"), QString("xml") );
    url.addQueryItem( QString("meta"), QString("siteinfo") );

    qDebug() << "Constructed base query URL" << url;
    d->reply = d->manager->get( QNetworkRequest(url) );
    d->state = StateApiUpdating;
}

void MediaWiki::finished( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Request failed, " << reply->errorString();
        emit finished(false);
        return;
    }

    qDebug() << "Request succeeded";

    if ( d->state == StateApiUpdating ) {
	bool ok = processBaseResult( reply );
	emit finished( ok );
	reply->deleteLater();

	d->state = StateReady;
	d->reply = d->manager->get( QNetworkRequest(d->query) );
	QTimer::singleShot( d->timeout, this, SLOT( abort() ) );
    }
    else {
	bool ok = processSearchResult( reply );

	emit finished( ok );
	d->reply->deleteLater();
	d->reply = 0;
    }
}

bool MediaWiki::processBaseResult( QIODevice *source )
{
    QXmlStreamReader reader( source );

    while ( !reader.atEnd() ) {
        QXmlStreamReader::TokenType tokenType = reader.readNext();
        // qDebug() << "Token" << int(tokenType);
        if ( tokenType == QXmlStreamReader::StartElement ) {
            if ( reader.name() == QString("general") ) {
                QXmlStreamAttributes attrs = reader.attributes();

		d->baseUrl = QUrl( attrs.value( QString("base") ).toString() );
		return true;
	    }
	}
        else if ( tokenType == QXmlStreamReader::Invalid )
            return false;

    }

    return true;
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

		Result r;
		r.url = d->baseUrl.resolved( attrs.value( QString("title") ).toString() );
		r.title = attrs.value( QString("title") ).toString();

		qDebug() << "Got result: url=" << r.url << "title=" << r.title;

		d->results.append( r );
            }
        }
        else if ( tokenType == QXmlStreamReader::Invalid )
            return false;
    }

    return true;
}


