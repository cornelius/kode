/* 
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#ifndef SCHEMA_COMPLEXTYPE_H
#define SCHEMA_COMPLEXTYPE_H

#include <qstring.h>

#include "attribute.h"
#include "element.h"
#include "qualifiedname.h"
#include "xsdtype.h"

namespace Schema {

class ComplexType : public XSDType
{
  public:
    typedef QValueList<ComplexType> List;

    typedef enum { 
      SEQ = 0, 
      CHOICE, 
      ALL 
    } Compositor;

    typedef enum {
      Restriction,
      Extension
    } Derivation;
       
    ComplexType();
    ComplexType( const QString& );
    ~ComplexType();

    void setName( const QString &name );
    QString name() const;

    QualifiedName qualifiedName() const;

    void setDocumentation( const QString &documentation );
    QString documentation() const;

    void setType( int type );
    int type() const;

    void setContentType( int contentType );
    int contentType() const;

    void setContentModel( int model );
    int contentModel() const;

    bool isSimple() const;

    int attributeType( int index );
    QString attributeName( int index );

    int elementType( int index );
    QString elementName( int index );

    int numElements() const;
    int numAttributes() const;

    void setAnonymous( bool anonymous );
    bool isAnonymous() const;

    void setBaseType( int type, Derivation derivation, const XSDType *ptr );
    void setBaseTypeName( const QString &baseTypeName );

    int baseType() const;
    int baseDerivation() const;
    QString baseTypeName() const;

    Compositor topLevelGroup() const;
    Compositor groupType( int groupId ) const;

    const Element *element( const QString &name );
    const Attribute *attribute( const QString &name );
    Element *element( int id );
    Attribute *attribute( int id );

    void setElements( const Element::List &elements );
    Element::List elements() const;

    void setAttributes( const Attribute::List &attributes );
    Attribute::List attributes() const;

    void setIsArray( bool isArray );
    bool isArray() const;

    void setCompositor( Compositor type, bool open = true, int minOccurs = 1, int maxOccurs = 1 );

    void addAttribute( const QString &name, int type_id, bool qualified = false,
                       const QString &defaultValue = QString(),
                       const QString &fixedValue = QString(),
                       bool use = false );
    void addAttributeRef( const QualifiedName &name, bool qualified, bool use );

    void addElement( const QString &name, int type_id, int minOccurs = 1,
                     int maxOccurs = 1, bool qualified = false,
                     const QString &defaultValue = QString(),
                     const QString &fixedValue = QString(),
                     const QString &documentation = QString() );
    void addElementRef( const QualifiedName &name, int minOccurs, int maxOccurs );

    void matchAttributeRef( const QString &name, Attribute &attribute );
    void matchElementRef( const QString &name, Element &element );

    bool checkOccurrences();
    void resetCounters();

  private:
    QString mName;
    QString mNameSpace;
    QString mDocumentation;
    int mType;

    Element::List mElements;
    Attribute::List mAttributes;

    int mContentModel;
    bool mMixed;
    bool mAnonymous;
    bool mIsArray;
    int mContentType;
 
    struct
    {
      int typeId;
      Derivation derivation;
      const  XSDType *type;
      QString name;
    } mBaseType;
  
    struct CompositorStruct
    {
      CompositorStruct()
      {
      }

      CompositorStruct( Compositor _type, int min = 1, int max = 1 )
        : type( _type ), minOccurs( min ), maxOccurs( max )
      {
      }

      Compositor type;
      int minOccurs;
      int maxOccurs;
    };

    QValueList<struct CompositorStruct> mGroups;

    Compositor mTopLevelGroup;
    int mCurrentGroup;
    int mPreviousGroup;
    bool mForwardElementRef;
    bool mForwardAttributeRef;
};

}

#endif
