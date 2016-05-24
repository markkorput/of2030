//
//  map_effect.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-24.
//
//

#ifndef map_effect_hpp
#define map_effect_hpp

#include "effect.hpp"

namespace of2030 { namespace effects {
    
    class MapEffect : public Effect{
    public: // methods
         MapEffect();
        virtual void setup(Context &context);
        virtual void draw(Context &context);

    protected: // attributes
        string coordsName, maskShaderName;
        ofShader *maskShader;
    };
}}

#endif /* map_effect_hpp */
