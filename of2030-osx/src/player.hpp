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

#include "CMSCollection.h"
#include "effect.hpp"

namespace of2030{
    
    class Player {
        
    public: // main interface methods

        Player() : m_time(0.0f), m_bPlaying(false){}
        // ~Player(){}

        //void setup();
        void update(); // realtime
        void update(float dt); // custom time
        //void destroy();

        void start();
        void stop();

    protected: // helper methods

        void setPlaybackTime(float time);

    public: // properties

        // CMS::Collection<Effect> active_effects_collection;

    private: // attributes
        
        bool m_bPlaying;
        float m_time, m_startTime;
    };
}

#endif /* player_hpp */
