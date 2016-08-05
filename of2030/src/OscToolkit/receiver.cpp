//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "receiver.hpp"

using namespace OscToolkit;

SINGLETON_INLINE_IMPLEMENTATION_CODE(Receiver)

Receiver::Receiver() : bConnected(false){
    osc_port = 2030;
}

void Receiver::setPort(int port){
    if(port == osc_port)
        return; // no change

    int old_port = osc_port;
    osc_port = port;

    if(bConnected){
        if(osc_port == 0){
            disconnect();
        } else {
            connect();
        }
    }
}

void Receiver::setup(){
    if(!bConnected && osc_port != 0){
        connect();
    }
}

void Receiver::update(){
    ofxOscMessage m;
    int message_count = 0;

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages() && message_count < MAX_MESSAGES_PER_CYCLE){
        // get the next message
        m_oscReceiver.getNextMessage(m);
        process(m);
        message_count++;
    }
}

void Receiver::destroy(){
    if(bConnected){
        disconnect();
    }
}

bool Receiver::connect(){
    // m_oscReceiver.enableReuse();

#ifdef __BOOL_OSC_SETUP__
    if(m_oscReceiver.setup(osc_port)){
        bConnected = true;
        ofLog() << "of2030::OscReceiver listening to port: " << osc_port;
        return true;
    }

    ofLogWarning() << "OscReceiver could not start listening to port: " << osc_port;
    return false;
#else
    m_oscReceiver.setup(osc_port);
    bConnected = true;
    ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
    return true;
#endif // __BOOL_OSC_SETUP__

}

void Receiver::disconnect(){
    // (private method) m_oscReceiver.shutdown();
    bConnected = false;
}

