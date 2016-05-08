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

#include "renderer.hpp"
#include "client_info.hpp"
#include "xml_settings.hpp"

namespace of2030{
    
    class MultiClient {
    public:
        static MultiClient* instance();
    private:
        static MultiClient* singleton;

    private:
        static XmlSettings* xml_settings;

    public:
        MultiClient() : enabled(false) {};
        ~MultiClient(){ destroy(); }

        void setup();
        void destroy();
        void draw();
    
    public: // attributes
        
        bool enabled;

        vector<ClientInfo*> m_client_infos;
        vector<Renderer*> m_renderers;

        ofEasyCam cam;
    };
}

#endif // #ifdef __MULTI_CLIENT_ENABLED__
#endif // multi_client_hpp
