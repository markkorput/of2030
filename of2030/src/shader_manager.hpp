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
#include "shared2030.h"

namespace of2030 {
    class ShaderManager {
        SINGLETON_INLINE_HEADER_CODE(ShaderManager)

    public: // methods/interface
        ShaderManager();
        ~ShaderManager();

        //void setup();
        //void update();
        void destroy();
        void clear();

        ofShader* load(string name, ofShader* pShader=NULL);
        ofShader* get(string name, bool load=true);

    protected:
        string name_to_path(string name);
        string name_to_path_type(string name, string type);
        
    private: // attributes
        map<string, ofShader*> shaders;
        string folder_path;
    };
}

#endif /* video_manager_hpp */
