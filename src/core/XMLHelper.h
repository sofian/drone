#ifndef XMLHELPER_INCLUDED
#define XMLHELPER_INCLUDED

#include <qdom.h>

class XMLHelper
{
public:

  static QDomNode findChildNode(const QDomNode &parent, std::string name);
};
                 
#endif


