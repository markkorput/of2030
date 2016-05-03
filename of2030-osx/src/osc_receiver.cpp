//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "osc_receiver.hpp"
#include "ofxJSONElement.h"

using namespace of2030;

void OscReceiver::configure(unsigned int port, Interface* interface){
    m_port = port;

    if(interface)
        m_interface = interface;

    if(m_bConnected){
        // reconnect
        connect();
    }
}

void OscReceiver::setup(){
    if(m_interface == NULL){
        m_interface = Interface::instance();
    }

    if(!m_bConnected){
        connect();
    }
}

void OscReceiver::update(){
    int message_count = 0;

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages() && message_count < MAX_MESSAGES_PER_CYCLE){
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);
        message_count++;
        
        // "/change"-type message?
        if(m.getAddress() == "/change"){
            ofLogVerbose() << "Got /change OSC Message";
            processChangeMessage(m);
            continue;
        }

        if(m.getAddress() == "/effect"){
            ofLogVerbose() << "Got /effect OSC Message";
            processEffectMessage(m);
            continue;
        }

        if(m.getAddress() == "/message"){
            ofLogVerbose() << "Got /message OSC Message";
            processMessageMessage(m);
            continue;
        }

        ofLog() << "Got unknown OSC Message " << m.getAddress();
    }
}

void OscReceiver::destroy(){
    if(m_bConnected){
        disconnect();
    }
}

bool OscReceiver::connect(){
    m_oscReceiver.enableReuse();

    if(m_oscReceiver.setup(m_port)){
        m_bConnected = true;
        ofLog() << "of2030::OscReceiver listening to port: " << m_port;
        return true;
    }

    ofLogWarning() << "OscReceiver could not start listening to port: " << m_port;
    return false;
}

void OscReceiver::disconnect(){
    m_bConnected = false;
}

void OscReceiver::processChangeMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/change message didn't have any args";
        return;
    }

    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/change didn't have string arg";
        return;
    }

    // parse json
    map<string, string> data;
    getMapFromJsonString(m.getArgAsString(0), data);

    // create change model with extracted data
    CMS::Model* change_model = new CMS::Model();
    change_model->set(data);
    m_interface->changes_collection.add(change_model);
}

void OscReceiver::processEffectMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/effect message didn't have any args";
        return;
    }

    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/effect didn't have string arg";
        return;
    }

    effects::Effect* effect = createEffectFromJsonString(m.getArgAsString(0));
    if(effect){
        // ofLog() << "[OscReceiver] Triggering interface's effectEvent";
        ofNotifyEvent(m_interface->effectEvent, *effect, m_interface);
        return;
    }
    
    ofLogWarning() << "Could not create effect instance from OSC /effect message with data: " << m.getArgAsString(0);
}

void OscReceiver::getMapFromJsonString(const std::string &str, map<string, string> &target){
    ofxJSONElement jsonEl;
    jsonEl.parse(str);

    // get all attributes as string (no support for nester structure for now)
    vector<string> attrs = jsonEl.getMemberNames();

    for(int i=attrs.size()-1; i>=0; i--){
        string name = attrs[i];
        target[name] = jsonEl[name].asString();
    }
}

effects::Effect* OscReceiver::createEffectFromJsonString(const std::string &json_string){
    effects::Effect* pEffect;
    std::string type, value;
    
    ofxJSONElement json;
    json.parse(json_string);

    if(json.isMember("type")){
        type = json["type"].asString();
    }

    // create an instance of the appropriate effects class
    // beased on the model's type attribute
    // and populate type-specific attributes
    // the assign the pEffect pointer to the created instance
    if(type == "OFF"){
        
        effects::Off* effect = new effects::Off();
        pEffect = (effects::Effect*)effect;
        
    } else if(type == "COLOR"){
        
        effects::Color* effect = new effects::Color();
        if(json.isMember("r") && json.isMember("g") && json.isMember("b")){
            effect->color.set(ofColor(json["r"].asInt(),
                                      json["g"].asInt(),
                                      json["b"].asInt()));
        }
        pEffect = (effects::Effect*)effect;

    } else {

        ofLogWarning() << "[EffectCreator] got unknown effect model type: " << type;
        pEffect = new effects::Effect();
        
    }

    // process some more (optional) general effect attributes
    if(json.isMember("start"))
        pEffect->startTime = json["start"].asFloat();

    if(json.isMember("end"))
        pEffect->endTime = json["end"].asFloat();

    if(json.isMember("duration"))
        pEffect->duration = json["duration"].asFloat();

    return pEffect;
}

void OscReceiver::processMessageMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/message message didn't have any args";
        return;
    }
    
    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/effect didn't have string arg";
        return;
    }
    
    string messageType = m.getArgAsString(0);
    
    if(messageType == "cursor"){
        effects::Cursor* cursor_effect = new effects::Cursor();
        ofNotifyEvent(m_interface->effectEvent, (*(effects::Effect*)cursor_effect), m_interface);
        return;
    }
    
    if(messageType == "stars"){
        effects::Stars* effect = new effects::Stars();
        ofNotifyEvent(m_interface->effectEvent, (*(effects::Effect*)effect), m_interface);
        // ofLog() << "stars effect";
        return;
    }

    ofLogWarning() << "Unknown messageType" << messageType;
}