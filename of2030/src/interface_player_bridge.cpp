//
//  effect_creator.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "interface_player_bridge.hpp"
#include "effect.hpp"
#include "xml_configs.hpp"
#include "video_manager.hpp"
#include "image_manager.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(InterfacePlayerBridge)

InterfacePlayerBridge::InterfacePlayerBridge(){
    m_interface = NULL;
    m_player = NULL;
    m_bStarted = false;
    active_effects = NULL;
}

InterfacePlayerBridge::~InterfacePlayerBridge(){
    destroy();
}

void InterfacePlayerBridge::setup(){
    ofLogVerbose() << "InterfacePlayerBridge::setup";
    if(!m_interface)
        m_interface = Interface::instance();

    if(!m_player){
        m_player = Player::instance();
    }
    
    if(!active_effects){
        active_effects = &m_player->getActiveEffects();
    }

    if(!m_bStarted)
        registerCallbacks(true);

    m_bStarted = true;
}

void InterfacePlayerBridge::destroy(){
    if(m_bStarted)
        registerCallbacks(false);
    m_bStarted = false;
}

void InterfacePlayerBridge::onOscMessage(ofxOscMessage &m){
    string addr,param,sub;
    
    addr = m.getAddress();
    if(m.getNumArgs() > 0)
        param = m.getArgAsString(0);
    else
        param = "";
    
    ofLogVerbose() << "[osc-in] " << addr << " with " << param;
    
    
    if(addr == OSC_TRIGGER){
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    sub = OSC_TRIGGER;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    
    sub = OSC_CONFIG_EFFECT;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        sub = addr.substr(sub.size());
        std::size_t pos = sub.find("/");
        
        if (pos==std::string::npos){
            ofLogError() << "could not get effect-path and param name from osc address";
            return;
        }
        
        if(m.getNumArgs() < 1){
            ofLogError() << "param value missing from OSC message";
            return;
        }
        
        EffectConfig cfg;
        cfg.setting_name = sub.substr(0, pos);
        cfg.param_name = sub.substr(pos+1);
        cfg.param_value = param;
        
        ofNotifyEvent(m_interface->effectConfigEvent, cfg, m_interface);
        return;
    }
    
    
    if(addr == OSC_PLAY){
        ofNotifyEvent(m_interface->playbackEvent, param, m_interface);
        return;
    }
    
    sub = OSC_PLAY;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->playbackEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_TRIGGERSTOP){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    sub = OSC_TRIGGERSTOP;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_PLAYSTOP){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->stopPlaybackEvent, param, m_interface);
        return;
    }
    
    sub = OSC_PLAYSTOP;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->stopPlaybackEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_CONTROL){
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    sub = OSC_CONTROL;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    sub = OSC_CONFIG_SCREEN;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        sub = addr.substr(sub.size());
        std::size_t pos = sub.find("/");
        
        if (pos==std::string::npos){
            ofLogError() << "could not get screen-name and param name from osc address";
            return;
        }
        
        if(m.getNumArgs() < 1){
            ofLogError() << "param value missing from OSC message";
            return;
        }
        
        EffectConfig cfg;
        
        // 2 params and screen config param-name end with pos?
        // then treat as two _x and _y params
        if(m.getNumArgs() == 2 && sub.substr(sub.size()-3) == "pos"){
            cfg.setting_name = sub.substr(0, pos);
            cfg.param_name = sub.substr(pos+1) + "_x";
            cfg.param_value = param;
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_y";
            cfg.param_value = m.getArgAsString(1);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        }
        
        // 2 params and screen config param-name end with pos?
        // then treat as two _x and _y params
        if(m.getNumArgs() == 3 && sub.substr(sub.size()-3) == "pos"){
            cfg.setting_name = sub.substr(0, pos);
            cfg.param_name = sub.substr(pos+1) + "_x";
            cfg.param_value = param;
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_y";
            cfg.param_value = m.getArgAsString(1);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_z";
            cfg.param_value = m.getArgAsString(2);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        }
        
        cfg.setting_name = sub.substr(0, pos);
        cfg.param_name = sub.substr(pos+1);
        cfg.param_value = param;
        
        ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        return;
    }
    
    if(addr == OSC_VIDLOAD){
        ofNotifyEvent(m_interface->loadVideoEvent, param, m_interface);
        return;
    }
    
    sub = OSC_VIDLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->loadVideoEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_VIDUNLOAD){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->unloadVideoEvent, param, m_interface);
        return;
    }
    
    sub = OSC_VIDUNLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->unloadVideoEvent, param, m_interface);
        return;
    }
    
    
    if(addr == OSC_IMGLOAD){
        ofNotifyEvent(m_interface->loadImageEvent, param, m_interface);
        return;
    }
    
    sub = OSC_IMGLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->loadImageEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_IMGUNLOAD){
        ofNotifyEvent(m_interface->unloadImageEvent, param, m_interface);
        return;
    }
    
    sub = OSC_IMGUNLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->unloadImageEvent, param, m_interface);
        return;
    }
    
    ofLog() << "Unable to process OSC Message " << m.getAddress();
}




