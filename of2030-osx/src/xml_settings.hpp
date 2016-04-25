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
        vector<int> multi_client_ids;
    };
}

#endif /* xml_settings_hpp */
