//
//  pos.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#ifndef pos_hpp
#define pos_hpp

#include "effect.hpp"

namespace of2030 { namespace effects {
    
    class Pos : public Effect{
    public: // methods
        Pos();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
        
    public: // attributes
        
    };
    
}}

#endif /* pos_hpp */
