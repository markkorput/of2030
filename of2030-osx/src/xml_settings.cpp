
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


//
// local methods
//

void loadOsc(TiXmlDocument &doc, OscSetting &osc_setting){
    TiXmlElement *el = doc.FirstChildElement("of2030");
    if(!el) return;

    el = el->FirstChildElement("osc");
    if(!el) return;

    el = el->FirstChildElement("addresses");
    if(!el) return;

    TiXmlElement* child = el->FirstChildElement();
    string name, val;

    while(child){
        name = child->ValueStr();
        val = child->GetText();
        osc_setting.addresses[name] = val;
        ofLogVerbose() << "[XmlSettings::loadOsc got: " << name << ":" << val;
        child = child->NextSiblingElement();
    }
}

//
// XmlSettings
//

void XmlSettings::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    log_level_name = xml.getValue("of2030:app:log_level", "");
    map<string, ofLogLevel> log_level_map = {
        {"verbose", OF_LOG_VERBOSE},
        {"notice", OF_LOG_NOTICE},
        {"silent", OF_LOG_SILENT},
        {"warning", OF_LOG_WARNING},
        {"", OF_LOG_NOTICE}
    };
    log_level = log_level_map[log_level_name];

    osc_setting.port = xml.getValue("of2030:osc:port", 2030);

    loadOsc(xml.doc, osc_setting);

    client_id = xml.getValue("of2030:client_id", 1);
    multi_client_ids.clear();
    if(xml.pushTag("of2030")){
        if(xml.pushTag("multi")){
            int count = xml.getNumTags("id");
            for(int i=0; i<count; i++){
                int id = xml.getValue("id", 0, i);
                multi_client_ids.push_back(id);
            }
            multi_room_scale = ofVec3f(xml.getValue("room_scale_x", 1.0f),
                                       xml.getValue("room_scale_y", 1.0f),
                                       xml.getValue("room_scale_z", 1.0f));
        }
    }

}

void XmlSettings::save(){
    ofxXmlSettings xml;
    xml.setValue("of2030:osc:port", osc_setting.port);
    xml.setValue("of2030:osc:port", osc_setting.port);

    xml.setValue("of2030:client_id", client_id);

    if(multi_client_ids.size() > 0){
        if(xml.pushTag("of2030")){
            if(xml.pushTag("multi")){
                for(int i=0; i<multi_client_ids.size(); i++){
                    xml.setValue("id", multi_client_ids[i], i);
                }
            }
        }
    }
    xml.saveFile(path);
}
