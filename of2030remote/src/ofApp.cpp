#include "ofApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowPosition(0,0);

    ofLog() << "loading settings.xml";
    ofxXmlSettings settings;
    if(!settings.loadFile("settings.xml")){
        ofLogWarning() << "failed to load settings.xml, loading default values";
    }
    
    osc_host = settings.getValue("of2030:remote:osc:host", "localhost");
    osc_port = settings.getValue("of2030:remote:osc:port", 2030);
    min_value = settings.getValue("of2030:remote:value:min", 0.0f);
    max_value = settings.getValue("of2030:remote:value:max", 1.0f);
    message = settings.getValue("of2030:remote:osc:message", "/value");
    for(int i=1; settings.tagExists("of2030:remote:guide"+ofToString(i)); i++){
        guide_values.push_back(settings.getValue("of2030:remote:guide"+ofToString(i), 0.0f));
    }
    preview_width = settings.getValue("of2030:remote:preview_width", 0.5f);

    ofLog() << "settings:";
    ofLog() << "osc host/port: " << osc_host << "/" << osc_port;
    ofLog() << "value range: " << min_value << "," << max_value;
    ofLog() << "message: " << message;
    for(auto guide_value: guide_values){
        ofLog() << "guide: " << guide_value;
    }
    ofLog() << "preview width: " << preview_width;

    osc_sender.setup(osc_host, osc_port);
    ofLog() << "osc client connect to: " << osc_host << " at port:: " << osc_port;

    current_value = ofLerp(min_value, max_value, 0.5);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(50,0,0);
    ofDrawRectangle(ofMap(current_value, min_value, max_value, 0, ofGetWidth()),
                    0,
                    ofMap(preview_width, 0, max_value-min_value, 0, ofGetWidth()),
                    ofGetHeight());
    
    ofSetColor(255,255,255,100);
    for(auto guide_value: guide_values){
        ofDrawRectangle(ofMap(guide_value, min_value, max_value, 0.0f, ofGetWidth()), 0.0f, 1, ofGetHeight());
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'i')
        updateValue(current_value-0.1);
    if(key == 'j')
        updateValue(current_value-0.03);
    if(key == 'b')
        updateValue(current_value-0.01);
    
    if(key == 'p')
        updateValue(current_value+0.1);
    if(key == 'l')
        updateValue(current_value+0.03);
    if(key == 'm')
        updateValue(current_value+0.01);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    updateValue(ofMap(x, 0, ofGetWidth(), min_value, max_value));
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

void ofApp::updateValue(float val){
    current_value = val;
    sendValue(current_value);
    ofLogVerbose() << "val: " << current_value;
}

void ofApp::sendValue(float val){
    osc_message.setAddress(message);
    osc_message.addFloatArg(val);
    osc_sender.sendMessage(osc_message);
    osc_message.clear();
}