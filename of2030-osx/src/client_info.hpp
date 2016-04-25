//
//  client_info.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef client_info_hpp
#define client_info_hpp

#include "json_file_reader.hpp"
#include "config_file.hpp"

namespace of2030{
    class ClientInfo {

    public:
        static ClientInfo* instance();
    private:
        static ClientInfo* singleton;
        
    public:
        ClientInfo();
        // ~ClientInfo();

        void setup();
        
    public:
        of2030::JsonFileReader m_client_cache_file;
        of2030::ConfigFile m_config_file;

        int client_id, client_index, client_count;
    };
}
#endif /* client_info_hpp */
