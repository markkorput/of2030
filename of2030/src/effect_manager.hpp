//
//  effect_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#ifndef effect_manager_hpp
#define effect_manager_hpp

#include "shared2030.h"
#include "effects.hpp"

#include "ofMain.h"


namespace of2030{
    class EffectManager{

    public: // methods
        ~EffectManager();

        void add(effects::Effect* effect);
        bool remove(effects::Effect* effect);

        effects::Effect* get(string typ);
        effects::Effect* findByType(effects::EffectType typ);
        inline int getCount(){ return effects.size(); }
        inline const vector<effects::Effect*> &getEffects(){ return effects; }

    public: // events
        ofEvent<effects::Effect> effectAddedEvent;
        ofEvent<effects::Effect> effectRemovedEvent;
        
    protected: // methods
        effects::EffectType typeStringToType(string typ);
        effects::Effect* createEffect(string typ);
        void deleteEffect(effects::Effect* effect);

    protected: // attributes
        vector<effects::Effect*> effects;
    };


    class EfficientEffectManager : public EffectManager{
        SINGLETON_CLASS_HEADER_CODE(EfficientEffectManager)
    
    public: // methods
        effects::Effect* get(string typ);
        void finish(effects::Effect* effect);
    
    protected: // attributes
        EffectManager idle_manager;
    };
}

#endif /* effect_manager_hpp */
