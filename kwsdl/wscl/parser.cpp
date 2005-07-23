/* 
    This file is part of KDE WSCL Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include <qdom.h>

#include "parser.h"

using namespace WSCL;

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::parse( const QString &xml )
{
  QDomDocument doc( "kwscl" );
  QString errorMsg;
  int errorLine, errorColumn;
  bool ok = doc.setContent( xml, true, &errorMsg, &errorLine, &errorColumn );
  if ( !ok ) {
    qDebug( "Error parsing wscl (%d:%d) %s", errorLine, errorColumn, errorMsg.latin1() );
    return;
  }

  QDomNodeList nodes = doc.elementsByTagName( "Conversation" );
  if ( nodes.count() <= 0 ) {
    qDebug( "No conversation tag found in wscl data" );
    return;
  }

  QDomElement conversationElement = nodes.item( 0 ).toElement();

  mConversation.setName( conversationElement.attribute( "name" ) );
  mConversation.setVersion( conversationElement.attribute( "version" ) );
  mConversation.setDescription( conversationElement.attribute( "description" ) );
  mConversation.setNameSpace( conversationElement.attribute( "targetNamespace" ) );
  mConversation.setSchema( conversationElement.attribute( "hrefSchema" ) );
  mConversation.setInitialInteraction( conversationElement.attribute( "initialInteraction" ) );
  mConversation.setFinalInteraction( conversationElement.attribute( "finalInteraction" ) );

  QDomNode node = conversationElement.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
      if ( element.tagName() == "ConversationInteractions" ) {
        Interaction::List interactions;

        QDomNode interactionNode = element.firstChild();
        while ( !interactionNode.isNull() ) {
          QDomElement interactionElement = interactionNode.toElement();
          if ( !interactionElement.isNull() ) {
            if ( interactionElement.tagName() != "Interaction" ) {
              qDebug( "Expected tag name 'Interaction', got '%s'", interactionElement.tagName().latin1() );
              continue;
            }

            Interaction interaction;
            interaction.setId( interactionElement.attribute( "id" ) );
            const QString type = interactionElement.attribute( "interactionType" );
            if ( type == "ReceiveSend" )
              interaction.setType( Interaction::ReceiveSend );
            else if ( type == "SendReceive" )
              interaction.setType( Interaction::SendReceive );
            else if ( type == "Receive" )
              interaction.setType( Interaction::Receive );
            else if ( type == "Send" )
              interaction.setType( Interaction::Send );
            else if ( type == "Empty" )
              interaction.setType( Interaction::Empty );
            else
              qDebug( "Unknown interaction type '%s'", type.latin1() );

            XMLDocument::List inputDocuments;
            XMLDocument::List outputDocuments;
            XMLDocument inputDocument;
            XMLDocument outputDocument;

            QDomNode contentNode = interactionElement.firstChild();
            while ( !contentNode.isNull() ) {
              QDomElement contentElement = contentNode.toElement();
              if ( !contentElement.isNull() ) {
                const QString tagName = contentElement.tagName();
                if ( tagName == "InboundXMLDocument" ) {
                  XMLDocument document;
                  document.setId( contentElement.attribute( "id" ) );
                  document.setSchema( contentElement.attribute( "hrefSchema" ) );

                  inputDocuments.append( document );
                  inputDocument = document;
                } else if ( tagName == "OutboundXMLDocument" ) {
                  XMLDocument document;
                  document.setId( contentElement.attribute( "id" ) );
                  document.setSchema( contentElement.attribute( "hrefSchema" ) );

                  outputDocuments.append( document );
                  outputDocument = document;
                }
              }


              contentNode = contentNode.nextSibling();
            }

            switch ( interaction.type() ) {
              case Interaction::ReceiveSend:
                {
                  ReceiveSendDocument document;
                  document.setInputDocument( inputDocument );
                  document.setOutputDocuments( outputDocuments );
                  interaction.setReceiveSendDocument( document );
                }
                break;
              case Interaction::SendReceive:
                {
                  SendReceiveDocument document;
                  document.setInputDocuments( inputDocuments );
                  document.setOutputDocument( outputDocument );
                  interaction.setSendReceiveDocument( document );
                }
                break;
              case Interaction::Receive:
                {
                  ReceiveDocument document;
                  document.setInputDocument( inputDocument );
                  interaction.setReceiveDocument( document );
                }
                break;
              case Interaction::Send:
                {
                  SendDocument document;
                  document.setOutputDocument( outputDocument );
                  interaction.setSendDocument( document );
                }
                break;
              case Interaction::Empty:
              default:
                break;
            }

            interactions.append( interaction );
          }

          interactionNode = interactionNode.nextSibling();
        }

        mConversation.setInteractions( interactions );

      } else if ( element.tagName() == "ConversationTransitions" ) {
        Transition::List transitions;

        QDomNode transitionNode = element.firstChild();
        while ( !transitionNode.isNull() ) {
          QDomElement transitionElement = transitionNode.toElement();
          if ( !transitionElement.isNull() ) {
            if ( transitionElement.tagName() != "Transition" ) {
              qDebug( "Expected tag name 'Transition', got '%s'", transitionElement.tagName().latin1() );
              continue;
            }

            Transition transition;

            QDomNode contentNode = transitionElement.firstChild();
            while ( !contentNode.isNull() ) {
              QDomElement contentElement = contentNode.toElement();
              if ( !contentElement.isNull() ) {
                const QString tagName = contentElement.tagName();
                if ( tagName == "SourceInteraction" )
                  transition.setSourceInteraction( contentElement.attribute( "href" ) );
                else if ( tagName == "DestinationInteraction" )
                  transition.setDestinationInteraction( contentElement.attribute( "href" ) );
                else if ( tagName == "SourceInteractionCondition" )
                  transition.setSourceInteractionCondition( contentElement.attribute( "href" ) );
                else
                  qDebug( "Unknown transition element %s", tagName.latin1() );
              }

              contentNode = contentNode.nextSibling();
            }

            transitions.append( transition );
          }

          transitionNode = transitionNode.nextSibling();
        }

        mConversation.setTransitions( transitions );
      }
    }

    node = node.nextSibling();
  }
}

void Parser::reset()
{
  mConversation = Conversation();
}

Conversation Parser::conversation() const
{
  return mConversation;
}
