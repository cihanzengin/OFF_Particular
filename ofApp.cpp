#include "ofApp.h"

class Particle {
public:
    Particle();
    void setup();
    void update( float dt );
    void draw();
    //Class constructor
    //Start particle
    //Recalculate physics
    //Draw particle
    //Position
    //Velocity
    //Time of living
    //Allowed lifetime
    //Is particle live
    ofPoint pos;
    ofPoint vel;
    float time;
    float lifeTime;
    bool live;
    
};


Params param;        //Definition of global variable
void Params::setup() {
    eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 );
    eRad = 50;
    velRad = 200;
    lifeTime = 1.0;
    rotate = 90; }

Particle::Particle() {
    live = false;
}

ofPoint randomPointInCircle( float maxRad ){
    ofPoint pnt;
    float rad = ofRandom( 0, maxRad );
    float angle = ofRandom( 0, M_TWO_PI );
    pnt.x = cos( angle ) * rad;
    pnt.y = sin( angle ) * rad;
    return pnt; }

// ---- here we will go to universe!!
void Particle::setup() {
    pos = param.eCenter + randomPointInCircle( param.eRad );
    vel = randomPointInCircle( param.velRad );
    time = 0;
    lifeTime = param.lifeTime;
    live = true;
}

void Particle::update( float dt ){
    if ( live ) {
        //Rotate vel
        vel.rotate( 0, 0, param.rotate * dt );
        //Update pos
        pos += vel * dt;    //Euler method
        //Update time and check if particle should die
        time += dt;
        if ( time >= lifeTime ) {
            live = false;   //Particle is now considered as died
        }
    } }


void Particle::draw(){
    if ( live ) {
        //Compute size
        float size = ofMap(
                           fabs(time - lifeTime/2), 0, lifeTime/2, 3, 1 );
        //Compute color
        ofColor color = ofColor::red;
        float hue = ofMap( time, 0, lifeTime, 128, 255 );
        color.setHue( hue );
        ofSetColor( color );
        ofCircle( pos, size );  //Draw particle
    }
}









//--------------------------------------------------------------
void ofApp::setup(){
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetFrameRate( 60 );    //Set screen frame rate
    //Allocate drawing buffer
    int w = ofGetWidth();
    int h = ofGetHeight();
    fbo.allocate( w, h, GL_RGB32F_ARB );
    //Fill buffer with white color
    fbo.begin();
    ofBackground(255, 255, 255);
    fbo.end();
    //Set up parameters
    param.setup();          //Global parameters
    history = 0.995;
    time0 = ofGetElapsedTimef();
    
    //Compute dt
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;
    //If the particle is not active - activate it
    if ( !p.live ) {
        p.setup(); }
    //Update the particle
    p.update( dt );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( 255, 255, 255 );  //Set white background
    //1. Drawing to buffer
    fbo.begin();
    
    ofEnableAlphaBlending();         //Enable transparency
    float alpha = (1-history) * 255;
    ofSetColor( 255, 255, 255, alpha );
    ofFill();
    ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
      ofDisableAlphaBlending();
    
    ofFill();
    p.draw();
    
    fbo.end();
    
    ofSetColor(255, 255, 255);
    fbo.draw(0, 0);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
