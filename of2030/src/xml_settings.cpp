
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

SINGLETON_INLINE_IMPLEMENTATION_CODE(XmlSettings)


//
// local methods
//

//void loadOscAddresses(TiXmlDocument &doc, OscSetting &osc_setting){
//    TiXmlElement *el = doc.FirstChildElement("of2030");
//    if(!el) return;
//
//    el = el->FirstChildElement("osc");
//    if(!el) return;
//
//    el = el->FirstChildElement("addresses");
//    if(!el) return;
//
//    TiXmlElement* child = el->FirstChildElement();
//    string name, val;
//
//    while(child){
//        name = child->ValueStr();
//        val = child->GetText();
//        osc_setting.addresses[name] = val;
//        // ofLogVerbose() << "XmlSettings::loadOsc found OSC-address: " << name << ":" << val;
//        child = child->NextSiblingElement();
//    }
//}

//
// XmlSettings
//

bool XmlSettings::load(bool reload){
    ofLogVerbose() << "XmlSettings::load";

    if(loaded && !reload) return false;

    ofxXmlSettings xml;

    // load client id from separate file?
    if(client_id_path != "" && xml.loadFile(client_id_path)){
        client_id = xml.getValue("of2030:client_id", "rpi1");
        ofLog() << "Trying to load settings.xml from: " << ofToDataPath(path, true);
        if(!xml.loadFile(path)){
            return false;
        }
    } else {
        // load client id from main settings file
        ofLog() << "Trying to load settings.xml from: " << ofToDataPath(path, true);
        if(!xml.loadFile(path)){
            return false;
        }
        client_id = xml.getValue("of2030:client_id", "rpi1");
    }
    
    ofLog() << "client id: " << client_id;

    // logging
    log_level_name = xml.getValue("of2030:app:log_level", "");
    if(log_level_name == "verbose") log_level = OF_LOG_VERBOSE;
    else if(log_level_name == "silent") log_level = OF_LOG_SILENT;
    else if(log_level_name == "warning") log_level = OF_LOG_WARNING;
    else log_level = OF_LOG_NOTICE; // if(log_level_name == "notice")
    ofLogVerbose() << "log_level: " << log_level_name;
    log_alive_interval = xml.getValue("of2030:app:log_alive_interval", 100.0f);

    // osc
    osc_setting.port = xml.getValue("of2030:osc:port", 0);
    ofLogVerbose() << "OSC port: " << osc_setting.port;

#ifdef __OSC_SENDER_ENABLED__
    osc_out_port = xml.getValue("of2030:osc:out:port", 0);
    osc_out_host = xml.getValue("of2030:osc:out:host", "");
    osc_out_keycheck = xml.getValue("of2030:osc:out:key_check", "1") == "1";
#endif // __OSC_SENDER_ENABLED__

    // loadOscAddresses(xml.doc, osc_setting);

    room_size = ofVec3f(xml.getValue("of2030:room_size_x", 1.0f),
                        xml.getValue("of2030:room_size_y", 1.0f),
                        xml.getValue("of2030:room_size_z", 1.0f));
    
    rgbaVidPixels = xml.getValue("of2030:rgba_pixels", "0") == "1";
    drawToFboFirst = xml.getValue("of2030:draw_to_fbo_first", "0") == "1";
    cache_screen_configs = xml.getValue("of2030:cache_screen_configs", "0") == "1";

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
    return true;
}
