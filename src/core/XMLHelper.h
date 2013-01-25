#ifndef XMLHELPER_INCLUDED
#define XMLHELPER_INCLUDED

#include <QtXml>
#include <qpair.h>

class XMLHelper
{
public:

  static QDomNode findChildNode(const QDomNode &parent, QString name);
  static void appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname, QString str,
                                 QList<QPair<QString,QString> > att);
																 
  static void appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname, QString str);
};

#endif


