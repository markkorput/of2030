//
//  multi_client.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef multi_client_hpp
#define multi_client_hpp

//#include <stdio.h>
#include "shared2030.h"

#ifdef __MULTI_CLIENT_ENABLED__

#include "xml_settings.hpp"
#include "renderer.hpp"
#include "client_info.hpp"


namespace of2030{
    
    class MultiClient {
    public:
        MultiClient() : enabled(false), bSetup(false) {};
        ~MultiClient(){ destroy(); }
        
        void setup();
        void destroy();
        void load(XmlSettings &xml);
        void draw();
    
    public: // attributes
        
        bool enabled;

        XmlSettings *m_xml;
        vector<ClientInfo*> m_client_infos;
        vector<Renderer*> m_renderers;

        ofEasyCam cam;
    
    private: // attributes
        
        bool bSetup;
    };
}

#endif // #ifdef __MULTI_CLIENT_ENABLED__
#endif // multi_client_hpp
