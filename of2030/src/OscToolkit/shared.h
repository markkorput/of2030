//
//  shared.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#ifndef shared_h
#define shared_h

#define OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(x) \
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

#define OSC_TOOLKIT_SINGLETON_INLINE_IMPLEMENTATION_CODE(x) \
    x* x::singleton = NULL;

#endif /* shared_hpp */
