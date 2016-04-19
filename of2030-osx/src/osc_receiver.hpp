//
//  osc_receiver.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef osc_receiver_hpp
#define osc_receiver_hpp

#include <stdio.h>
#include "ofxOsc.h"
#include "interface.hpp"

namespace of2030 {
    
    class OscReceiver {
        
    public:
        OscReceiver();
        ~OscReceiver();
        void configure(unsigned int port=2030, Interface* interface=NULL);
        void setup();
        void update();
        void destroy();

    private:
        bool connect();
        void disconnect();

    private:
        Interface* m_interface;
        ofxOscReceiver m_oscReceiver;
        bool m_bConnected;
        unsigned int m_port;
    };

}

#endif /* osc_receiver_hpp */
