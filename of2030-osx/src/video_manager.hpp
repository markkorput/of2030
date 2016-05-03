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

namespace of2030 {
    class VideoManager {

    public: // methods/interface
        
        static VideoManager* instance();
        VideoManager();
        ~VideoManager();

        void setup();
        void update();

        ofVideoPlayer* load(string video_name);
        ofVideoPlayer* get(string video_name, bool load=true);
    
    protected:
        string video_name_to_path(string video_name);
    
    private: // attributes
        static VideoManager* singleton;
        vector<ofVideoPlayer*> players;
    };
}

#endif /* video_manager_hpp */
