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

ClientInfo::ClientInfo() : id(-1){
}

void ClientInfo::setup(){
    m_config_file.setPath("config.json");
    // m_client_cache_file.setPath("client.cache.json");
    m_config_file.load();
    m_xml_settings.load();

    count = m_config_file.getClientCount();
    // setClientId(ofToInt(m_client_cache_file.getValue("client_id")));
    setClientId(m_xml_settings.client_id);

    ofLogVerbose() << "[ClientInfo.setup] client id: " << id;
    ofLogVerbose() << "[ClientInfo.setup] client count: " << m_config_file.getClientCount();
}

void ClientInfo::copy(ClientInfo &other){
    count = other.count;
    id = other.id;
    index = other.index;
}

void ClientInfo::setClientId(int cid){
    id = cid;
    updateClientIndex();
}

void ClientInfo::updateClientIndex(){
    int count = m_config_file.getClientCount();

    index = 0;
    for (int i=0; i<count; i++){
        if(ofToInt(m_config_file.getClientId(i)) == id){
            index = i;
            return;
        }
    }
}