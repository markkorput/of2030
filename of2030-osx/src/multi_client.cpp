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

XmlSettings* MultiClient::xml_settings = NULL;

void MultiClient::setup(){
    destroy();

    xml_settings = XmlSettings::instance();
    enabled = xml_settings->multi_client_ids.size() > 0;

    // done
    if(!enabled) return;

    for(int i=0; i<xml_settings->multi_client_ids.size(); i++){
        int id = xml_settings->multi_client_ids[i];

        // create client info instance
        ClientInfo *cinfo = new ClientInfo();
        cinfo->setup();
        // cinfo->copy(*ClientInfo::instance());
        cinfo->setClientId(xml_settings->multi_client_ids[i]);
        m_client_infos.push_back(cinfo);

        // create renderer instance
        Renderer* renderer = new Renderer();
        renderer->client_info = cinfo;
        renderer->setup();
        m_renderers.push_back(renderer);
    }

    ofLog() << "[MultiClient] enabled";
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
    ofClear(0);
    cam.begin();

    ofPushMatrix();
        ofScale(xml_settings->multi_room_scale.x, xml_settings->multi_room_scale.y, xml_settings->multi_room_scale.z);

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