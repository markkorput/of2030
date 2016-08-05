//
//  osc_interface.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-04.
//
//

#include "interface.hpp"

using namespace OscToolkit;

SINGLETON_INLINE_IMPLEMENTATION_CODE(Interface);

void Interface::process(ofxOscMessage &message){
    ofNotifyEvent(messageEvent, message, this);
}