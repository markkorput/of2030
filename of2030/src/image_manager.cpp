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
        images[alias] = img;
        ofLog() << alias << " loaded. Image count: " << images.size();
    }

    // return it
    return img;
}

void ImageManager::unloadAll(){
    string alias;
    // don't iterate like normal, because the iterator gets corrupted and causes
    // BAD ACCESS errors when the map gets modified during its iterations
    // we'll just take the first item every time and remove it, until there's nothing left
    while(!images.empty()){
        alias = images.begin()->first;
        unload(alias);
    }

    images.clear();
}

bool ImageManager::unload(const string &alias){
    // find specified image
    std::map<string,ofImage*>::iterator it = images.find(alias);

    // not found, abort
    if(it == images.end()){
        ofLogWarning() << "image not found for unload: " << alias;
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
    ofLog() << alias << " unloaded. Image count: " << images.size();
    return true;
}

void ImageManager::unload(ofImage *image){
    if(!image){
        return;
    }

    string alias = "";

    // find player
    for (auto& pair: images) {
        // this one?
        if(pair.second == image){
            alias = pair.first;
            break;
        }
    }

    if(alias == ""){
        ofLog() << "could not find image to unload";
        return;
    }

    // call the unload-by-alias function
    unload(alias);
}

ofImage* ImageManager::create(const string &name){
    string path = name_to_path(name);
    // ofLog() << "ImageManager::create loading: " << path;

    // does file exist?
    if(!ofFile::doesFileExist(path)){
        ofLogWarning() << "could not find image file to load: " << path;
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
