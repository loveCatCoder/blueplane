/*
 * XmlHelper.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: yjt
 */

#include "XmlHelper.h"
#include <iostream>
#include <stdio.h>


using namespace std;


XmlHelper::XmlHelper()
{

}

XmlHelper::~XmlHelper()
{

}

void traverse_xml(XMLNode *node, std::map<string, string> &node_map, string path = "") {
    if (node->ToElement()) {
        // 节点是一个元素
        XMLElement *element = node->ToElement();
        string name = element->Name();
        path += "/" + name;
 
        // 遍历属性
        for (const XMLAttribute *attr = element->FirstAttribute(); attr; attr = attr->Next()) {
            node_map[attr->Name()] = attr->Value();
        }
 
        // 遍历子节点
        for (XMLNode *child = node->FirstChild(); child; child = child->NextSibling()) {
            traverse_xml(child, node_map, path);
        }
    } else if (node->ToText()) {
        // 节点是文本
        string text = node->ToText()->Value();
        // 忽略空白文本节点
        if (text.find_first_not_of(' ') != string::npos) {
            node_map[path] = text;
        }
    }
}

bool XmlHelper::Load(const std::string &strFileName)
{
    // 解析XML字符串
    XMLError error = m_doc.Parse(strFileName.c_str());
    if( error != XMLError::XML_SUCCESS);
    if (m_doc.Error()) {
        //cout << "解析错误：" << doc.GetErrorStr1() << endl;
        return false;
    }
    traverse_xml(m_doc.RootElement(), node_map);
 
    for (const auto &kv : node_map) {
        cout << kv.first << " -> " << kv.second << endl;
    }
    return true;
}

std::string XmlHelper::GetNodeValue(const std::string &strNodePath)
{
    auto iter = node_map.find(strNodePath);
    if(iter != node_map.end())
    {
        return iter->second;
    }
    return "";
       
}

bool XmlHelper::IsNodeExist(const std::string &strNodePath)
{
    auto iter = node_map.find(strNodePath);
    if(iter != node_map.end())
    {
        return true;
    }
    return false;
}
