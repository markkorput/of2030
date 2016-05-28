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

        void addEffect(Effect &effect);
        void stopEffectByTrigger(string &trigger);
        void stopEffectsByVideoPlayer(ofVideoPlayer *player);
        void stopAllVideoEffects();
        void clearEffects();

        inline const vector<Effect*> &getActiveEffects(){ return active_effects_manager.getEffects(); }
        inline const string &getSong(){ return song; }
        inline const string &getClip(){ return clip; }
        inline void setSong(const string &newSong){ song = newSong; }
        inline void setClip(const string &newClip){ clip = newClip; }

        inline float getTime(){ return m_time; }

    protected: // helper methods

        void movePlaybackTimeTo(float time);
        void setPlaybackTime(float time);
        inline bool effectStarted(const Effect &effect);
        inline bool effectEnded(const Effect &effect);

    public: // properties

        EffectManager effect_manager;

    private: // attributes

        string song, clip;
        EffectManager active_effects_manager, pending_effects_manager;

        bool m_bPlaying;
        float m_time, m_startTime, m_lastUpdateTime;
    };
}

#endif /* player_hpp */
