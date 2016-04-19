//
//  composition.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef composition_hpp
#define composition_hpp

#include <stdio.h>
#include "CMSCollection.h"
#include "effect.hpp"

namespace of2030{
  
    class Composition{
      
    public:
        Composition(){}
        ~Composition(){}

    private:
        CMS::Collection<Effect> m_effects;
    };

};

#endif /* composition_hpp */
