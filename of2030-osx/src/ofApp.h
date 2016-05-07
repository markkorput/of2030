#pragma once

#include "shared2030.h"

#include "osc_receiver.hpp"
#include "player.hpp"
#include "interface_player_bridge.hpp"
#include "renderer.hpp"
#include "client_info.hpp"
#include "xml_settings.hpp"

#ifdef __MULTI_CLIENT_ENABLED__
    #include "multi_client.hpp"
#endif

#include "ofMain.h"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit(ofEventArgs &args);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        // callbacks
        void onReconfigSettings(string &path);
        void onReconfigClients(string &path);
        void onReconfigEffects(string &path);

    private:
        of2030::OscReceiver m_oscReceiver;
        of2030::Player *m_player;
        of2030::Renderer m_renderer;
        of2030::InterfacePlayerBridge m_interface_player_bridge;
        of2030::ClientInfo *m_clientInfo;
        of2030::XmlSettings m_xmlSettings;
    
#ifdef __MULTI_CLIENT_ENABLED__
        of2030::MultiClient m_multiClient;
#endif
    
};
