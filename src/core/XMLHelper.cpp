#include "XMLHelper.h"


QDomNode XMLHelper::findChildNode(const QDomNode &parent, QString name)
{
  QDomNode nodeIt = parent.firstChild();

  while(!nodeIt.isNull() && (nodeIt.nodeName() != name))
    nodeIt = nodeIt.nextSibling();

  return nodeIt;
}


void XMLHelper::appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname,QString str, QList<QPair<QString,QString> > att)
{
  QDomElement elem = doc.createElement(tagname);
  QDomText txt = doc.createTextNode(str);
  elem.appendChild(txt);
  for(QList<QPair<QString,QString> >::Iterator it=att.begin();it!=att.end();++it)
    elem.setAttribute((*it).first, (*it).second);

  parent.appendChild(elem);
}

void XMLHelper::appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname,QString str)
{
  QList<QPair<QString,QString> > att;
  appendTaggedString(doc, parent,tagname,str,att);
}
