//
//  client_info.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef client_info_hpp
#define client_info_hpp

#include "xml_clients.hpp"
#include "setting_types.h"

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
        void setClientId(string cid);
        inline ClientSetting* getClientSetting(){ return client_setting; }

    private:
        ClientSetting* findClientSetting();

    private:
        ClientSetting placeholderXmlClient;
        ClientSetting* client_setting;

        int index, count;
        string id;
    };
}
#endif /* client_info_hpp */
