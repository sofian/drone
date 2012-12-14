#include "XMLHelper.h"


QDomNode XMLHelper::findChildNode(const QDomNode &parent, std::string name)
{
  QDomNode nodeIt = parent.firstChild();
  
  while(!nodeIt.isNull() && (nodeIt.nodeName() != name.c_str()))  
    nodeIt = nodeIt.nextSibling();
     
  return nodeIt;
}
