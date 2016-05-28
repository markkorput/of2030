//
//  effect_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#include "effect_manager.hpp"

using namespace of2030;

EffectManager::~EffectManager(){
    for(auto effect: effects){
        deleteEffect(effect);
    }

    effects.clear();
}

Effect* EffectManager::get(string typ){
    // create new instance
    Effect* pEffect = createEffect(typ);
    // add to our internal list
    add(pEffect);
    // return it to caller
    return pEffect;
}

// returns a pointer (or NULL if not found) to an allocated effect instance
// of the specified type in the internal idle_effects vector
Effect* EffectManager::findByType(EffectType typ){
    for(int i=effects.size()-1; i>=0; i--){
        if(effects[i]->getType() == typ){
            // found!
            return effects[i];
        }
    }

    // not found
    return NULL;
}

void EffectManager::add(Effect* effect){
    effects.push_back(effect);
    ofNotifyEvent(effectAddedEvent, *effect, this);
}

EffectType EffectManager::typeStringToType(string typ){
//     if(typ == "spot") return EffectType::SPOT;
//    if(typ == "pos") return EffectType::POS;
    return EffectType::DEFAULT;
}

Effect* EffectManager::createEffect(string typ){
    ofLogVerbose() << "EffectManager::createEffect with: " << typ;

    Effect* pEffect;

//     if(typ == "spot") return (Effect*) new effects::Spot();
//    if(typ == "pos") return (Effect*) new effects::Pos();

    // default type, just set name to whatever was specified
    pEffect = new Effect();
    pEffect->name = typ;
    return pEffect;
}

#define IF_TYP_DEL(__x__,__y__) \
    if(effect->getType() == EffectType::__x__){\
        delete (effects::__y__*) effect;\
        return;\
    }\

void EffectManager::deleteEffect(Effect* effect){
    ofLogVerbose() << "EffectManager::deleteEffect";
    
//    IF_TYP_DEL(SPOT, Spot)
//    IF_TYP_DEL(POS, Pos)
//    // figure out effect type and delete from memory
//    if(effect->getType() == EffectType::VID){
//        // turn into Vid effect pointer before deleting, to delete appropriate class type
//        delete (effects::Vid*) effect;
//        return;
//    }
//    
//    if(effect->getType() == EffectType::SPOT){
//        delete (effects::Spot*) effect;
//        return;
//    }
//    
//    if(effect->getType() == EffectType::VOICE){
//        delete (effects::Voice*) effect;
//        return;
//    }
    
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

int EffectManager::getCountByType(EffectType typ){
    int count = 0;
    for(auto effect: effects)
        if(effect->getType() == typ)
            count++;
    return count;
}




//
// EfficientEffectManager
//

SINGLETON_CLASS_IMPLEMENTATION_CODE(EfficientEffectManager)

const int idle_cache_limit_per_type = 3;

Effect* EfficientEffectManager::get(string typ){
    // convert string (typ) to EffectType (type)
    EffectType type = typeStringToType(typ);
    // find already allocated effect in idle manager
    Effect* pEffect = idle_manager.findByType(type);

    // no existing idle instance found, default to "normal behaviour" (allocate new instance)
    if(!pEffect){
        // no idle effect found; create new one like usual
        return EffectManager::get(typ);
    }
    
    // found one, remove it from idle manager
    idle_manager.remove(pEffect);
    // reset its time values (and some other attributes)
    pEffect->reset();
    // update name if necessary
//    if(pEffect->getType() == EffectType::DEFAULT){
    pEffect->name = typ;
//    }

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
    if(idle_manager.getCountByType(effect->getType()) >= idle_cache_limit_per_type){
        // idle cache full, just delete this instance
        deleteEffect(effect);
        return;
    }

    // add it to our idle_manager, so we can recycle this instance later
    // ofLogVerbose() << "idle_manager count before: " << idle_manager.getCount();
    idle_manager.add(effect);
    ofLogVerbose() << "[EfficientEffectManager] idle effects count: " << idle_manager.getCount();
}

