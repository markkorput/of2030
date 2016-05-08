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

void InterfacePlayerBridge::start(){
    registerInterfaceCallbacks(true);
    m_bStarted = true;
}

void InterfacePlayerBridge::stop(){
    registerInterfaceCallbacks(false);
    m_bStarted = false;
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

void InterfacePlayerBridge::registerInterfaceCallbacks(bool _register){
    if(_register){
        // subscribe to new effect model events of specified interface
        ofAddListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofAddListener(m_interface->shaderEffectEvent, this, &InterfacePlayerBridge::onShaderEffect);
        ofAddListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofAddListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    } else {
        // unsubscribe from new effect model events of previous interface
        ofRemoveListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofRemoveListener(m_interface->shaderEffectEvent, this, &InterfacePlayerBridge::onShaderEffect);
        ofRemoveListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofRemoveListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    }
}

// callback to process new effect events from the interface
void InterfacePlayerBridge::onEffect(string &name){
    // create effect
    effects::Effect* fx = createEffect(name);
    // add to players realtime comp
    m_player->realtime_composition.add(fx);
}

void InterfacePlayerBridge::onShaderEffect(string &shader){
    // create effect
    effects::ShaderEffect* fx = new effects::ShaderEffect();
    fx->setShader(shader);
    // add to players realtime comp
    m_player->realtime_composition.add((effects::Effect*)fx);
}

void InterfacePlayerBridge::onSong(string &name){
    m_player->song = name;
}

void InterfacePlayerBridge::onClip(string &name){
    m_player->clip = name;
}

effects::Effect* InterfacePlayerBridge::createEffect(string &name){
    if(name == "cursor")
        return (effects::Effect*) new effects::Cursor();

    if(name == "vid")
        return (effects::Effect*) new effects::Vid();

    return NULL;
}