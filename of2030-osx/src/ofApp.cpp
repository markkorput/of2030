#include "ofApp.h"

#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    loadSettings();
    
    m_oscReceiver.configure(2030);
    m_oscReceiver.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    m_oscReceiver.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

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

