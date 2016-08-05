//
//  osc_playback_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_playback_manager_hpp
#define osc_playback_manager_hpp

#include "shared.h"
#include "playback.hpp"
#include "interface.hpp"

namespace OscToolkit{

    class PlaybackManager{
        OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(PlaybackManager)

    public:
        PlaybackManager();
        ~PlaybackManager(){ clear(); }

        Playback* start(const string &name);
        bool stop(const string &name);
        void update(float dt);
    
    private: // methods

        bool remove(Playback *playback);
        void clear();

        static string nameToPath(const string &name);
        Playback* getPlayback(const string &name);

        
    public: // attributes
        Interface* interface;

    private:
        vector<Playback*> playbacks;
    };
}
#endif /* osc_playback_manager_hpp */
