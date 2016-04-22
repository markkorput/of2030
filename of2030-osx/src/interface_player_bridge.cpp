//
//  effect_creator.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "interface_player_bridge.hpp"
#include "effects.hpp"

using namespace of2030;

InterfacePlayerBridge::InterfacePlayerBridge(){
    setInterface(Interface::instance());
    m_player = Player::instance();
    m_bStarted = false;
}

InterfacePlayerBridge::~InterfacePlayerBridge(){
    if(m_bStarted)
        stop();
}

void InterfacePlayerBridge::setInterface(Interface *interface){
    if(m_bStarted && m_interface){
        registerInterfaceCallbacks(false);
    }

    m_interface = interface;

    if(m_bStarted && m_interface){
        registerInterfaceCallbacks(true);
    }
}

void InterfacePlayerBridge::start(){
    registerInterfaceCallbacks(true);
    m_bStarted = true;
}

void InterfacePlayerBridge::stop(){
    registerInterfaceCallbacks(false);
    m_bStarted = false;
}

void InterfacePlayerBridge::registerInterfaceCallbacks(bool _register){
    if(_register){
        // subscribe to new effect model events of specified interface
        ofAddListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
    } else {
        // unsubscribe from new effect model events of previous interface
        ofRemoveListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
    }
}

// callback to process new effect events from the interface
void InterfacePlayerBridge::onEffect(effects::Effect &effect){
    // finally, add the effect instance to the realtime_composition of the player
    m_player->realtime_composition.add(&effect);
    //    ofLog() << effect.type << "-type effect added to player's realtime composition";
}

