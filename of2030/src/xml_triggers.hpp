
#ifndef xml_triggers_hpp
#define xml_triggers_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    
    class XmlTriggers{
    public:
        static XmlTriggers* instance();
    private:
        static XmlTriggers* singleton;

    public:
        XmlTriggers() : path("triggers.xml"){};
        // ~XmlTriggers(){ destroy(); }
        // void destroy();
        void load();

        string getEffectName(const string &triggerName);

    private:
        std::string path;
        map<string, string> trigger_effects;
    };
}

#endif /* xml_triggers_hpp */
