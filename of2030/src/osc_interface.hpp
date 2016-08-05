//
//  osc_interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-04.
//
//

#ifndef osc_interface_hpp
#define osc_interface_hpp

#include "shared2030.h"

#include "ofxOsc.h"

namespace of2030 {
    class OscInterface {
        SINGLETON_INLINE_HEADER_CODE(OscInterface)

    public: // method: input
        inline void process(ofxOscMessage &message){
            ofNotifyEvent(messageEvent, message, this);
        }

    public: // event: output
        ofEvent<ofxOscMessage> messageEvent; // triggered by every possible source (also recording playback)
    };
};

#endif /* osc_interface_hpp */
