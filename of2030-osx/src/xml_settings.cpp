
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
    
    multi_client_ids.clear();
    if(xml.pushTag("of2030")){
        if(xml.pushTag("multi")){

            int count = xml.getNumTags("id");
            for(int i=0; i<count; i++){
                int id = xml.getValue("id", 0, i);
                multi_client_ids.push_back(id);
            }
        }
    }

}

void XmlSettings::save(){
    ofxXmlSettings xml;
    xml.setValue("of2030:osc_port", osc_port);
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
