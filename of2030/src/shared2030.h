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
    #define __MULTI_CLIENT_ENABLED__
    #define __OSC_RECORDER_ENABLED__
    #define __BOOL_OSC_SETUP__
    #define __DRAGNDROP__ 
#else
//    #define __SET_DATA_ROOT_PATH__
    #define __HIDE_CURSOR__
#endif

#define __RENDER_TO_FBO_FIRST__
// #define __AUTO_UNLOAD_VIDEOS_WHEN_EFFECTS_END__
// #define __EXTENDED_EFFECT_CONFIG__

#define CTRL_RELOAD_CLIENTS "reload_clients"
#define CTRL_RELOAD_SCREENS "reload_screens"
#define CTRL_RELOAD_SETTINGS "reload_settings"
#define CTRL_RELOAD_EFFECTS "reload_effects"
#define CTRL_RELOAD_SHADERS "reload_shaders"

// singleton conveniece macros
#define SINGLETON_CLASS_HEADER_CODE(x) \
    public: \
    static x* instance();\
    static void delete_instance(); \
    private:\
    static x* singleton;

#define SINGLETON_CLASS_IMPLEMENTATION_CODE(x) \
    x* x::singleton = NULL;\
    x* x::instance(){\
        if(!singleton){\
            ofLogVerbose() << "Creating singleton of class " << #x;\
            singleton = new x();\
        }\
        return singleton;\
    }\
    void x::delete_instance(){\
        if(singleton){\
            delete singleton;\
            singleton = NULL;\
        }\
    }

#define SINGLETON_INLINE_HEADER_CODE(x) \
    public: \
        inline static x* instance(){\
            if(!singleton){\
                ofLogVerbose() << "Creating singleton of class " << #x;\
                singleton = new x();\
            }\
            return singleton;\
        }\
        inline static void delete_instance(){ \
            if(singleton){\
                delete singleton;\
                singleton = NULL;\
            }\
        }\
    private:\
        static x* singleton;

#define SINGLETON_INLINE_IMPLEMENTATION_CODE(x) \
    x* x::singleton = NULL;

#endif /* shared2030_h */
