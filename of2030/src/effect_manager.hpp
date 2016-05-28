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

        void add(Effect* effect);
        bool remove(Effect* effect);
        void clear();

        Effect* get(string typ);
        Effect* findByType(EffectType typ);
        inline int getCount(){ return effects.size(); }
        inline int getCountByType(EffectType typ);
        inline const vector<Effect*> &getEffects(){ return effects; }

    public: // events
        ofEvent<Effect> effectAddedEvent;
        ofEvent<Effect> effectRemovedEvent;
        
    protected: // methods
        EffectType typeStringToType(string typ);
        Effect* createEffect(string typ);
        void deleteEffect(Effect* effect);

    protected: // attributes
        vector<Effect*> effects;
    };


    class EfficientEffectManager : public EffectManager{
        SINGLETON_CLASS_HEADER_CODE(EfficientEffectManager)

    public: // methods
        Effect* get(string typ);
        void finish(Effect* effect);
    
    protected: // attributes
        EffectManager idle_manager;
    };
}

#endif /* effect_manager_hpp */
