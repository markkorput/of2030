//
//  osc_receiver.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef osc_receiver_hpp
#define osc_receiver_hpp

#include "shared.h"
#include "interface.hpp"
#include "effect.hpp"

#include "ofxOsc.h"

namespace OscToolkit {
    
    class Receiver : public Interface{
        OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(Receiver)

    public: // methods
        Receiver();
        ~Receiver(){ destroy(); }

        void setup();
        void update();
        void destroy();

        void setPort(int port);
        inline bool isConnected(){ return bConnected; }

    private: // methods
        bool connect();
        void disconnect();
        
    private: // attributes
        ofxOscReceiver m_oscReceiver;
        int osc_port;
        bool bConnected;
        const static int MAX_MESSAGES_PER_CYCLE = 10;
    };
}

#endif /* osc_receiver_hpp */
