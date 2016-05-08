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
#include "xml_effects.hpp"

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
        // subscribe to events
        ofAddListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofAddListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofAddListener(m_interface->shaderEffectEvent, this, &InterfacePlayerBridge::onShaderEffect);
        ofAddListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofAddListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofAddListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    } else {
        // unsubscribe from events
        ofRemoveListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofRemoveListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofRemoveListener(m_interface->shaderEffectEvent, this, &InterfacePlayerBridge::onShaderEffect);
        ofRemoveListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofRemoveListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
        ofRemoveListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
    }
}

void InterfacePlayerBridge::onTrigger(string &trigger){
    // get effect to be triggerd by this trigger name
    string effectName = XmlTriggers::instance()->getEffectName(trigger);

    // shader effect trigger?
    string sub = "shader-";

    if(effectName.substr(0, sub.size()) == sub){
        // get shader name
        sub = effectName.substr(sub.size());
        // create effect
        effects::ShaderEffect* fx = new effects::ShaderEffect();
        fx->setShader(sub);
        fx->trigger = trigger;
        // add to players realtime comp
        m_player->realtime_composition.add((effects::Effect*)fx);
        return;
    }

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

void InterfacePlayerBridge::onShaderEffect(string &shader){
    // create effect
    effects::ShaderEffect* fx = new effects::ShaderEffect();
    fx->setShader(shader);
    // add to players realtime comp
    m_player->realtime_composition.add((effects::Effect*)fx);
}

void InterfacePlayerBridge::onEffectConfig(EffectConfig &cfg){
    XmlEffects::instance()->setEffectSettingParam(cfg.setting_name, cfg.param_name, cfg.param_value);
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