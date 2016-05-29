//
//  multi_client.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "multi_client.hpp"
#ifdef __MULTI_CLIENT_ENABLED__

#include "xml_configs.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(MultiClient)
XmlSettings* MultiClient::xml_settings = NULL;


void MultiClient::setup(){
    xml_settings = XmlSettings::instance();
    enabled = xml_settings->multi_client_ids.size() > 0;

    // done?
    if(!enabled) return;

    int existing_renderers_count = m_renderers.size();
    Renderer* renderer;

    for(int i=0; i<xml_settings->multi_client_ids.size(); i++){

        if(i < existing_renderers_count){
            // recycle existing instances
            renderer = m_renderers[i];
        } else {
            renderer = new Renderer();
            m_renderers.push_back(renderer);
        }

        renderer->setClientId(xml_settings->multi_client_ids[i]);
        renderer->setup();
    }

    // remove existing renderers that are no longer needed
    for(int i=xml_settings->multi_client_ids.size(); i<existing_renderers_count; i++){
        renderer = m_renderers.back();
        delete renderer;
    }

    ofSetWindowPosition(0,0);
    ofLog() << "[MultiClient] enabled";
}

void MultiClient::destroy(){
    for(int i=0; i<m_renderers.size(); i++){
        delete m_renderers[i];
    }

    m_renderers.clear();
}


void MultiClient::draw(){
    ofClear(0);

    cam.begin();
    ofPushMatrix();
        ofScale(xml_settings->multi_scale, xml_settings->multi_scale, xml_settings->multi_scale);
        ofTranslate(xml_settings->multi_translate);

        drawFloor();

//        if(xml_settings->multi_debug)
//            drawDebug();

        drawScreens();

    ofPopMatrix();
    cam.end();

    if(previewClient >= 0){
        Renderer* r = m_renderers[previewClient];
        r->draw();
    }
}

void MultiClient::drawFloor(){
    ofPushMatrix();
    ofTranslate(0.0f, xml_settings->room_size.y*-0.5, 0.0f);
    ofRotateX(90.0f);
    ofSetColor(30, 30, 30);
    ofDrawRectangle(xml_settings->room_size.x*-0.5f,
                    xml_settings->room_size.z*-0.5f,
                    xml_settings->room_size.x,
                    xml_settings->room_size.z);
    ofPopMatrix();
}

void MultiClient::drawScreens(){
    XmlConfigs* screens = XmlConfigs::screens();

    XmlItemSetting* screen_setting;

    for(auto renderer: m_renderers){
        screen_setting = screens->getItem(renderer->clientId());

        if(screen_setting == NULL){
            ofLogWarning() << "not screen setting found for screen ID: " << renderer->clientId();
            continue;
        }

        ofVec3f scrpos = screen_setting->getValue("pos", ofVec3f(0.0));
        ofVec3f scrrot = screen_setting->getValue("rot", ofVec3f(0.0));

        ofVec2f worldSize = screen_setting->getValue("world_size", ofVec2f(2.67f, 2.0f));
        //ofVec2f renderSize = screen_setting->getValue("resolution", ofVec2f(768, 576));
        ofVec2f renderSize = renderer->getScreenSize();
         //ofGetWindowSize();

        ofPushMatrix();
            // move to screen's position
            ofTranslate(scrpos);
            // rotate according to cameras orientation
            ofRotateX(scrrot.x);
            ofRotateY(scrrot.y);
            ofRotateZ(scrrot.z);

            // translate to where the topleft corner of the screen should start
            ofTranslate(worldSize*-0.5);

            ofVec2f scaler = worldSize / renderSize;
            ofScale(scaler.x, scaler.y, 1.0f);

            // NOW we can finally draw the screen
            renderer->draw();
        ofPopMatrix();
    }
}
#endif // #ifdef __MULTI_CLIENT_ENABLED__
