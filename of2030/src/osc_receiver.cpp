//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "osc_receiver.hpp"
#include "setting_types.h"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(OscReceiver)

OscReceiver::OscReceiver() : m_interface(NULL), bConnected(false){
    default_setting.port = 2030;
    osc_setting = &default_setting;
}

void OscReceiver::configure(OscSetting &_osc_setting){
    this->osc_setting = &_osc_setting;

    if(bConnected){
        if(_osc_setting.port == 0){
            disconnect();
        } else { // reconnect
            connect();
        }
    }
}

void OscReceiver::setup(){
    if(m_interface == NULL){
        m_interface = OscInterface::instance();
    }

    if(!bConnected && osc_setting->port != 0){
        connect();
    }
}

void OscReceiver::update(){
    ofxOscMessage m;
    int message_count = 0;

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages() && message_count < MAX_MESSAGES_PER_CYCLE){
        // get the next message
        m_oscReceiver.getNextMessage(m);
        m_interface->process(m);
        message_count++;
        // ofNotifyEvent(m_interface->receiveEvent, m, m_interface);
        // processMessage(m);
    }
}

void OscReceiver::destroy(){
    if(bConnected){
        disconnect();
    }
}

bool OscReceiver::connect(){
    // m_oscReceiver.enableReuse();

#ifdef __BOOL_OSC_SETUP__
    if(m_oscReceiver.setup(osc_setting->port)){
        bConnected = true;
        ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
        return true;
    }

    ofLogWarning() << "OscReceiver could not start listening to port: " << osc_setting->port;
    return false;
#else
    m_oscReceiver.setup(osc_setting->port);
    bConnected = true;
    ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
    return true;
#endif // __BOOL_OSC_SETUP__

}

void OscReceiver::disconnect(){
    // (private method) m_oscReceiver.shutdown();
    bConnected = false;
}

