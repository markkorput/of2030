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
    
    typedef struct {
        int id;
        ofVec3f screenpos, screenrot;
    } XmlClient;

    
    class XmlClients{

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

#endif /* xml_settings_hpp */
