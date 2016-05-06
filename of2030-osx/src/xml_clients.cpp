#include "xml_clients.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

// two local methods

void xmlLoadVec3f(ofxXmlSettings &xml, ofVec3f &vec3f){
    vec3f = ofVec3f(xml.getValue("x", 0.0f), xml.getValue("y", 0.0f), xml.getValue("z", 0.0f));
}

void xmlLoadClient(ofxXmlSettings &xml, XmlClient &client){
    client.id = xml.getValue("id", 1);
    if(xml.pushTag("screenpos")){
        xmlLoadVec3f(xml, client.screenpos);
        xml.popTag();
    }
    if(xml.pushTag("screenrot")){
        xmlLoadVec3f(xml, client.screenrot);
        xml.popTag();
    }
}

// XmlClient implementation

void XmlClients::destroy(){
    for(auto &client: xml_clients)
        if(client)
            delete client;

    xml_clients.clear();
}

void XmlClients::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    if(xml.pushTag("2030")){
        if(xml.pushTag("clients")){

            XmlClient *c;
            int loaded_client_count = xml_clients.size();
            int xml_count = xml.getNumTags("client");
            
            // looop over each client node in the xml
            for(int i=0; i<xml_count; i++){
                if(xml.pushTag("client", i)){
            
                    // allocate new instance or use previsouly allocated?
                    if(i >= loaded_client_count){
                        // new instance
                        c = new XmlClient();
                        // add to list
                        xml_clients.push_back(c);
                        // increase our loaded count
                        loaded_client_count++;
                    } else {
                        // grab existing
                        c = xml_clients[i];
                    }
                    
                    // populate our client instance
                    xmlLoadClient(xml, *c);

                    xml.popTag();
                }
            }
            
            // remove any too-many xml_clients
            while(loaded_client_count > xml_count){
                c = xml_clients.back();
                delete c;
                xml_clients.pop_back();
                loaded_client_count--;
            }


        }
    }
}

//void XmlClients::save(){
//    ofxXmlSettings xml;
//    xml.saveFile(path);
//}
