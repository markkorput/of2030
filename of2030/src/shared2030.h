//
//  shared2030.h
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-07.
//
//

#ifndef shared2030_h
#define shared2030_h

#ifdef __APPLE__
    #define __MULTI_CLIENT_ENABLED__ (true)
    #define __BOOL_OSC_SETUP__ (true)
#endif

#define CTRL_RELOAD_CLIENTS "reload_clients"
#define CTRL_RELOAD_SCREENS "reload_screens"
#define CTRL_RELOAD_SETTINGS "reload_settings"
#define CTRL_RELOAD_EFFECTS "reload_effects"
#define CTRL_RELOAD_SHADERS "reload_shaders"

// singleton conveniece macros
#define SINGLETON_CLASS_HEADER_CODE(x) \
    public: \
    static x* instance();\
    private:\
    static x* singleton;

#define SINGLETON_CLASS_IMPLEMENTATION_CODE(x) \
    x* x::singleton = NULL;\
    x* x::instance(){\
        if(!singleton)\
            singleton = new x();\
        return singleton;\
    }

#endif /* shared2030_h */
