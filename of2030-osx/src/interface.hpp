//
//  interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef interface_hpp
#define interface_hpp

#include <stdio.h>

#include "CMSModel.h"
#include "CMSCollection.h"

namespace of2030 {
    
    class Interface {

    public:
        static Interface* instance();
 
    public:
        CMS::Collection<CMS::Model> changes_collection;
        CMS::Collection<CMS::Model> effects_collection;

    private:
        static Interface* singleton;

    };
};

#endif /* interface_hpp */
