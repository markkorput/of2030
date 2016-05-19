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

        SINGLETON_CLASS_HEADER_CODE(VideoManager)

    public: // methods/interface
        VideoManager();
        ~VideoManager();

        //void setup();
        void update();

        ofVideoPlayer* load(string video_name);
        ofVideoPlayer* get(string video_name, bool load=true);
    
    protected: // helper methods
        string video_name_to_path(string video_name);
    
    private: // attributes
        vector<ofVideoPlayer*> players;
    };
}

#endif /* video_manager_hpp */