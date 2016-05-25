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

SINGLETON_CLASS_IMPLEMENTATION_CODE(MultiClient)
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

    ofSetWindowShape(400,300);
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

//void MultiClient::drawDebug(){
//    XmlConfigs* screens = XmlConfigs::screens();
//
//    ofPushStyle();
//    ofSetColor(200, 0, 0);
//
//    ofCamera screen_cam;
//    XmlItemSetting* screen_setting;
//
//    for(auto &renderer: m_renderers){
//        screen_setting = screens->getItem(renderer->clientId());
//        if(screen_setting == NULL){
//            ofLogWarning() << "not screen setting found for screen ID: " << renderer->clientId();
//            continue;
//        }
//        screen_cam.setPosition(screen_setting->getValue("pos_x", 0.0f),
//                               screen_setting->getValue("pos_y", 0.0f),
//                               screen_setting->getValue("pos_z", 0.0f));
//        screen_cam.lookAt(ofVec3f(screen_setting->getValue("cam_look_at_x", 0.0f),
//                                  screen_setting->getValue("cam_look_at_y", 0.0f),
//                                  screen_setting->getValue("cam_look_at_z", 1.0f)));
//
//        ofPushMatrix();
//            ofTranslate(screen_cam.getPosition());
//            ofVec3f rot = screen_cam.getOrientationEuler();
//            ofRotateX(rot.x);
//            ofRotateY(rot.y);
//            ofRotateZ(rot.z);
//
//            ofTranslate(0.0f, 0.0f, -0.2f);
//
//            float wheight = screen_setting->getValue("world_height", 2.0f) * 1.1;
//            float wwidth = screen_setting->getValue("world_width", 2.67f) * 1.1;
//
//            ofDrawRectangle(-0.5f*wwidth,
//                            -0.5f*wheight,
//                            wwidth,
//                            wheight);
//        ofPopMatrix();
//    }
//    ofPopStyle();
//
//}

void MultiClient::drawScreens(){
    XmlConfigs* screens = XmlConfigs::screens();

    XmlItemSetting* screen_setting;

    for(auto &renderer: m_renderers){
        screen_setting = screens->getItem(renderer->clientId());

        if(screen_setting == NULL){
            ofLogWarning() << "not screen setting found for screen ID: " << renderer->clientId();
            continue;
        }

        ofVec3f scrpos = screen_setting->getValue("pos", ofVec3f(0.0));
        ofVec3f scrrot = screen_setting->getValue("rot", ofVec3f(0.0));

        float wwidth = screen_setting->getValue("world_width", 2.67f);
        float wheight = screen_setting->getValue("world_height", 2.0f);

        float pixwidth = screen_setting->getValue("pixel_width", 768);
        float pixheight = screen_setting->getValue("pixel_width", 576);

        ofPushMatrix();
            // move to screen's camera position
            ofTranslate(scrpos);
            // rotate according to cameras orientation
            ofRotateX(scrrot.x);
            ofRotateY(scrrot.y);
            ofRotateZ(scrrot.z);
            // translate 4.5 "meters" forward, we're gonna assume that's where the projection ends up
            ofTranslate(wwidth*-0.5f, wheight*-0.5f, 0.0f);
            // scale from pixel size to world size (renderer draws its fbo simply at screen size, because normally
            // it's drawing fullscreen)
            ofScale(wwidth / pixwidth, wheight / pixheight, 1.0f);
            // NOW we can finally draw the screen
            renderer->draw();
        ofPopMatrix();
    }
}
#endif // #ifdef __MULTI_CLIENT_ENABLED__
