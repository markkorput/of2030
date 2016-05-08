//
//  xml_screens.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-08.
//
//

#ifndef xml_screens_hpp
#define xml_screens_hpp

#include "xml_effects.hpp"

namespace of2030{
    class XmlScreens : public XmlEffects{
    private:
        const static string path;
        const static string rootNodeName;
        const static string itemNodeName;

    public:
        static XmlScreens* instance();
    private:
        static XmlScreens* singleton;
    };
}

#endif /* xml_screens_hpp */
