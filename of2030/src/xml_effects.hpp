
#ifndef xml_effects_hpp
#define xml_effects_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    
    class XmlEffects{
    public:
        static XmlEffects* instance();
        static XmlEffects* screens();
    private:
        static XmlEffects* _screens_instance;
        static XmlEffects* _instance;

    public:
        XmlEffects();
        ~XmlEffects(){ destroy(); }
        void destroy();
        void load();

        XmlItemSetting* getItem(string name);
        void setItemParam(string settingName, string paramName, string value);

    private:
        vector<XmlItemSetting*> settings;

        string path;
        string rootNodeName;
        string itemNodeName;
    };
}

#endif /* xml_effects_hpp */
