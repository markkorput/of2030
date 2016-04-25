//
//  client_instance.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef client_instance_hpp
#define client_instance_hpp

//#include <stdio.h>
#include "renderer.hpp"
#include "client_info.hpp"


namespace of2030{
    class ClientInstance{
    
    public:
        ClientInstance();
        ~ClientInstance(){ destroy(); }

        void setup();
        // void update();
        // void draw();
        void destroy();

        Renderer *m_renderer;
        bool m_bPrivateRenderer;
        ClientInfo m_clientInfo;
    };
}
#endif /* client_instance_hpp */
