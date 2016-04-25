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
    m_client_cache_file.setPath("client.cache.json");

    client_id = ofToInt(m_client_cache_file.getValue("client_id"));
    client_count = m_config_file.getClientCount();

    const vector<string> ids = m_config_file.getClientIds();
    client_index = 0;
    for (int i=0; i<ids.size(); i++){
        if(ofToInt(ids[i]) == client_id){
            client_index = i;
            break;
        }
    }

    ofLog() << "client id: " << client_id;
    ofLog() << "client count: " << m_config_file.getClientCount();
}