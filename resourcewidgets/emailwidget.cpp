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
#include "../utils.h"


using namespace Crystal;

EmailWidget::EmailWidget(QGraphicsWidget *parent)
    : ResourceWidget(parent)
{
    kDebug() << "Creating an email widget";
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
    //m_infoLabel->setText(i18nc("info in the image widget", "Image Size: %1x%2", m_width, m_height));
}

QString EmailWidget::info()
{
    return QString("An Email");
}

void EmailWidget::setResource(Nepomuk::Resource resource)
{
    m_email = Email(resource.uri());
    ResourceWidget::setResource(resource);
}


#include "emailwidget.moc"
