

#ifndef XMLHELPER_H_
#define XMLHELPER_H_


#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;
class XmlHelper
{
public:
    XmlHelper();
    virtual ~XmlHelper();

    bool Load(const std::string &strFileName);
    std::string GetNodeValue(const std::string &strNodePath);

    bool IsNodeExist(const std::string &strNodePath);

private:
  XMLDocument m_doc;
  std::map<string, string> node_map;
};


#endif
