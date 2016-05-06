
#ifndef xml_clients_hpp
#define xml_clients_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    
    class XmlClients{
    public:
        static XmlClients* instance();
    private:
        static XmlClients* singleton;

    public:
        XmlClients() : path("clients.xml"){};
        ~XmlClients(){ destroy(); }
        void destroy();
        void load();
        //void save();

        std::string path;
        vector<ClientSetting*> clients;
    };
}

#endif /* xml_clients_hpp */
