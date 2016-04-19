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
            ofLog() << "Got /change OSC Message";
            processChangeMessage(m);
            continue;
        }

        if(m.getAddress() == "/effect"){
            ofLog() << "Got /effect OSC Message";
            processEffectMessage(m);
            continue;
        }

        ofLog() << "Got unknown OSC Message";
    }
}

void OscReceiver::destroy(){
    if(m_bConnected){
        disconnect();
    }
}

bool OscReceiver::connect(){
    m_oscReceiver.setup(m_port);
    m_bConnected = true;
    ofLog() << "of2030::OscReceiver listening to port: " << m_port;
    return true;
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
    ofxJSONElement jsonEl;
    jsonEl.parse(m.getArgAsString(0));
    
    // get all attributes as string (no support for nester structure for now)
    map<string, string> data;
    vector<string> attrs = jsonEl.getMemberNames();
    
    for(int i=attrs.size()-1; i>=0; i--){
        string name = attrs[i];
        data[name] = jsonEl[name].asString();
    }
    
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

    std::string fxType = m.getArgAsString(0);
    if(fxType == "OFF"){
        Effect *effect = new Effect();
        effect->m_type = EffectType::OFF;
        
        if(m.getNumArgs() > 1){
            effect->m_time = m.getArgAsFloat(1);
        }
        
        m_interface->effects_collection.add(effect);
        return;
    }
    
    ofLog() << "Unknown /effect message: " << fxType;
}
