#include "XMLHelper.h"


QDomNode XMLHelper::findChildNode(const QDomNode &parent, std::string name)
{
  QDomNode nodeIt = parent.firstChild();

  while(!nodeIt.isNull() && (nodeIt.nodeName() != name))
    nodeIt = nodeIt.nextSibling();

  return nodeIt;
}


void XMLHelper::appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname,QString str, QValueList<QPair<QString,QString> > att)
{
  QDomElement elem = doc.createElement(tagname);
  QDomText txt = doc.createTextNode(str);
  elem.appendChild(txt);
  for(int i=0;i<att.size();++i)
    elem.setAttribute(att[0].first,att[0].second);

  parent.appendChild(elem);
}

void XMLHelper::appendTaggedString(QDomDocument &doc, QDomNode &parent, QString tagname,QString str)
{
  QValueList<QPair<QString,QString> > att;
  appendTaggedString(doc, parent,tagname,str,att);
}
