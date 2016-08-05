//
//  osc_sender.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-11.
//
//

#include "sender.hpp"

using namespace OscToolkit;

OSC_TOOLKIT_SINGLETON_INLINE_IMPLEMENTATION_CODE(Sender)

void Sender::setup(string host, int port){
    // make sure it has loaded it content from the settings file
    // (won't reload when already loaded)
    if(host == "" || port < 1){
        bEnabled = false;
        return;
    }

    sender.setup(host, port);
    ofLog() << "OscSender initialized to send to " << host << ", port: " << port;
    bEnabled = true;
}

void Sender::process(ofxOscMessage &message){
    sender.sendMessage(message);
    Interface::process(message);
}