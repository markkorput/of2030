#include "ofApp.h"

#include "interface.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogToFile("log.txt", true);
    loadSettings();

    m_clientInfo = of2030::ClientInfo::instance();
    m_clientInfo->setup();

    m_oscReceiver.configure(2030);
    m_oscReceiver.setup();

    ofAddListener(of2030::Interface::instance()->changes_collection.modelAddedEvent, this, &ofApp::onNewChangeModel);

    m_player = of2030::Player::instance();
    m_player->start();

    // the InterfacePlayerBridge class auto-initializes with the
    // interface and player singleton instances
    m_interface_player_bridge.start();

    m_multiClient.load(m_xmlSettings);
    m_multiClient.setup();
    
    m_renderer.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    m_oscReceiver.update();
    m_player->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(m_multiClient.enabled){
        m_multiClient.draw();
    } else {
        m_renderer.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
    // saveSettings();
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


//--------------------------------------------------------------
void ofApp::loadSettings(){
    m_xmlSettings.load();
    m_oscReceiver.configure(m_xmlSettings.osc_port);
}

void ofApp::saveSettings(){
    m_xmlSettings.osc_port = (int)m_oscReceiver.getPort();
    m_xmlSettings.save();
}

void ofApp::onNewChangeModel(CMS::Model &model){
    // ofLog() << "Got new change model with " << model.attributes().size() << " attributes";
    
    for(map<string,string>::iterator it = model.attributes().begin(); it != model.attributes().end(); ++it) {
        ofLog() << " - " << it->first << ": " << it->second;
    }
}

