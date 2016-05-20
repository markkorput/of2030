//
//  spot.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#ifndef spot_hpp
#define spot_hpp

#include "effect.hpp"

namespace of2030 { namespace effects {

    class Spot : public Effect{
    public: // methods
        Spot();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

    
}}

#endif /* spot_hpp */
