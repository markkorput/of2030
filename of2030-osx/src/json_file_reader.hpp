//
//  json_file_reader.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#ifndef json_file_reader_hpp
#define json_file_reader_hpp

#include "ofMain.h"
#include "ofxJSONElement.h"

namespace of2030{
    
    class JsonFileReader {
        
    public:
        JsonFileReader();
        // ~JsonFileReader();
        
        void setPath(string path);
        void load();
        string getValue(std::string valuePath);
        
        
    protected: // atributes
        
        bool m_bLoaded;
        ofxJSONElement m_json;
        string m_path;
    };
}

#endif /* json_file_reader_hpp */
