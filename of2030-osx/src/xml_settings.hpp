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
#include "setting_types.h"

namespace of2030{

    class XmlSettings{
    public:
        XmlSettings() : path("settings.xml"), log_level(OF_LOG_NOTICE){};
        void load();
        void save();

        std::string path;
        OscSetting osc_setting;

        int client_id;
        vector<int> multi_client_ids;
        ofVec3f multi_room_scale;

        string log_level_name;
        ofLogLevel log_level;
    };
}

#endif /* xml_settings_hpp */
