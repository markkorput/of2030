//
//  effect_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#include "effect_manager.hpp"
#include <regex>

using namespace of2030;

EffectManager::~EffectManager(){
    for(auto effect: effects){
        deleteEffect(effect);
    }

    effects.clear();
}

Effect* EffectManager::get(const string &trigger){
    // create new instance
    Effect* pEffect = createEffect(trigger);
    // add to our internal list
    add(pEffect);
    // return it to caller
    return pEffect;
}

void EffectManager::add(Effect* effect){
    effects.push_back(effect);
    ofNotifyEvent(effectAddedEvent, *effect, this);
}

Effect* EffectManager::createEffect(const string &trigger){
    ofLogVerbose() << "EffectManager::createEffect with: " << trigger;

    Effect* pEffect;

    // default type, just set name to whatever was specified
    pEffect = new Effect();
    pEffect->name = triggerToName(trigger);
    pEffect->trigger = trigger;
    return pEffect;
}

#define IF_TYP_DEL(__x__,__y__) \
    if(effect->getType() == EffectType::__x__){\
        delete (effects::__y__*) effect;\
        return;\
    }\

void EffectManager::deleteEffect(Effect* effect){
    ofLogVerbose() << "EffectManager::deleteEffect";
    
    // default; simply Effect
    delete effect;
}

bool EffectManager::remove(Effect* effect){
    // find effect in our list
    for(int i=effects.size()-1; i>=0; i--){
        // this one?
        if(effects[i] == effect){
            // remove from list
            effects.erase(effects.begin()+i);
            // notify
            ofNotifyEvent(effectRemovedEvent, *effect, this);
            // done
            return true;
        }
    }

    // not found
    ofLogWarning() << "[EffectManager::remove] could not find specified effect in list, ignoring";
    return false;
}

void EffectManager::clear(){
    for(auto effect: effects){
        remove(effect);
    }
}

string EffectManager::triggerToName(const string &trigger){
    // matches on any digits at then end of the string
    std::regex expression("(\\d+)$");
    // return the trigger name without any trailing digits
    return std::regex_replace(trigger, expression, "");
}



//
// EfficientEffectManager
//

SINGLETON_CLASS_IMPLEMENTATION_CODE(EfficientEffectManager)

const int idle_cache_limit = 3;

Effect* EfficientEffectManager::get(string trigger){
    // find already allocated effect in idle manager
    Effect* pEffect = idle_manager.getEffectByIndex(idle_manager.getCount() - 1);

    // no existing idle instance found, default to "normal behaviour" (allocate new instance)
    if(!pEffect){
        // no idle effect found; create new one like usual
        return EffectManager::get(trigger);
    }

    // found one, remove it from idle manager
    idle_manager.remove(pEffect);
    // reset its time values (and some other attributes)
    pEffect->reset();
    // update name if necessary
    pEffect->name = triggerToName(trigger);
    pEffect->trigger = trigger;

    // add to our own list
    add(pEffect);

    // done
    return pEffect;
}

void EfficientEffectManager::finish(Effect* effect){
    // ofLogVerbose() << "EfficientEffectManager::finish";

    // remove from our "active list"
    if(!remove(effect)){
        // remove failed, meaning this effect could not be found in our list, abort
        return;
    }

    // check if cache of idle instance has reached its limits yet
    if(idle_manager.getCount() >= idle_cache_limit){
        // idle cache full, just delete this instance
        deleteEffect(effect);
        return;
    }

    // add it to our idle_manager, so we can recycle this instance later
    // ofLogVerbose() << "idle_manager count before: " << idle_manager.getCount();
    idle_manager.add(effect);
    ofLogVerbose() << "[EfficientEffectManager] idle effects count: " << idle_manager.getCount();
}
