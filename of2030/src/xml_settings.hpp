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
        SINGLETON_INLINE_HEADER_CODE(XmlSettings)

    public: // methods
        XmlSettings() : loaded(false), path("settings.xml"), client_id_path("client_id.xml"), log_level(OF_LOG_NOTICE){};
        bool load(bool reload=false);
        // void save();

    public: // properties
        string client_id;
        OscSetting osc_setting;
        // logging
        string log_level_name;
        ofLogLevel log_level;
        float log_alive_interval;
        ofVec3f room_size;

        bool rgbaVidPixels;
        bool alphaBlending;
#ifdef __MULTI_CLIENT_ENABLED__
        
        vector<string> multi_client_ids;
        bool multi_debug;
        float multi_scale;
        ofVec3f multi_translate;
#endif

    private: // attributes
        bool loaded;
        std::string path, client_id_path;
    };
}

#endif /* xml_settings_hpp */
