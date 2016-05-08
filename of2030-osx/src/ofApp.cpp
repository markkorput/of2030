#include "ofApp.h"

#include "interface.hpp"
#include "xml_clients.hpp"
#include "xml_effects.hpp"
#include "xml_triggers.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogToFile("log.txt", true);

    m_xmlSettings.load();
    ofSetLogLevel(m_xmlSettings.log_level);

    of2030::XmlClients::instance()->load();
    of2030::XmlEffects::instance()->load();

    m_clientInfo = of2030::ClientInfo::instance();
    m_clientInfo->setup();

    m_oscReceiver.configure(m_xmlSettings.osc_setting);
    m_oscReceiver.setup();

    m_player = of2030::Player::instance();
    m_player->start();

    // the InterfacePlayerBridge class auto-initializes with the
    // interface and player singleton instances
    m_interface_player_bridge.start();

#ifdef __MULTI_CLIENT_ENABLED__
    m_multiClient.load(m_xmlSettings);
    m_multiClient.setup();
#endif
    
    m_renderer.setup();
    
    ofAddListener(of2030::Interface::instance()->reconfigSettingsEvent, this, &ofApp::onReconfigSettings);
    ofAddListener(of2030::Interface::instance()->reconfigClientsEvent, this, &ofApp::onReconfigClients);
    ofAddListener(of2030::Interface::instance()->reconfigEffectsEvent, this, &ofApp::onReconfigEffects);
}

//--------------------------------------------------------------
void ofApp::update(){
    m_oscReceiver.update();
    m_player->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef __MULTI_CLIENT_ENABLED__
    if(m_multiClient.enabled){
        m_multiClient.draw();
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

void ofApp::onReconfigSettings(string &path){
    m_xmlSettings.load();
    ofSetLogLevel(m_xmlSettings.log_level);
    m_oscReceiver.configure(m_xmlSettings.osc_setting);
#ifdef __MULTI_CLIENT_ENABLED__
    m_multiClient.load(m_xmlSettings);
#endif
}

void ofApp::onReconfigClients(string &path){
    of2030::XmlEffects::instance()->load();
    of2030::XmlClients* instance = of2030::XmlClients::instance();
    if(path != "") instance->path = path;
    instance->load();
}


void ofApp::onReconfigEffects(string &path){
    of2030::XmlTriggers::instance()->load();
    of2030::XmlEffects::instance()->load();
}