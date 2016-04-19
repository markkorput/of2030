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
#include "CMSCollection.h"
#include "effects.hpp"

namespace of2030{
    
    class Player {
        
    public: // main interface methods

        Player();
        // ~Player(){}

        //void setup();
        void update(); // realtime
        void update(float dt); // custom time
        //void destroy();

        void start();
        void stop();

    protected: // helper methods

        void setPlaybackTime(float time);
        void removeActiveEffectsEndingBefore(float time);

    public: // events

        ofEvent <effects::Effect> effectEndedEvent;

    public: // properties

        vector<effects::Effect> active_effects;

    private: // attributes
        
        bool m_bPlaying;
        float m_time, m_startTime;
        effects::Off m_offEffect; // if all effects end, at least this effect remains
    };
}

#endif /* player_hpp */
