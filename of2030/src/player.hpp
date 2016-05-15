//
//  player.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef player_hpp
#define player_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "effects.hpp"
#include "effect_manager.hpp"

namespace of2030{

    class Player {

    public: // singleton accessor method

        static Player* instance();

    private: // singleton static attribute

        static Player* singleton;

    public: // main interface methods

        Player();
        // ~Player(){}

        //void setup();
        void update(); // realtime
        void update(float dt); // custom time
        //void destroy();

        void start();
        void stop();

        float getTime(){ return m_time; }
        bool effectActive(effects::Effect &effect);
        void activateEffect(effects::Effect &effect);

    protected: // helper methods

        void setPlaybackTime(float time);
        void removeActiveEffectsEndingBefore(float time);

    public: // events

        ofEvent <effects::Effect> effectEndedEvent;

    public: // properties

        vector<effects::Effect*> active_effects;
        EffectManager effect_manager;
        string song, clip;

    private: // attributes

        bool m_bPlaying;
        float m_time, m_startTime;
    };
}

#endif /* player_hpp */
