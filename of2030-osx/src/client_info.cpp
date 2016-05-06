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
    placeholderXmlClient.id = id;
}

void ClientInfo::setup(){
    m_xml_settings.load();

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
    placeholderXmlClient.id = cid;
    // updateClientIndex();
}

ClientSetting* ClientInfo::getClient(){
    for(auto &client: XmlClients::instance()->clients){
        if(client->id == id){
            return client;
        }
    }

    return &placeholderXmlClient;
}

//void ClientInfo::updateClientIndex(){
//    index = 0;
//}