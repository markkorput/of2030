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
#include "xml_settings.hpp"

namespace of2030{
    
    class MultiClient {
        SINGLETON_CLASS_HEADER_CODE(MultiClient)

    private:
        static XmlSettings* xml_settings;

    public:
        MultiClient() : enabled(false) {};
        ~MultiClient(){ destroy(); }

        void setup();
        void destroy();
        void draw();

    private:
        void drawFloor();
        //void drawDebug();
        void drawScreens();
    
    public: // attributes
        
        bool enabled;

        vector<Renderer*> m_renderers;

        ofEasyCam cam;
    };
}

#endif // #ifdef __MULTI_CLIENT_ENABLED__
#endif // multi_client_hpp
