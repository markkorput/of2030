//
//  shader_manager.cpp
//  raspi2030
//
//  Created by Mark van de Korput on 16-05-04.
//
//

#include "shader_manager.hpp"

using namespace of2030;

ShaderManager* ShaderManager::singleton = NULL;

ShaderManager* ShaderManager::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new ShaderManager();
    }
    return singleton;
}

void ShaderManager::destroyInstance(){
  if (singleton){
    delete singleton;
    singleton = NULL;
  }
}

ShaderManager::ShaderManager(){
  #ifdef TARGET_OPENGLES
    folder_path = "shadersES2/";
  #else
    if(ofIsGLProgrammableRenderer())
      folder_path = "shadersGL3/";
    else
      folder_path = "shadersGL2/";
  #endif

  ofLogNotice(__func__) << "using shader folder: " << folder_path;
}

ShaderManager::~ShaderManager(){
    // while(!shaders.empty()){
    //     // get (pointer to) last shader from the list
    //     ofShader* shader = shaders[-1];
    //     // remove pointer from the list
    //     shaders.pop_back();
    //     // remove the shader from memory
    //     delete shaders;
    // }
    destroy();
}

// void ShaderManager::setup(){
//
// }

// void ShaderManager::update(){
//
// }

void ShaderManager::destroy(){
  // loop over all loaded shader and remove them from memory
  for (auto& it: shaders) {
    // unload (not necessary?)
    // it.second->unload()
    delete it.second;
  }

  // clear our internal map of loaded shaders
  shaders.clear();
}

ofShader* ShaderManager::load(string name, ofShader* pShader){
    // get shader path for the provided shader name
    string path = name_to_path(name);
    // allocate new shader object
    ofShader *shader = pShader;
    if(shader == NULL) shader = new ofShader;
    // load vert and fragment shader source files
    if(!shader->load(path+".vert", path+".frag"))
        shader->unload();
    // store shader object for later reference
    shaders[name] = shader;
    // log activity
    ofLogNotice(__func__) << "shaders loaded: " << path;
    // return the ofShader pointer
    return shader;
}

ofShader* ShaderManager::get(string name, bool load){
    ofShader *pExisting = NULL;
    
    // try to find already-loaded shader by name
    std::map<string, ofShader*>::iterator it = shaders.find(name);
    if (it != shaders.end()){ // found!
        if(it->second->isLoaded())
            return it->second;
        // already allocated but failed to load previously
        pExisting = it->second;
    }

    // load new shader?
    if(load){
        return this->load(name, pExisting);
    }

    // not found
    return NULL;
}

string ShaderManager::name_to_path(string video_name){
    return folder_path + video_name;
}
