
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

    public: // methods
        XmlEffects();
        ~XmlEffects(){ destroy(); }
        void destroy();
        void load(bool reload=false);

        bool isLoaded(){ return settings.size() > 0; }

    public: // getters/setters
        XmlItemSetting* getItem(string name);
        void setItemParam(string settingName, string paramName, string value);
        void setNameFilter(const string &filter);

    private: // attributes
        vector<XmlItemSetting*> settings;

        bool bLoaded;
        string path;
        string nameFilter;
        string rootNodeName;
        string itemNodeName;
    };
}

#endif /* xml_effects_hpp */
