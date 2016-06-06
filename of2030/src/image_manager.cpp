//
//  image_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-05.
//
//

#include "image_manager.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(ImageManager)


ImageManager::ImageManager(){
    folder_path = "images/";
}

void ImageManager::destroy(){
    for (std::map<string,ofImage*>::iterator it=images.begin(); it!=images.end(); ++it){
        unload(it->first);
    }

    images.clear();
}

ofImage* ImageManager::get(const string &name, bool load){
    // assume alias IS the video's path
    return get(name, name, load);
}

ofImage* ImageManager::get(const string &name, const string &alias, bool load){
    std::map<string,ofImage*>::iterator it = images.find(alias);

    // found it
    if(it != images.end()){
        return it->second;
    }

    // not found, no loading
    if(!load)
        return NULL;
    
    // (try to) create a player
    ofImage* img = create(name);
    
    // store it
    if(img){
        ofLog() << alias << " loaded.";
        images[alias] = img;
    }

    // return it
    return img;
}

bool ImageManager::unload(const string &alias){
    ofLog() << "ImageManager::unload with " << alias;

    // No specific player specified?
    if(alias == ""){
        // destroy (unload all)
        destroy();
        return true;
    }

    // find specified image
    std::map<string,ofImage*>::iterator it = images.find(alias);

    // not found, abort
    if(it == images.end()){
        ofLogWarning() << "ImageManager::unload image not found";
        return false;
    }

    ofNotifyEvent(unloadEvent, *it->second, this);

    // remove from our list
    images.erase(it);
    
    if(it->second){
        // delete instance from memory
        delete it->second;
    }
    
    // log and report
    ofLog() << "Images still loaded: " << images.size();
    return true;
}

void ImageManager::unload(ofImage *image){
    if(!image){
        return;
    }

    // find player
    for (auto& pair: images) {
        // this one?
        if(pair.second == image){
            // call the unload-by-alias function
            unload(pair.first);
        }
    }
}

ofImage* ImageManager::create(const string &name){
    string path = name_to_path(name);
    ofLog() << "ImageManager::create loading: " << path;

    // does file exist?
    if(!ofFile::doesFileExist(path)){
        ofLogWarning() << "could not find image file";
        return NULL;
    }

    // create ofImage instance and load image data
    ofImage* img = new ofImage;
    // TODO; use ofxThreadedImageLoader?
    if(img->load(path)){
        return img;
    }

    ofLogWarning() << "ofImage.load failed";
    delete img;
    return NULL;
}
