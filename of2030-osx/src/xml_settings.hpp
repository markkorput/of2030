//
//  xml_settings.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef xml_settings_hpp
#define xml_settings_hpp

#include "ofMain.h"

namespace of2030{
    
    class XmlSettings{
    public:
        XmlSettings() : path("settings.xml"){};
        void load();
        void save();
        
        std::string path;
        int osc_port;
        int client_id;
        vector<int> multi_client_ids;
        float multi_screen_scale;
        ofVec3f multi_room_scale;
    };
}

#endif /* xml_settings_hpp */
