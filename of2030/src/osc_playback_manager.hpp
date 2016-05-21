//
//  osc_playback_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_playback_manager_hpp
#define osc_playback_manager_hpp

#include "ofMain.h"
#include "shared2030.h"
#include "osc_playback.hpp"

namespace of2030{
    
    class OscPlaybackManager{
        SINGLETON_CLASS_HEADER_CODE(OscPlaybackManager)

    public:
        ~OscPlaybackManager(){ clear(); }

        void start(string recording_path);
        void update();
        
        void clear();
        
        void add(OscPlayback &playback);
        bool remove(OscPlayback *playback);
    
    private:
        void onMessage(ofxOscMessage &message);
    
    private:
        vector<OscPlayback*> playbacks;
    };
}
#endif /* osc_playback_manager_hpp */
