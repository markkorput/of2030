//
//  image_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-05.
//
//

#ifndef image_manager_hpp
#define image_manager_hpp

#include "ofMain.h"
#include "shared2030.h"

namespace of2030 {
    class ImageManager {

        SINGLETON_INLINE_HEADER_CODE(ImageManager)
        
    public: // methods/interface
        ImageManager();
        inline ~ImageManager(){ destroy(); }

        //void setup();
        // void update();
        void destroy();

        ofImage* get(const string &name, bool load=true);
        ofImage* get(const string &name, const string &alias, bool load=true);

        bool unload(const string &name);
        void unload(ofImage* image);
        
    public: // events
        
        ofEvent<ofImage> unloadEvent;

    protected: // helper methods

        inline string name_to_path(const string &name) const { return folder_path + name; }
        ofImage* create(const string &name);

    private: // attributes
        map<string, ofImage*> images;
        string folder_path;
    };
}



#endif /* image_manager_hpp */
