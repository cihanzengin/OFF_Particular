#include "ofApp.h"


//--------------------------------------------------------------
//----------------------  Params -------------------------------
//--------------------------------------------------------------
Params param;        //Определение глобальной переменной

void Params::setup() {
    eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 );
    eRad = 100;
    velRad = 0;
    lifeTime = 2.0;
    
    rotate = 0;
    force = 1000;
    spinning = 1000;
    friction = 0.05;
}

//--------------------------------------------------------------
//----------------------  частица ----------------------------
//--------------------------------------------------------------
Particle::Particle() {
    live = false;
}

//--------------------------------------------------------------
ofPoint randomPointInCircle( float maxRad ){
    ofPoint pnt;
    float rad = ofRandom( 0, maxRad );
    float angle = ofRandom( 0, M_TWO_PI );
    pnt.x = cos( angle ) * rad;
    pnt.y = sin( angle ) * rad;
    return pnt;
}

//--------------------------------------------------------------
void Particle::setup() {
    pos = param.eCenter + randomPointInCircle( param.eRad );
    vel = randomPointInCircle( param.velRad );
    time = 0;
    lifeTime = param.lifeTime;
    live = true;
}

//--------------------------------------------------------------
void Particle::update( float dt ){
    if ( live ) {
        //Угловая скорость
        vel.rotate( 0, 0, param.rotate * dt );
        
        ofPoint acc;         //ускорение
        ofPoint delta = pos - param.eCenter;
        float len = delta.length();
        if ( ofInRange( len, 0, param.eRad ) ) {
            delta.normalize();
            
            //привлечение/сила отталкивания
            acc += delta * param.force;
            
            //спиннинг сила
            acc.x += -delta.y * param.spinning;
            acc.y += delta.x * param.spinning;
        }
        vel += acc * dt;            //метод Эйлера
        vel *= ( 1 - param.friction );  //трение
        
        //Обновление поз
        pos += vel * dt;    //метод Эйлера
        
        //Время обновления и проверить, если частица должна умереть
        time += dt;
        if ( time >= lifeTime ) {
            live = false;   //Частиц в настоящее время считается умер
        }
    }
}

//--------------------------------------------------------------
void Particle::draw(){
    if ( live ) {
        //Вычислить размер
        float size = ofMap(
                           fabs(time - lifeTime/2), 0, lifeTime/2, 3, 1 );
        
        //вычислять цвет
        ofColor color = ofColor::white;
        float hue = ofMap( time, 255, lifeTime, 0, 255 );
        color.setHue( hue );
        ofSetColor( color );
        
        ofCircle( pos, size );  //Рисуй частицу
    }
}

//--------------------------------------------------------------
//----------------------  testApp  -----------------------------
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate( 60 );	//Установите частоту кадров экран
    
    //Выделите рисунок буфер
    int w = ofGetWidth();
    int h = ofGetHeight();
    fbo.allocate( w, h, GL_RGB32F_ARB );
    
    //Заполните буфер белого цвета
    fbo.begin();
    ofBackground(255, 255, 255);
    fbo.end();
    
    //Настройка параметров
    param.setup();		//Глобальные параметры
    history = 0.9;
    bornRate = 1500;
    
    bornCount = 0;
    time0 = ofGetElapsedTimef();
    
    //графический интерфейс пользователя
    interf.setup();
    interf.addSlider( "rate", &bornRate, 0, 3000 );
    interf.addSlider( "lifeTime", &param.lifeTime, 0, 5 );
    interf.addSlider( "history", &history, 0, 1 );
    
    interf.addSlider( "eRad", &param.eRad, 0, 800 );
    interf.addSlider( "velRad", &param.velRad, 0, 400 );
    interf.addSlider( "rotate", &param.rotate, -500, 500 );
    interf.addSlider( "spinning", &param.spinning, -1000, 1000 );
    interf.addSlider( "force", &param.force, -1000, 1000 );
    interf.addSlider( "friction", &param.friction, 0, 0.1 );
    
    drawInterface = true;
}

