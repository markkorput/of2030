#include "ofApp.h"

#include "shared2030.h"

#ifdef __MULTI_CLIENT_ENABLED__
    #include "multi_client.hpp"
#endif



#include "interface.hpp"
#include "xml_configs.hpp"
#include "xml_triggers.hpp"
#include "shader_manager.hpp"
#include "xml_settings.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogToFile("log.txt", true);
    ofSetWindowPosition(0, 0);

    of2030::XmlSettings::instance()->load();
    ofSetLogLevel(of2030::XmlSettings::instance()->log_level);

    // of2030::XmlClients::instance()->load();
    of2030::XmlConfigs::instance()->load();
    of2030::XmlConfigs::screens()->load();

    m_clientInfo = of2030::ClientInfo::instance();
    m_clientInfo->setup();

    m_oscReceiver.configure(of2030::XmlSettings::instance()->osc_setting);
    m_oscReceiver.setup();

    m_player = of2030::Player::instance();
    m_player->start();

    // the InterfacePlayerBridge class auto-initializes with the
    // interface and player singleton instances
    m_interface_player_bridge.start();

#ifdef __MULTI_CLIENT_ENABLED__
    of2030::MultiClient::instance()->setup();
#endif
    
    m_renderer.setup();
    
    ofAddListener(of2030::Interface::instance()->controlEvent, this, &ofApp::onControl);
}

//--------------------------------------------------------------
void ofApp::update(){
    m_oscReceiver.update();
    m_player->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef __MULTI_CLIENT_ENABLED__
    if(of2030::MultiClient::instance()->enabled){
        of2030::MultiClient::instance()->draw();
    } else {
        m_renderer.draw();
    }
#else
    m_renderer.draw();
#endif

}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::onControl(string &type){

    if(type == CTRL_RELOAD_SHADERS){
        of2030::ShaderManager::instance()->clear();
        return;
    }

    if(type == CTRL_RELOAD_EFFECTS){
        of2030::XmlTriggers::instance()->load();
        of2030::XmlConfigs::instance()->load();
        return;
    }

    if(type == CTRL_RELOAD_SCREENS){
        of2030::XmlConfigs::screens()->load();
#ifdef __MULTI_CLIENT_ENABLED__
        of2030::MultiClient::instance()->setup();
#endif
        return;
    }

    if(type == CTRL_RELOAD_SETTINGS){
        of2030::XmlSettings::instance()->load(true);
        ofSetLogLevel(of2030::XmlSettings::instance()->log_level);
        m_oscReceiver.configure(of2030::XmlSettings::instance()->osc_setting);
#ifdef __MULTI_CLIENT_ENABLED__
        of2030::MultiClient::instance()->setup();
#endif
        return;
    }

    if(type == CTRL_RELOAD_CLIENTS){
        ofLogWarning() << "reload clients is deprecated";
        // of2030::XmlClients::instance()->load();
        return;
    }
}
