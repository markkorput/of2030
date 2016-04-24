#include "ofApp.h"

#include "ofxXmlSettings.h"
#include "interface.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogToFile("log.txt", true);
    loadSettings();

    m_oscReceiver.configure(2030);
    m_oscReceiver.setup();

    ofAddListener(of2030::Interface::instance()->changes_collection.modelAddedEvent, this, &ofApp::onNewChangeModel);

    m_player = of2030::Player::instance();
    m_player->start();

    // the InterfacePlayerBridge class auto-initializes with the
    // interface and player singleton instances
    m_interface_player_bridge.start();

    m_config_file.setPath("config.json");
    m_client_cache_file.setPath("client.cache.json");
    ofLog() << "client id: " << m_client_cache_file.getValue("client_id");
    ofLog() << "client counnt: " << m_config_file.getClientCount();
}

//--------------------------------------------------------------
void ofApp::update(){
    m_oscReceiver.update();
    m_player->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    m_renderer.draw();
}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
    saveSettings();
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
    ofxXmlSettings settings;
    settings.loadFile("settings.xml");
    unsigned int port = settings.getValue("of2030:osc_port", 2030);
    m_oscReceiver.configure(port);
}

void ofApp::saveSettings(){
    ofxXmlSettings settings;
    settings.setValue("of2030:osc_port", (int)m_oscReceiver.getPort());
    settings.saveFile("settings.xml");
}

void ofApp::onNewChangeModel(CMS::Model &model){
    // ofLog() << "Got new change model with " << model.attributes().size() << " attributes";
    
    for(map<string,string>::iterator it = model.attributes().begin(); it != model.attributes().end(); ++it) {
        ofLog() << " - " << it->first << ": " << it->second;
    }
}

