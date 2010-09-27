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

/*
email type:  QUrl( "http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#Email" )

    (QUrl("http://www.w3.org/2000/01/rdf-schema#Resource") ,
    QUrl( "http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PersonContact" )  ,
    QUrl( "http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#Email" )  )
    QUrl( "http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PersonContact" )

"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#nickname" "Giorgos Gousios"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#sentDate" "2008-12-04T11:17:56Z"
"http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel" "Georgios Gousios,Re: Silvester!"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contactUID" "fwXqi129vE"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#plainTextMessageContent" "Hallo Kim!

ja, es bleibt dabei,"

"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#from" "nepomuk:/res/0ff5715c-3119-4000-"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#messageId" "<003501c95601$f770f160$d5151bac@"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#hasPhoneNumber" "nepomuk:/res/20657ced-9fad-4000-"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#isRead" "true"
"http://akonadi-project.org/ontologies/aneo#akonadiItemId" "86584"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#to" "nepomuk:/res/fd953c03-0e7d-4000-"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#messageSubject" "Re: Silvester!"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#cc" "nepomuk:/res/1fd7cf21-f6ed-4000-"
"http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isPartOf" "akonadi:?collection=90,akonadi:?"
"http://www.semanticdesktop.org/ontologies/2007/01/19/nie#byteSize" "8410"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#nameGiven" "Georgios"
"http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol" "internet-mail,view-pim-contacts"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#hasEmailAddress" "mailto:gousiosg@aueb.gr"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nco#nameFamily" "Gousios"
"http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#hasAttachment" "akonadi:?item=86584#2"
"Kim <b>Kerckhoffs</b>."

*/

//Qt
#include <QGraphicsLinearLayout>

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

// Plasma
#include <Plasma/Label>

//own
#include "resourcewidgets/emailwidget.h"
//#include "imagewidget.h"
#include "../utils.h"


using namespace Crystal;

EmailWidget::EmailWidget(Nepomuk::Query::Result result, QGraphicsWidget *parent)
    : ResourceWidget(result, parent)
{
    kDebug() << "Creating an email widget";
    m_email = Email(result.resource().uri());
    m_icon = "internet-mail";
    kDebug() << "need update?";

    updateWidgets();
}

EmailWidget::~EmailWidget()
{
}

void EmailWidget::updateWidgets()
{
    ResourceWidget::updateWidgets();
    //if ((m_width + m_height) > 0) {
        //m_infoLabel->setText(i18nc("info in the image widget", "Image Size: %1x%2", m_width, m_height));
        //kDebug() << "Size: " << m_width << m_height;
    //} else {
    //kDebug() << "=========> Contact updating" << m_resource.properties().keys();
        //kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/05/10/nexif#model")).toString().toInt();
        //kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#width")).toString().toInt();
        //kDebug() << m_resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height")).toString().toInt();
        //http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height
    //}
    //kDebug() << m_resource.properties();
    //m_infoLabel->setText("An Email");
}

QString EmailWidget::info()
{
    return QString("An Email");
}


/*
void EmailWidget::setResource(Nepomuk::Resource resource)
{
    //http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height
    //QString m_url = m_resource.property(QUrl( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url" )).toString();
    m_width = resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#width")).toString().toInt();
    m_height = resource.property(QUrl("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#height")).toString().toInt();
    kDebug() << "------> wxh::" << m_width << m_height;
    ResourceWidget::setResource(resource);
}
void EmailWidget::updateUrl()
{
    m_imageWidget->setMimeType(m_mimeType);
    m_imageWidget->setUrl(m_url);
}

QPixmap EmailWidget::pixmap()
{
    return m_imageWidget->pixmap();
}
*/
#include "emailwidget.moc"
