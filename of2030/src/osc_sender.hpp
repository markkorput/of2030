//
//  osc_sender.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-11.
//
//

#ifndef osc_sender_hpp
#define osc_sender_hpp

#include "shared2030.h"

#ifdef __OSC_SENDER_ENABLED__ // to speed op raspi compile

#include "ofxOsc.h"

namespace of2030{
    class OscSender{
        
        SINGLETON_INLINE_HEADER_CODE(OscSender)

    public: // methods

        OscSender() : bEnabled(false){}
        // ~OscSender(){}
        void setup(string host, int port);
        // void update();
        // void destroy();
        
        inline bool isEnabled() const { return bEnabled; }

    public: // properties

        ofxOscSender sender;

    private: // attributes

        bool bEnabled;
        
    };
}

#endif // __OSC_SENDER_ENABLED__
#endif // osc_sender_hpp
