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

ClientInfo::ClientInfo() : client_id(-1){
}

void ClientInfo::setup(){
    m_config_file.setPath("config.json");
    // m_client_cache_file.setPath("client.cache.json");
    m_config_file.load();
    m_xml_settings.load();

    client_count = m_config_file.getClientCount();
    // setClientId(ofToInt(m_client_cache_file.getValue("client_id")));
    setClientId(m_xml_settings.client_id);

    ofLogVerbose() << "[ClientInfo.setup] client id: " << client_id;
    ofLogVerbose() << "[ClientInfo.setup] client count: " << m_config_file.getClientCount();
}

void ClientInfo::copy(ClientInfo &other){
    client_count = other.client_count;
    client_id = other.client_id;
    client_index = other.client_index;
}

void ClientInfo::setClientId(int id){
    client_id = id;
    updateClientIndex();
}

void ClientInfo::updateClientIndex(){
    int count = m_config_file.getClientCount();

    client_index = 0;
    for (int i=0; i<count; i++){
        string id = m_config_file.getClientId(i);
        if(ofToInt(id) == client_id){
            client_index = i;
            break;
        }
    }

}