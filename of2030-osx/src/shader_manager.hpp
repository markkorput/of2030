//
//  shader_manager.hpp
//  raspi2030
//
//  Created by Mark van de Korput on 16-05-04.
//
//

#ifndef shader_manager_hpp
#define shader_manager_hpp

#include "ofMain.h"

namespace of2030 {
    class ShaderManager {

    public: // methods/interface

        static ShaderManager* instance();
        static void destroyInstance();

        ShaderManager();
        ~ShaderManager();

        //void setup();
        //void update();
        void destroy();

        ofShader* load(string name, ofShader* pShader=NULL);
        ofShader* get(string name, bool load=true);

    protected:
        string name_to_path(string name);

    private: // attributes
        static ShaderManager* singleton;
        map<string, ofShader*> shaders;
        string folder_path;
    };
}

#endif /* video_manager_hpp */
