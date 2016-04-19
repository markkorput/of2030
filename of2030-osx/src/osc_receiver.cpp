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

            if(m.getNumArgs() < 1){
                ofLogWarning() << "/change message didn't have any args";
                continue;
            }

            if(m.getArgType(0) != OFXOSC_TYPE_STRING){
                ofLogWarning() << "/change didn't have string arg";
                continue;
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

