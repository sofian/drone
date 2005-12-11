#ifndef XMLHELPER_INCLUDED
#define XMLHELPER_INCLUDED

#include <qdom.h>
#include <qpair.h>
#include <qvaluelist.h>

class XMLHelper
{
public:

  static QDomNode findChildNode(const QDomNode &parent, std::string name);
  static void appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname, QString str,
                                 QValueList<QPair<QString,QString> > att);
  static void appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname, QString str);
};

#endif


