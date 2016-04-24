//
//  config_file.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-24.
//
//

#ifndef config_file_hpp
#define config_file_hpp

#include "ofMain.h"
#include "ofxJSONElement.h"

namespace of2030{

    class ConfigFile {
        
    public:
        ConfigFile();
        // ~ConfigFile();
        
        void setPath(string path);
        void load();
        string getValue(std::string valuePath);
        

    private: // atributes

        bool m_bLoaded;
        ofxJSONElement m_json;
        string m_path;
    };
}

#endif /* config_file_hpp */
