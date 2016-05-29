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

        Effect* get(const string &trigger);
        inline int getCount(){ return effects.size(); }
        inline const vector<Effect*> &getEffects(){ return effects; }
        inline Effect* getEffectByIndex(int idx){ return idx >= 0 && effects.size() > idx ? effects[idx] : NULL; }

    public: // events
        ofEvent<Effect> effectAddedEvent;
        ofEvent<Effect> effectRemovedEvent;

    protected: // methods
        static Effect* createEffect(const string &trigger);
        static void deleteEffect(Effect* effect);
        static string triggerToName(const string &trigger);

    protected: // attributes
        vector<Effect*> effects;
    };


    class EfficientEffectManager : public EffectManager{
        SINGLETON_CLASS_HEADER_CODE(EfficientEffectManager)

    public: // methods
        Effect* get(string trigger);
        void finish(Effect* effect);
    
    protected: // attributes
        EffectManager idle_manager;
    };
}

#endif /* effect_manager_hpp */
