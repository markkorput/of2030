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
        inline ~VideoManager(){ destroy(); }

        //void setup();
        void update();
        void destroy();

        ofVideoPlayer* load(const string &video_name);
        ofVideoPlayer* get(const string &video_name, bool load=true);
        
        bool unload(const string &video_name);
        void unload(ofVideoPlayer *player);

    protected: // helper methods
        string video_name_to_path(const string &video_name);
    
    private: // attributes
        map<string, ofVideoPlayer*> players;
        //        vector<ofVideoPlayer*> players;
        string folder_path;
    };
}

#endif /* video_manager_hpp */
