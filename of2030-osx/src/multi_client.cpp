//
//  multi_client.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//


#include "multi_client.hpp"

using namespace of2030;

void MultiClient::load(XmlSettings &xml){
    m_xml = &xml;
    enabled = false;
    if(m_xml->multi_client_ids.size() > 0){
        enabled = true;
    }
}

void MultiClient::setup(){
    if(!enabled) return;

    for(int i=0; i<m_xml->multi_client_ids.size(); i++){
        int id = m_xml->multi_client_ids[i];
        
        ClientInfo *cinfo = new ClientInfo();
        cinfo->setup();
        // cinfo->copy(*ClientInfo::instance());
        cinfo->setClientId(m_xml->multi_client_ids[i]);
        m_client_infos.push_back(cinfo);
        
        Renderer* renderer = new Renderer();
        renderer->m_client_info = cinfo;
        renderer->setup();
        m_renderers.push_back(renderer);
    }
    
    ofLog() << "[MultiClient] enabled, resizing window";
    ofSetWindowShape(m_renderers.size()*Renderer::WIDTH, Renderer::HEIGHT);
}

void MultiClient::destroy(){
    for(int i=0; i<m_client_infos.size(); i++){
        delete m_client_infos[i];
    }

    for(int i=0; i<m_renderers.size(); i++){
        delete m_renderers[i];
    }
    
    m_client_infos.clear();
    m_renderers.clear();
}


void MultiClient::draw(){
    if(!enabled) return;

    ofPushMatrix();
    for(int i=0; i<m_renderers.size(); i++){
        m_renderers[i]->draw();
        ofTranslate(Renderer::WIDTH, 0);
    }
    ofPopMatrix();

}

ofPoint MultiClient::getTotalSize(){
}