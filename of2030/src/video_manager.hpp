//
//  video_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-03.
//
//

#ifndef video_manager_hpp
#define video_manager_hpp

#include "ofMain.h"
#include "shared2030.h"

namespace of2030 {
    class VideoManager {

        SINGLETON_INLINE_HEADER_CODE(VideoManager)

    public: // methods/interface
        VideoManager();
        ~VideoManager(){ destroy(); }

        //void setup();
        void update();
        void destroy(){ unloadAll(); }

        ofVideoPlayer* get(const string &video_name, bool load=true);
        ofVideoPlayer* get(const string &video_name, const string &alias, bool load=true);
        
        void unloadAll();
        bool unload(const string &alias);
        void unload(ofVideoPlayer *player);
        inline void deprecate(ofVideoPlayer *player){ deprecated_players.push_back(player); }

    public: // events
        
        ofEvent<ofVideoPlayer> unloadEvent;

    protected: // helper methods

        inline string video_name_to_path(const string &video_name) const { return folder_path + video_name; }
        ofVideoPlayer* createPlayer(const string &video_name);
    
    private: // attributes
        map<string, ofVideoPlayer*> players;
        vector<ofVideoPlayer*> deprecated_players;

        //        vector<ofVideoPlayer*> players;
        string folder_path;
    };
}

#endif /* video_manager_hpp */