//--------------------------------------------------------------
void testApp::update(){
    //Вычислить DT
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;
    
    //Удалить неактивные частицы
    int i=0;
    while (i < p.size()) {
        if ( !p[i].live ) {
            p.erase( p.begin() + i );
        }
        else {
            i++;
        }
    }
    
    //Родился новые частицы
    bornCount += dt * bornRate;      //Обновление кол-ва порождаемых частиц
    if ( bornCount >= 1 ) {          //переход к созданию частиц
        int bornN = int( bornCount );//кол-во порождаемых частиц
        bornCount -= bornN;          //Корректировка значения
        for (int i=0; i<bornN; i++) {
            Particle newP;
            newP.setup();            //Запуск новой частицы
            p.push_back( newP );     //добавления этой частицы в массив
        }
    }
    
    //Обновите частиц
    for (int i=0; i<p.size(); i++) {
        p[i].update( dt );
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground( 0, 0, 30 );  //Установить фон
    
    //1. Рисование в буфер
    fbo.begin();
    
    //Нарисуйте полупрозрачные белый прямоугольник
    //немного очистки буфера (в зависимости от истории значений)
    
    ofEnableAlphaBlending();         //Включить прозрачность
    
    float alpha = (1-history) * 49;
    ofSetColor( 0, 0, 30, alpha );
    ofFill();
    ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
    
    ofDisableAlphaBlending();        //Отключить прозрачность
    
    //Нарисуйте частицы
    ofFill();
    for (int i=0; i<p.size(); i++) {
        p[i].draw();
    }
    
    fbo.end();
    
    //2. Нарисуйте буфер на экране
    ofSetColor( 255, 255, 255 );
    fbo.draw( 0, 0 );
    
    //графический интерфейс пользователя
    if ( drawInterface ) {
        //Нарисуйте текст
        ofSetColor( 255, 255, 255 );
        ofDrawBitmapString( "Keys: Enter-hide/show GUI, Space-screenshot, 1,2,...,9-load preset, Shift+1,2,...9-save preset", 20, 20 );
        ofDrawBitmapString( "Particles: " + ofToString( p.size() ), 20, 40 );
        
        //Нарисуйте ползунки
        interf.draw();
        
        //Draw излучатель в виде круга
        ofSetCircleResolution( 30 );
        ofNoFill();
        ofSetColor( 246, 23, 233 );
        ofCircle( param.eCenter, param.eRad );
        ofSetCircleResolution( 20 );
    }
}

//--------------------------------------------------------------
//----------------------  графический интерфейс пользователя ----------------------------------
//--------------------------------------------------------------
void Interface::setup(){
    selected = -1;
}

void Interface::addSlider( string title, float *value, float minV, float maxV ){
    Slider s;
    s.title = title;
    s.rect = ofRectangle( 20, 60 + slider.size() * 40, 150, 30 );
    s.value = value;
    s.minV = minV;
    s.maxV = maxV;
    slider.push_back( s );
}

void Interface::draw(){
    for (int i=0; i<slider.size(); i++) {
        Slider &s = slider[i];
        ofRectangle r = s.rect;
        ofFill();
        ofSetColor( 255, 255, 255 );
        ofRect( r );
        ofSetColor( 0, 0, 0 );
        ofNoFill();
        ofRect( r );
        ofFill();
        float w = ofMap( *s.value, s.minV, s.maxV, 0, r.width );
        ofRect( r.x, r.y + 15, w, r.height - 15 );
        ofDrawBitmapString( s.title + " " + ofToString( *s.value, 2 ), r.x + 5, r.y + 12 );
    }
}

void Interface::mousePressed( int x, int y ){
    for (int i=0; i<slider.size(); i++) {
        Slider &s = slider[i];
        ofRectangle r = s.rect;
        if ( ofInRange( x, r.x, r.x + r.width ) && ofInRange( y, r.y, r.y + r.height ) ) {
            selected = i;
            *s.value = ofMap( x, r.x, r.x + r.width, s.minV, s.maxV, true );
        }
    }
}

void Interface::mouseDragged( int x, int y ){
    if ( selected >= 0 ) {
        Slider &s = slider[selected];
        ofRectangle r = s.rect;
        *s.value = ofMap( x, r.x, r.x + r.width, s.minV, s.maxV, true );
    }
}

void Interface::mouseReleased (int x, int y ){
    selected = -1;
}

//--------------------------------------------------------------


void Interface::save( int index )
{
    vector<string> list;
    for (int i=0; i<slider.size(); i++) {
        list.push_back( ofToString( *slider[i].value ) );
    }
    string text = ofJoinString( list," " );
    string fileName = "preset" + ofToString( index ) + ".txt";
    ofBuffer buffer = ofBuffer( text );
    ofBufferToFile( fileName, buffer );
}

//--------------------------------------------------------------
void Interface::load( int index )
{
    string fileName = "preset" + ofToString( index ) + ".txt";
    string text = string( ofBufferFromFile( fileName ) );
    vector<string> list = ofSplitString( text, " " );
    
    if ( list.size() == slider.size() ) {
        for (int i=0; i<slider.size(); i++) {
            *slider[i].value = ofToFloat( list[i] );
        }
    }
}

//--------------------------------------------------------------
//----------------------  testApp again  -----------------------
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == OF_KEY_RETURN ) {	//Скрыть / показать графический интерфейс пользователя
        drawInterface = !drawInterface;
    }
    
    if ( key == ' ' ) {		//Возьмите изображение с монитора в файл
        ofImage image;
        image.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
        
        //Выберите случайное имя файла
        int n = ofRandom( 0, 1000 );
        string fileName = "screen" + ofToString( n ) + ".png";
        
        image.saveImage( fileName );
    }
    
    //загружать пресеты
    if ( key == '1' ) { interf.load( 1 ); }
    if ( key == '2' ) { interf.load( 2 ); }
    if ( key == '3' ) { interf.load( 3 ); }
    if ( key == '4' ) { interf.load( 4 ); }
    if ( key == '5' ) { interf.load( 5 ); }
    if ( key == '6' ) { interf.load( 6 ); }
    if ( key == '7' ) { interf.load( 7 ); }
    if ( key == '8' ) { interf.load( 8 ); }
    if ( key == '9' ) { interf.load( 9 ); }
    
    //Сохранить пресеты
    if ( key == '!' ) { interf.save( 1 ); }
    if ( key == '@' ) { interf.save( 2 ); }
    if ( key == '#' ) { interf.save( 3 ); }
    if ( key == '$' ) { interf.save( 4 ); }
    if ( key == '%' ) { interf.save( 5 ); }
    if ( key == '^' ) { interf.save( 6 ); }
    if ( key == '&' ) { interf.save( 7 ); }
    if ( key == '*' ) { interf.save( 8 ); }
    if ( key == '(' ) { interf.save( 9 ); }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if ( drawInterface ) {
        interf.mouseDragged( x, y );     
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if ( drawInterface ) {
        interf.mousePressed( x, y );     
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    interf.mouseReleased( x, y );    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
