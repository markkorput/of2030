//
//  client_info.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "client_info.hpp"

using namespace of2030;

ClientInfo* ClientInfo::singleton = NULL;

ClientInfo* ClientInfo::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new ClientInfo();
    }
    return singleton;
}

ClientInfo::ClientInfo() : id(-1), count(1), index(0){
}

void ClientInfo::setup(){
    m_xml_settings.load();
    m_xml_clients.load();

    // setClientId(ofToInt(m_client_cache_file.getValue("client_id")));
    setClientId(m_xml_settings.client_id);

    ofLogVerbose() << "[ClientInfo.setup] client id: " << id;
}

void ClientInfo::copy(ClientInfo &other){
    count = other.count;
    id = other.id;
    index = other.index;
}

void ClientInfo::setClientId(int cid){
    id = cid;
    // updateClientIndex();
}

XmlClient* ClientInfo::getXmlClient(){
    for(auto &xml_client: m_xml_clients.xml_clients){
        if(xml_client->id == id){
            return xml_client;
        }
    }
    
    return NULL;
}

//void ClientInfo::updateClientIndex(){
//    index = 0;
//}