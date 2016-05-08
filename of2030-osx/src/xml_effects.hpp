
#ifndef xml_effects_hpp
#define xml_effects_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    
    class XmlEffects{
    private:
        const static string path;
        const static string rootNodeName;
        const static string itemNodeName;

    public:
        static XmlEffects* instance();
    private:
        static XmlEffects* singleton;

    public:
        // XmlEffects();
        ~XmlEffects(){ destroy(); }
        void destroy();
        void load();

        XmlItemSetting* getItem(string name);
        void setItemParam(string settingName, string paramName, string value);

    private:
        vector<XmlItemSetting*> settings;
    };
}

#endif /* xml_effects_hpp */
