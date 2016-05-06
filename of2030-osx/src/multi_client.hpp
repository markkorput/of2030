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
#include "xml_settings.hpp"
#include "renderer.hpp"
#include "client_info.hpp"

namespace of2030{
    
    class MultiClient {
    public:
        MultiClient() : enabled(false), m_scaleFactor(0.5f){};
        ~MultiClient(){ destroy(); }
        
        void setup();
        void destroy();
        void load(XmlSettings &xml);
        void draw();
        
        ofPoint getTotalSize();
        
        bool enabled;

        XmlSettings *m_xml;
        vector<ClientInfo*> m_client_infos;
        vector<Renderer*> m_renderers;
        float m_scaleFactor;

        ofEasyCam cam;
    };
}

#endif /* multi_client_hpp */
