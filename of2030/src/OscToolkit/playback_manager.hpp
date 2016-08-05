//
//  osc_playback_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_playback_manager_hpp
#define osc_playback_manager_hpp

#include "shared2030.h"

#include "playback.hpp"
#include "osc_interface.hpp"

namespace OscToolkit{

    class PlaybackManager{
        SINGLETON_INLINE_HEADER_CODE(PlaybackManager)

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
        // void onMessage(ofxOscMessage &message);
        Playback* getPlayback(const string &name);

        
    public: // attributes
        of2030::OscInterface* interface;

    private:
        
        vector<Playback*> playbacks;
//#ifdef __OSC_SENDER_ENABLED__
//        bool toOscSender;
//    public:
//        void setToOscSender(bool set){ toOscSender=set; }
//#endif

    };
}
#endif /* osc_playback_manager_hpp */
