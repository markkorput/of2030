//
//  client_setting.h
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-06.
//
//

#ifndef client_setting_h
#define client_setting_h

namespace of2030{
    typedef struct{
        int id;
        ofVec3f screenpos, screenrot;
        ofVec2f screensize;
        
        float pano_start, pano_end;
        ofVec3f pano_rot;
    } ClientSetting;
}

#endif /* client_settings_h */
