
//
//  xml_settings.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "xml_settings.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

void XmlSettings::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);
    osc_port = xml.getValue("of2030:osc_port", 2030);
    client_id = xml.getValue("of2030:client_id", 1);
}

void XmlSettings::save(){
    ofxXmlSettings xml;
    xml.setValue("of2030:osc_port", osc_port);
    xml.setValue("of2030:client_id", client_id);
    xml.saveFile(path);
}
