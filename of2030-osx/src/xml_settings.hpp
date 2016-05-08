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
#include "shared2030.h"

namespace of2030{

    class XmlSettings{
    public:
        static XmlSettings* instance();
    private:
        static XmlSettings* singleton;

    public:
        XmlSettings() : path("settings.xml"), log_level(OF_LOG_NOTICE){};
        void load(bool reload=false);
        // void save();

        std::string path;
        OscSetting osc_setting;

        bool loaded;
        string client_id;
#ifdef __MULTI_CLIENT_ENABLED__
        vector<string> multi_client_ids;
        ofVec3f multi_room_scale;
#endif

        string log_level_name;
        ofLogLevel log_level;
    };
}

#endif /* xml_settings_hpp */
