//
//  config_file.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-24.
//
//

#ifndef config_file_hpp
#define config_file_hpp

#include "json_file_reader.hpp"

namespace of2030{

    class ConfigFile : public JsonFileReader{
        
    public:
        // ConfigFile();
        // ~ConfigFile();

        int getClientCount();
        const vector<string> getClientIds();

    private: // attributes
    };
}

#endif /* config_file_hpp */
