//
//  effect_creator.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "interface_player_bridge.hpp"
#include "effects.hpp"
#include "xml_configs.hpp"
#include "effect_manager.hpp"
#include "video_manager.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(InterfacePlayerBridge)

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
        registerCallbacks(true);

    m_bStarted = true;
}

void InterfacePlayerBridge::destroy(){
    if(m_bStarted)
        registerCallbacks(false);
    m_bStarted = false;
}

void InterfacePlayerBridge::registerCallbacks(bool _register){
    ofLogVerbose() << "InterfacePlayerBridge::registerCallbacks with: " << _register;
    if(_register){
        // subscribe to events
        ofAddListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofAddListener(m_interface->stopTriggerEvent, this, &InterfacePlayerBridge::onStopTrigger);
//        ofAddListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofAddListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofAddListener(m_interface->screenConfigEvent, this, &InterfacePlayerBridge::onScreenConfig);
//        ofAddListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
//        ofAddListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
        ofAddListener(m_player->effect_manager.effectRemovedEvent, this, &InterfacePlayerBridge::onEffectEnded);
    } else {
        // unsubscribe from events
        ofRemoveListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofRemoveListener(m_interface->stopTriggerEvent, this, &InterfacePlayerBridge::onStopTrigger);
//        ofRemoveListener(m_interface->effectEvent, this, &InterfacePlayerBridge::onEffect);
        ofRemoveListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofRemoveListener(m_interface->screenConfigEvent, this, &InterfacePlayerBridge::onScreenConfig);
//        ofRemoveListener(m_interface->songEvent, this, &InterfacePlayerBridge::onSong);
//        ofRemoveListener(m_interface->clipEvent, this, &InterfacePlayerBridge::onClip);
        ofRemoveListener(m_player->effect_manager.effectRemovedEvent, this, &InterfacePlayerBridge::onEffectEnded);
    }
}


void InterfacePlayerBridge::onTrigger(string &trigger){
    Effect* effect;

    // first check if there's already an effect with this trigger
    // (and unique enabled) if so; abort
    const vector<Effect*> effects = m_player->getActiveEffects();
    for(int i=effects.size()-1; i>=0; i--){
        effect = effects[i];

        if(effect->getUnique() && effect->trigger == trigger){
            return;
        }
    }

    Effect* fx = EfficientEffectManager::instance()->get(trigger);
    if(!fx){
        ofLogError() << "Could not create effect for trigger: " << trigger;
        return;
    }

    // add to players realtime comp
    m_player->addEffect(*fx);

    // ofLogVerbose() << "effects in player's manager: " << m_player->effect_manager.getCount();
}

void InterfacePlayerBridge::onStopTrigger(string &trigger){
    if(trigger == ""){
        ofLog() << "InterfacePlayerBridge::onStopTrigger - clear all";
        m_player->clearEffects();
    } else {
        m_player->stopEffectByTrigger(trigger);
    }
}

//// callback to process new effect events from the interface
//void InterfacePlayerBridge::onEffect(string &name){
//    // create effect
//    Effect* fx = EfficientEffectManager::instance()->get(name);
//    
//    if(!fx){
//        ofLogError() << "Could not create effect for name: " << name;
//        return;
//    }
//
//    // add to players realtime comp
//    m_player->addEffect(*fx);
//}

void InterfacePlayerBridge::onEffectConfig(EffectConfig &cfg){
    XmlConfigs::instance()->setItemParam(cfg.setting_name, cfg.param_name, cfg.param_value);
}

void InterfacePlayerBridge::onScreenConfig(EffectConfig &cfg){
    XmlConfigs::screens()->setItemParam(cfg.setting_name, cfg.param_name, cfg.param_value);
}

//void InterfacePlayerBridge::onSong(string &name){
//    m_player->setSong(name);
//}
//
//void InterfacePlayerBridge::onClip(string &name){
//    m_player->setClip(name);
//}

void InterfacePlayerBridge::onEffectEnded(Effect &effect){
    EfficientEffectManager::instance()->finish(&effect);

#ifdef __AUTO_UNLOAD_VIDEOS_WHEN_EFFECTS_END__
    // did the ended effect have a video (player)?
    ofVideoPlayer* player = effect.getVideoPlayer();
    
    if(player){
        // let's see if there are any more effects using this player
        const vector<Effect*> effects = m_player->effect_manager.getEffects();

        for(int i=effects.size()-1; i>=0 i--){
            // does this effect use the same player?
            if(effects[i]->getVideoPlayer() == player){
                // player still in use, abort
                return;
            }
        }

        // no other effects found that use this player, tell manager to unload video from memory
        VideoManager::instance()->unload(player);
    }
#endif
}
