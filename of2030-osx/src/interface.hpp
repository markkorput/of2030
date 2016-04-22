//
//  interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef interface_hpp
#define interface_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "effects.hpp"
#include "CMSModel.h"
#include "CMSCollection.h"

namespace of2030 {
    
    class Interface {

    public:
        static Interface* instance();
 
    public: // collection
        CMS::Collection<CMS::Model> changes_collection;
        // CMS::Collection<CMS::Model> effects_collection;
    
    public: // events
        ofEvent<effects::Effect> effectEvent;

    private:
        static Interface* singleton;

    };
};

#endif /* interface_hpp */
