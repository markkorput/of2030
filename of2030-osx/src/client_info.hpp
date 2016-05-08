//
//  client_info.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef client_info_hpp
#define client_info_hpp

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

        int index, count;
        string id;
    };
}
#endif /* client_info_hpp */
