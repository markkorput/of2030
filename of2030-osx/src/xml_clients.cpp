#include "xml_clients.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

// two local methods

void xmlLoadVec2f(ofxXmlSettings &xml, ofVec2f &vec2f){
    vec2f = ofVec2f(xml.getValue("x", 0.0f), xml.getValue("y", 0.0f));
}

void xmlLoadVec3f(ofxXmlSettings &xml, ofVec3f &vec3f){
    vec3f = ofVec3f(xml.getValue("x", 0.0f), xml.getValue("y", 0.0f), xml.getValue("z", 0.0f));
}

void xmlLoadClient(ofxXmlSettings &xml, ClientSetting &client){
    client.id = xml.getValue("id", 1);
    if(xml.pushTag("screenpos")){
        xmlLoadVec3f(xml, client.screenpos);
        xml.popTag();
    }
    if(xml.pushTag("screensize")){
        xmlLoadVec2f(xml, client.screensize);
        xml.popTag();
    }
    if(xml.pushTag("screenrot")){
        xmlLoadVec3f(xml, client.screenrot);
        xml.popTag();
    }
    
    if(xml.pushTag("pano")){
        client.pano_start = xml.getValue("start", 0.0);
        client.pano_end = xml.getValue("end", 1.0);

        if(xml.pushTag("rot")){
            xmlLoadVec3f(xml, client.pano_rot);
            xml.popTag();
        }
        xml.popTag();
    }
}

// XmlClients implementation


XmlClients* XmlClients::singleton = NULL;

XmlClients* XmlClients::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new XmlClients();
    }
    return singleton;
}


void XmlClients::destroy(){
    for(auto &client: clients)
        if(client)
            delete client;

    clients.clear();
}

void XmlClients::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    if(xml.pushTag("of2030")){
        if(xml.pushTag("clients")){

            ClientSetting *c;
            int loaded_client_count = clients.size();
            int xml_count = xml.getNumTags("client");
            
            // looop over each client node in the xml
            for(int i=0; i<xml_count; i++){
                if(xml.pushTag("client", i)){
            
                    // allocate new instance or use previsouly allocated?
                    if(i >= loaded_client_count){
                        // new instance
                        c = new ClientSetting();
                        // add to list
                        clients.push_back(c);
                        // increase our loaded count
                        loaded_client_count++;
                    } else {
                        // grab existing
                        c = clients[i];
                    }
                    
                    // populate our client instance
                    xmlLoadClient(xml, *c);

                    xml.popTag();
                }
            }
            
            // remove any too-many clients
            while(loaded_client_count > xml_count){
                c = clients.back();
                delete c;
                clients.pop_back();
                loaded_client_count--;
            }


        }
    }
}

//void XmlClients::save(){
//    ofxXmlSettings xml;
//    xml.saveFile(path);
//}
