
#ifndef xml_effects_hpp
#define xml_effects_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    
    class XmlEffects{
    public:
        static XmlEffects* instance();
    private:
        static XmlEffects* singleton;

    public:
        XmlEffects() : path("effects.xml"){};
        ~XmlEffects(){ destroy(); }
        void destroy();
        void load();

        std::string path;
        vector<EffectSetting*> effect_settings;
        
        EffectSetting* getEffectSetting(string name, string part);
    };
}

#endif /* xml_effects_hpp */
