
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

XmlSettings* XmlSettings::singleton = NULL;

XmlSettings* XmlSettings::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new XmlSettings();
    }
    return singleton;
}


void XmlSettings::load(bool reload){
    if(loaded && !reload) return;

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

    client_id = xml.getValue("of2030:client_id", "0");
    room_size = ofVec3f(xml.getValue("of2030:room_size_x", 1.0f),
                       xml.getValue("of2030:room_size_y", 1.0f),
                       xml.getValue("of2030:room_size_z", 1.0f));

#ifdef __MULTI_CLIENT_ENABLED__
    multi_client_ids.clear();
    if(xml.pushTag("of2030")){
        if(xml.pushTag("multi")){
            int count = xml.getNumTags("id");
            for(int i=0; i<count; i++){
                string id = xml.getValue("id", "0", i);
                multi_client_ids.push_back(id);
            }
            multi_debug = xml.getValue("debug", "false") == "true";
            multi_scale = xml.getValue("scale", 1.0f);
            multi_translate = ofVec3f(xml.getValue("translate_x", 0.0f),
                                      xml.getValue("translate_y", 0.0f),
                                      xml.getValue("translate_z", 0.0f));
        }
    }
#endif // __MULTI_CLIENT_ENABLED__

    loaded = true;
}

//void XmlSettings::save(){
//    ofxXmlSettings xml;
//    xml.setValue("of2030:osc:port", osc_setting.port);
//    xml.setValue("of2030:osc:port", osc_setting.port);
//    xml.setValue("of2030:client_id", client_id);
//
//#ifdef __MULTI_CLIENT_ENABLED__
//    if(multi_client_ids.size() > 0){
//        if(xml.pushTag("of2030")){
//            if(xml.pushTag("multi")){
//                for(int i=0; i<multi_client_ids.size(); i++){
//                    xml.setValue("id", multi_client_ids[i], i);
//                }
//            }
//        }
//    }
//#endif // __MULTI_CLIENT_ENABLED__
//    xml.saveFile(path);
//}
