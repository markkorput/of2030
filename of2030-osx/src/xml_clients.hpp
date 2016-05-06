
#ifndef xml_clients_hpp
#define xml_clients_hpp

#include "ofMain.h"

namespace of2030{
    
    typedef struct{
        int id;
        ofVec3f screenpos, screenrot;
    } XmlClient;

    
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
        vector<XmlClient*> xml_clients;
    };
}

#endif /* xml_clients_hpp */
