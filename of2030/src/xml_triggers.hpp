
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

        inline string getEffectName(string &triggerName){
            std::map<string,string>::iterator it = trigger_effects.find(triggerName);
            if(it == trigger_effects.end())
                return triggerName;
            return it->second;
        }

    private:
        std::string path;
        map<string, string> trigger_effects;
    };
}

#endif /* xml_triggers_hpp */
