#include "ofMain.h"
#include "ofApp.h"

#include "shared2030.h"

//========================================================================
int main(int argc, char** argv){

#ifdef __APPLE__
  ofSetupOpenGL(768,576, OF_WINDOW);      // <-------- setup the GL context
#else // raspbery pi
  // ofGLWindowSettings settings;
  // settings.setGLVersion(3,2);
  ofGLESWindowSettings settings;
  settings.glesVersion = 2;
  ofCreateWindow(settings);
  // ofSetupOpenGL(1024,768, OF_WINDOW); // OF_FULLSCREEN
#endif // __APPLE__

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new ofApp());
}
