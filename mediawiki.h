#ifndef MEDIAWIKI_H
#define MEDIAWIKI_H

#include <QObject>
#include <QStringList>
#include <QUrl>

class QNetworkReply;

class MediaWiki : public QObject
{
    Q_OBJECT

public:
    MediaWiki( QObject *parent );
    virtual ~MediaWiki();

    QHash <QString, QUrl> results() const;

    int maxItems() const;
    void setMaxItems( int limit );

    QUrl apiUrl() const;
    void setApiUrl( const QUrl &url );

signals:
    void finished( bool success );

public slots:
    void search( const QString &searchTerm );

private slots:
    void finished( QNetworkReply *reply );

private:
    bool processSearchResult( QIODevice *source );

    struct MediaWikiPrivate *d;
};

#endif // MEDIAWIKI_H
