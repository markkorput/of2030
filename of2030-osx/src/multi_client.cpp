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

XmlSettings* MultiClient::xml_settings = NULL;
MultiClient* MultiClient::singleton = NULL;

MultiClient* MultiClient::instance(){
    if (!singleton){
        singleton = new MultiClient();
    }
    return singleton;
}


void MultiClient::setup(){
    destroy();

    xml_settings = XmlSettings::instance();
    enabled = xml_settings->multi_client_ids.size() > 0;

    // done
    if(!enabled) return;

    for(int i=0; i<xml_settings->multi_client_ids.size(); i++){
        string id = xml_settings->multi_client_ids[i];

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
        ofScale(xml_settings->multi_scale, xml_settings->multi_scale, xml_settings->multi_scale);
        ofTranslate(xml_settings->multi_translate);
    

        drawFloor();
    
        if(xml_settings->multi_debug)
            drawDebug();

        drawScreens();

    ofPopMatrix();
    cam.end();
}

void MultiClient::drawFloor(){
    ofPushStyle();
    ofPushMatrix();
    ofRotateX(90.0f);
    ofSetColor(30, 30, 30);
    ofDrawRectangle(xml_settings->room_size.x*-0.5f,
                    xml_settings->room_size.z*-0.5f,
                    xml_settings->room_size.x,
                    xml_settings->room_size.z);
    ofPopMatrix();
    ofPopStyle();
}

void MultiClient::drawDebug(){
    XmlConfigs* screens = XmlConfigs::screens();

    ofPushStyle();
    ofSetColor(200, 0, 0);
    
    ofCamera screen_cam;
    XmlItemSetting* screen_setting;

    for(auto &renderer: m_renderers){
        screen_setting = screens->getItem(renderer->client_info->id);
        if(screen_setting == NULL){
            ofLogWarning() << "not screen setting found for screen ID: " << renderer->client_info->id;
            continue;
        }
        screen_cam.setPosition(screen_setting->getValue("cam_pos_x", 0.0f),
                               screen_setting->getValue("cam_pos_y", 0.0f),
                               screen_setting->getValue("cam_pos_z", 0.0f));
        screen_cam.lookAt(ofVec3f(screen_setting->getValue("cam_look_at_x", 0.0f),
                                  screen_setting->getValue("cam_look_at_y", 0.0f),
                                  screen_setting->getValue("cam_look_at_z", 1.0f)));

        ofPushMatrix();
            ofTranslate(screen_cam.getPosition());
            ofVec3f rot = screen_cam.getOrientationEuler();

            ofRotateX(rot.x);
            ofRotateY(rot.y);
            ofRotateZ(rot.z);
        
            ofTranslate(0.0f, 0.0f, -4.5f);
            ofDrawRectangle(0.0f, 0.0f, 1.0f, 1.0f);
        ofPopMatrix();
    }
    ofPopStyle();
    
}

void MultiClient::drawScreens(){
    // ofScale(xml_settings->multi_room_scale.x, xml_settings->multi_room_scale.y, xml_settings->multi_room_scale.z);
    
//    XmlConfigs* screens = XmlConfigs::screens();
    

//    for(auto &renderer: m_renderers){
//        // ClientSetting* c = renderer->client_info->getClientSetting();
//
//        //            ofPushMatrix();
//        //                ofTranslate(c->screenpos);
//        //                ofRotateX(c->screenrot.x);
//        //                ofRotateY(c->screenrot.y);
//        //                ofRotateZ(c->screenrot.z);
//        //                ofScale(1/renderer->fbo->getWidth(), 1/renderer->fbo->getHeight(), 1.0f);
//        //                ofScale(c->screensize.x, c->screensize.y, 1.0f);
//
//        //            ofPopMatrix();
//        renderer->draw();
//    }

}
#endif // #ifdef __MULTI_CLIENT_ENABLED__