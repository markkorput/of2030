
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

        EffectSetting* getEffectSetting(string name);
        void setEffectSettingParam(string settingName, string paramName, string value);

    private:
        std::string path;
        vector<EffectSetting*> effect_settings;
    };
}

#endif /* xml_effects_hpp */
