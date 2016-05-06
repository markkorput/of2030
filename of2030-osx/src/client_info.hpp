//
//  client_info.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef client_info_hpp
#define client_info_hpp

// #include "json_file_reader.hpp"
// #include "config_file.hpp"
#include "xml_settings.hpp"

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
        void copy(ClientInfo &other);
        void setClientId(int cid);
        
    protected:
        
        // void updateClientIndex();

    public:
        // of2030::JsonFileReader m_client_cache_file;
        // ConfigFile m_config_file;
        XmlSettings m_xml_settings;

        int id, index, count;
    };
}
#endif /* client_info_hpp */
