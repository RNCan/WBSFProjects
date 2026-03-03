#pragma once

#include <vector>
#include "Basic/ERMsg.h"
#include "Basic/UtilStd.h"
#include "Basic/pugixml/pugixml.hpp"

namespace WBSF
{

    //typedef pugi::xml_node xml_node;



    //use this template to write class that have write_xml member
    template <template<typename, typename> typename Container, typename T, typename A>
    void write_xml(const Container<T, A>& data, pugi::xml_node& parent_node, const std::string& element_name)
    {
        for (T val : data)
        {
            val.write_xml(parent_node.append_child(element_name.c_str()));
        }
    }

    ////use this template to write template of basic type like int, float ...
    //template<template <typename> class Container, typename T>
    //void write_xml_values(const Container<T>& data, const pugi::xml_node& parent_node, const std::string& element_name)
    //{
    //    for (T val : data)
    //    {
    //        parent_node.append_child(element_name.c_str()).append_child(pugi::node_pcdata).set_value(std::to_string(val).c_str());
    //    }
    //}


    template <template<typename, typename> typename Container, typename T, typename A>
    void read_xml(pugi::xml_node parent_node, const std::string& element_name, Container<T, A>& out)
    {
        for (pugi::xml_node node : parent_node)
        {
            T elem;
            elem.read_xml(node);
            out.push_back(elem);
        }
    }



    template<template <typename> class Container, typename T>
    Container<T> read_xml_values(pugi::xml_node& parent_node, const std::string& element_name)
    {
        Container<T> out;
        for (pugi::xml_node node : parent_node)
        {
            out.push_back(to_value<T>(node.text()));
        }

        return out;
    }




    class xml_document : public pugi::xml_document
    {
    public:


        ERMsg load_file(const std::string& file_path)
        {
            ERMsg msg;

            pugi::xml_parse_result result = pugi::xml_document::load_file(file_path.c_str());
            if (!result)
            {
                msg.ajoute(result.description());
                msg.ajoute("Unable to read file: " + file_path);
            }

            return msg;
        }

        ERMsg save_file(const std::string& file_path)
        {
            ERMsg msg;

            bool result = pugi::xml_document::save_file(file_path.c_str());
            if (!result)
                msg.ajoute("Unable to save file: " + file_path);

            return msg;
        }

    };


    template<class T>
    std::string to_xml_string(const T& object, const std::string& XML_FLAG)
    {

        pugi::xml_document doc;

        pugi::xml_node root = doc.append_child(XML_FLAG.c_str());
        //doc.append_attribute("version").set_value(version);
        object.write_xml(root);

        std::stringstream ss;
        doc.save(ss);

        return ss.str();
    }

    template<class T>
    ERMsg from_xml_string(const std::string& xml, T& object, const std::string& XML_FLAG)
    {
        ERMsg msg;

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml.c_str());
        if(result)
        {
            //int xml_version = doc.child(XML_FLAG).attribute("version").as_int();
            //if (xml_version == version)
            //{
                object.read_xml(doc.child(XML_FLAG.c_str()));
            //}
            ///else
            //{
              //  msg.ajoute(FormatMsg("Error reading xml version: %1% required, %2% read",to_string(version), to_string(xml_version)) );
            //}
        }
        else
        {
            msg.ajoute(result.description());
        }

        return msg;
    }

}

