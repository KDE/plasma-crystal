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

#ifndef MEDIAWIKI_H
#define MEDIAWIKI_H

#include <QObject>
#include <QList>
#include <QUrl>

class QNetworkReply;

class MediaWiki : public QObject
{
    Q_OBJECT

public:
    class Result {
    public:
	Result() {
	};

	Result( const Result &r ) {
	    this->title = r.title;
	    this->url = r.url;
	};

	Result &operator= (const Result &other)  {
	    this->title = other.title;
	    this->url = other.url;

	    return *this;
	};

	QString title;
	QUrl url;	
    };

    MediaWiki( QObject *parent );
    virtual ~MediaWiki();

    QList<MediaWiki::Result> results() const;

    int maxItems() const;
    void setMaxItems( int limit );

    QUrl apiUrl() const;
    void setApiUrl( const QUrl &url );

    int timeout() const;
    void setTimeout( int millis );

signals:
    void finished( bool success );

public slots:
    void search( const QString &searchTerm );

    void abort();

private slots:
    void finished( QNetworkReply *reply );

private:
    void findBase();
    bool processBaseResult( QIODevice *source );
    bool processSearchResult( QIODevice *source );

    struct MediaWikiPrivate *d;
};

#endif // MEDIAWIKI_H