void InterfacePlayerBridge::registerCallbacks(bool _register){
    ofLogVerbose() << "InterfacePlayerBridge::registerCallbacks with: " << _register;
    if(_register){
        // subscribe to events
        ofAddListener(OscInterface::instance()->messageEvent, this, &InterfacePlayerBridge::onOscMessage);
        ofAddListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofAddListener(m_interface->stopTriggerEvent, this, &InterfacePlayerBridge::onStopTrigger);
        ofAddListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofAddListener(m_interface->screenConfigEvent, this, &InterfacePlayerBridge::onScreenConfig);
        ofAddListener(m_player->effect_manager.effectRemovedEvent, this, &InterfacePlayerBridge::onEffectEnded);
    } else {
        // unsubscribe from events
        ofRemoveListener(OscInterface::instance()->messageEvent, this, &InterfacePlayerBridge::onOscMessage);
        ofRemoveListener(m_interface->triggerEvent, this, &InterfacePlayerBridge::onTrigger);
        ofRemoveListener(m_interface->stopTriggerEvent, this, &InterfacePlayerBridge::onStopTrigger);
        ofRemoveListener(m_interface->effectConfigEvent, this, &InterfacePlayerBridge::onEffectConfig);
        ofRemoveListener(m_interface->screenConfigEvent, this, &InterfacePlayerBridge::onScreenConfig);
        ofRemoveListener(m_player->effect_manager.effectRemovedEvent, this, &InterfacePlayerBridge::onEffectEnded);
    }
}


void InterfacePlayerBridge::onTrigger(string &trigger){
    Effect* effect;

    // first check if there's already an effect with this trigger
    // (and unique enabled) if so; abort
    for(int i=active_effects->size()-1; i>=0; i--){
        effect = (*active_effects)[i];

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
        // ofLog() << "InterfacePlayerBridge::onStopTrigger - clear all";
        m_player->clearEffects();
    } else {
        m_player->stopEffectByTrigger(trigger);
    }
}

void InterfacePlayerBridge::onEffectConfig(EffectConfig &cfg){
    XmlConfigs::instance()->setItemParam(cfg.setting_name, cfg.param_name, cfg.param_value);
}

void InterfacePlayerBridge::onScreenConfig(EffectConfig &cfg){
    XmlConfigs::screens()->setItemParam(cfg.setting_name, cfg.param_name, cfg.param_value);
}

void InterfacePlayerBridge::onEffectEnded(Effect &effect){
    EfficientEffectManager::instance()->finish(&effect);

#ifndef __AUTO_UNLOAD_VIDEOS_WHEN_EFFECTS_END__
    // do we want to unload the videos player of this effect?
    if(effect.unloadVideos()){
        ofVideoPlayer* player;
        
        player = effect.getVideoPlayer();
        if(player){
            VideoManager::instance()->deprecate(player);
        }
        
        player = effect.getMaskVideoPlayer();
        if(player){
            VideoManager::instance()->deprecate(player);
        }
    }
    
    // do we want to unload the images of this effect?
    if(effect.unloadImages()){
        ofImage* img;
        img = effect.getImage();
        if(img){
            ImageManager::instance()->unload(img);
        }
        img = effect.getMaskImage();
        if(img){
            ImageManager::instance()->unload(img);
        }
    }
#else
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
        VideoManager::instance()->deprecate(player);
    }
#endif
}
