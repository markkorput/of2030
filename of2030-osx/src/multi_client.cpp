//
//  multi_client.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//


#include "multi_client.hpp"
#ifdef __MULTI_CLIENT_ENABLED__

using namespace of2030;

void MultiClient::load(XmlSettings &xml){
    bool wasSetup = bSetup;
    destroy();
    m_xml = &xml;
    enabled = m_xml->multi_client_ids.size() > 0;
    if(wasSetup)
        setup();
}

void MultiClient::setup(){
    if(!enabled) return;

    for(int i=0; i<m_xml->multi_client_ids.size(); i++){
        int id = m_xml->multi_client_ids[i];

        // create client info instance
        ClientInfo *cinfo = new ClientInfo();
        cinfo->setup();
        // cinfo->copy(*ClientInfo::instance());
        cinfo->setClientId(m_xml->multi_client_ids[i]);
        m_client_infos.push_back(cinfo);

        // create renderer instance
        Renderer* renderer = new Renderer();
        renderer->client_info = cinfo;
        renderer->setup();
        m_renderers.push_back(renderer);
    }

    ofLog() << "[MultiClient] enabled, resizing window";
    //ofSetWindowShape(m_renderers.size()*Renderer::WIDTH*m_scaleFactor, Renderer::HEIGHT*m_scaleFactor);
    ofSetWindowPosition(10, 10);
    bSetup = true;
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
    bSetup = false;
}


void MultiClient::draw(){
    if(!enabled) return;

    ofClear(0);
    cam.begin();

    ofPushMatrix();
        ofScale(m_xml->multi_room_scale.x, m_xml->multi_room_scale.y, m_xml->multi_room_scale.z);

        // draw floor
        ofPushMatrix();
        ofRotateX(90.0f);
        ofPushStyle();
            ofSetColor(30, 30, 30);
            ofDrawRectangle(0.0f, 0.0f, 1.0f, 1.0f);
        ofPopStyle();
        ofPopMatrix();


        for(auto &renderer: m_renderers){
            ClientSetting* c = renderer->client_info->getClient();

            ofPushMatrix();
                ofTranslate(c->screenpos);
                ofRotateX(c->screenrot.x);
                ofRotateY(c->screenrot.y);
                ofRotateZ(c->screenrot.z);
                ofScale(1/renderer->fbo->getWidth(), 1/renderer->fbo->getHeight(), 1.0f);
                ofScale(c->screensize.x, c->screensize.y, 1.0f);
                renderer->draw();
            ofPopMatrix();
        }


    ofPopMatrix();
    cam.end();
}

#endif // #ifdef __MULTI_CLIENT_ENABLED__