//
//  config_file.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-24.
//
//

#include "config_file.hpp"
#include <stdio.h>
#include <string.h>



using namespace of2030;


int ConfigFile::getClientCount(){
    if(!m_bLoaded)
        load();

    Json::Value val = m_json["py2030"]["clients"];
    vector<string> ids = val.getMemberNames();
    return ids.size();
}