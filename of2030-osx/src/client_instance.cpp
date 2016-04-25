//
//  client_instance.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "client_instance.hpp"

using namespace of2030;

ClientInstance::ClientInstance(){
    m_renderer = NULL;
    m_bPrivateRenderer = false;
}

void ClientInstance::setup(){
    // m_clientInfo->setup();
    if(!m_renderer){
        m_renderer = new Renderer();
        m_renderer->m_client_info = &m_clientInfo;
        m_renderer->setup();
        m_bPrivateRenderer = true;
    }
}

void ClientInstance::destroy(){
    if(m_bPrivateRenderer){
        delete m_renderer;
        m_renderer = NULL;
        m_bPrivateRenderer = false;
    }
    m_renderer = NULL;
}