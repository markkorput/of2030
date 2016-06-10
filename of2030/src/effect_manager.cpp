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
    for(int i=effects.size()-1; i>=0; i--){
        deleteEffect(effects[i]);
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
    bool added=false;

    if(bSortByLayerAscending){
        int count=effects.size();
        int layer = effect->getLayer();

        for(int i=0; i<count; i++){
            if(effects[i]->getLayer() > layer){
                effects.insert(effects.begin()+i, effect);
                added=true;
                break;
            }

        }
    }

    // no effect found with a layer-value that's larger
    // than the new effect's layer-value; simply add the new effect
    // to the end of our list
    if(!added)
        effects.push_back(effect);

    ofNotifyEvent(effectAddedEvent, *effect, this);
}

Effect* EffectManager::createEffect(const string &trigger){
    ofLogVerbose() << "EffectManager::createEffect with: " << trigger;

    Effect* pEffect;

    // default type, just set name to whatever was specified
    pEffect = new Effect();
    // pEffect->name = triggerToName(trigger);
    pEffect->trigger = trigger;
    return pEffect;
}

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
// THIS CAUSES CRASHES
//    for(auto effect: effects){
//        remove(effect);
//    }

    for(int i=effects.size()-1; i>=0; i--){
        remove(effects[i]);
    }
}

//string EffectManager::triggerToName(const string &trigger){
//    // matches on any digits at then end of the string
//    std::regex expression("(\\d+)$");
//    // return the trigger name without any trailing digits
//    return std::regex_replace(trigger, expression, "");
//}

void EffectManager::sort(){
    vector<Effect*> sorted_effects;
    int lowest, tmp;

    // we'll move effect pointers from our effects vector
    // into the local sorted_effects vector and swap them at the end
    // kee looping until all pointer are moved
    while(!effects.empty()){

        // we'll need an initial layer value; take the first effect's layer
        lowest = effects[0]->getLayer();

        // loop over all other effects, to find the (next) lowest layer value
        for(int i=effects.size()-1; i>0; i--){
            tmp = effects[i]->getLayer();
            if(tmp < lowest)
                lowest = tmp;
        }

        // lowest now contains the lowest layer value for the remaining effects
        // loop over all remaining effects again and move the ones with this layer value
        tmp = effects.size();
        for(int i=0; i<tmp; i++){
            Effect* effect = effects[i];
            // does this effect has the lowest layer value?
            if(effect->getLayer() == lowest){
                // add this effect to the (back of) the sorted vector...
                sorted_effects.push_back(effect);
                // ... and remove it from our main vector
                effects.erase(effects.begin()+i);
            }
        }
    }

    // swap the content of the -currently empty- effects vector
    // with the temporary local sorted effects vector
    effects.swap(sorted_effects);
}


//
// EfficientEffectManager
//

SINGLETON_INLINE_IMPLEMENTATION_CODE(EfficientEffectManager)

const int idle_cache_limit = 5;

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
    //pEffect->name = triggerToName(trigger);
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
