//
//  osc_sender.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-11.
//
//

#include "osc_sender.hpp"

#ifdef __OSC_SENDER_ENABLED__ // to speed op raspi compile

#include "xml_settings.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(OscSender)

void OscSender::setup(string host, int port){
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

//void OscSender::destroy(){
//}

#endif // __OSC_SENDER_ENABLED__