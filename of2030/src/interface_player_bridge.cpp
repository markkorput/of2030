//
//  effect_creator.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "interface_player_bridge.hpp"
#include "effects.hpp"
#include "xml_triggers.hpp"
#include "xml_configs.hpp"

using namespace of2030;

SINGLETON_CLASS_IMPLEMENTATION_CODE(InterfacePlayerBridge)

InterfacePlayerBridge::InterfacePlayerBridge(){
    m_interface = NULL;
    m_player = NULL;
    m_bStarted = false;
}

InterfacePlayerBridge::~InterfacePlayerBridge(){
    destroy();
}

void InterfacePlayerBridge::setup(){
    ofLogVerbose() << "InterfacePlayerBridge::setup";
    if(!m_interface)
        m_interface = Interface::instance();

    if(!m_player)
        m_player = Player::instance();

    if(!m_bStarted)
        registerInterfaceCallbacks(true);

    m_bStarted = true;
}

void InterfacePlayerBridge::destroy(){
    if(m_bStarted)
        registerInterfaceCallbacks(false);
    m_bStarted = false;
}

void InterfacePlayerBridge::registerInterfaceCallbacks(bool _register){
    ofLogVerbose() << "InterfacePlayerBridge::registerInterfaceCallbacks with: " << _register;
    if(_register){
        // subscribe to events
        ofAddListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofAddListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofAddListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofAddListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofAddListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    } else {
        // unsubscribe from events
        ofRemoveListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofRemoveListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofRemoveListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofRemoveListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofRemoveListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    }
}

void InterfacePlayerBridge::onTrigger(string &trigger){
    // get effect to be triggerd by this trigger name
    const string effectName = XmlTriggers::instance()->getEffectName(trigger);

    // non-shader effect
    effects::Effect* fx = createEffect(effectName);
    if(!fx){
        ofLogError() << "Could not create effect for trigger: " << trigger;
        return;
    }
    fx->trigger = trigger;
    // add to players realtime comp
    m_player->realtime_composition.add(fx);
}

// callback to process new effect events from the interface
void InterfacePlayerBridge::onEffect(string &name){
    // create effect
    effects::Effect* fx = createEffect(name);
    // add to players realtime comp
    m_player->realtime_composition.add(fx);
}

void InterfacePlayerBridge::onEffectConfig(EffectConfig &cfg){
    XmlConfigs::instance()->setItemParam(cfg.setting_name, cfg.param_name, cfg.param_value);
}

void InterfacePlayerBridge::onSong(string &name){
    m_player->song = name;
}

void InterfacePlayerBridge::onClip(string &name){
    m_player->clip = name;
}

effects::Effect* InterfacePlayerBridge::createEffect(const string &name){
    if(name == "vid")
        return (effects::Effect*) new effects::Vid();

    // default type; use the name as effect name
    effects::Effect* pEffect = new effects::Effect();
    pEffect->name = name;
    return pEffect;
}
