//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "osc_receiver.hpp"

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

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);

        
        // "/change"-type message?
        if(m.getAddress() == "/change"){
             ofLog() << "Got change OSC Message";
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

