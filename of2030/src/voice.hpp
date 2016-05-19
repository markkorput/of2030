//
//  voice.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#ifndef voice_hpp
#define voice_hpp

#include "effect.hpp"

namespace of2030 { namespace effects {

    class Voice : public Effect{
    public: // methods
        Voice();
        virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

}}
#endif /* voice_hpp */
