#include "ofApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
    ofAppGlutWindow window; // создать окно
    // набор ширина, высота, режим (OF_WINDOW or OF_FULLSCREEN)
    ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
    ofRunApp(new testApp()); // запустить приложение
  
}
