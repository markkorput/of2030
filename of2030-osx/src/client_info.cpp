//
//  client_info.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "client_info.hpp"
#include "xml_settings.hpp"

using namespace of2030;

ClientInfo* ClientInfo::singleton = NULL;

ClientInfo* ClientInfo::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new ClientInfo();
    }
    return singleton;
}

ClientInfo::ClientInfo() : id(""), count(1), index(0){
}

void ClientInfo::setup(){
    XmlSettings* xml_settings = XmlSettings::instance();

    // setClientId(ofToInt(m_client_cache_file.getValue("client_id")));
    setClientId(xml_settings->client_id);

    ofLogVerbose() << "[ClientInfo.setup] client id: " << id;
}

void ClientInfo::copy(ClientInfo &other){
    count = other.count;
    id = other.id;
    index = other.index;
}

void ClientInfo::setClientId(string cid){
    id = cid;
}