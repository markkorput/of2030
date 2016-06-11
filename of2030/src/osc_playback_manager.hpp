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
        SINGLETON_INLINE_HEADER_CODE(OscPlaybackManager)

    public:
        ~OscPlaybackManager(){ clear(); }

        bool start(const string &name);
        bool stop(const string &name);
        void update(float dt);
    
    private: // methods

        void add(OscPlayback &playback);
        bool remove(OscPlayback *playback);
        void clear();

        static string nameToPath(const string &name);
        void onMessage(ofxOscMessage &message);
        OscPlayback* getPlayback(const string &name);

    private:
        vector<OscPlayback*> playbacks;
    };
}
#endif /* osc_playback_manager_hpp */
