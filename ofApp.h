#pragma once


#include "ofMain.h"

//класс частиц
class Particle {
public:
    Particle();                //конструктор класса
    void setup();              //Начать частиц
    void update( float dt );   //Пересчитать физику
    void draw();               //Рисовать частиц
    
    ofPoint pos;               //позиция
    ofPoint vel;               //скорость
    float time;                //Время жизни
    float lifeTime;            //Разрешено жизни
    bool live;                 //Живая частица
};

//Параметры контроля класс
class Params {
public:
    void setup();
    ofPoint eCenter;    //центр излучателя
    float eRad;         //радиус излучателя
    float velRad;       //Первоначальный предел скорости
    float lifeTime;     //Срок службы в секундах
    
    float rotate;   //Направление скорости вращения в углы в секунду
    
    float force;       //Привлечение / сила отталкивания внутри излучателя
    float spinning;    //Спиннинг силу внутри излучателя
    float friction;    //Трение в диапазоне [0, 1]
};

extern Params param; //Декларация глобальной переменной


//------------ графический интерфейс пользователя --------------
//класс слайдер
class Slider {
public:
    string title;		//название
    ofRectangle rect;	//Прямоугольник для рисования
    float *value;       //Указатель на значение, которое слайдер changes
    float minV, maxV;   //Минимальные и максимальные значения
};

//Интерфейсный класс, который управляет слайдеры
class Interface {
public:
    void setup();
    void addSlider( string title, float *value, float minV, float maxV );
    void draw();
    
    void save( int index );		//Сохранить пресет
    void load( int index );		//загрузить пресет
    
    void mousePressed( int x, int y );
    void mouseDragged( int x, int y );
    void mouseReleased (int x, int y );
    
    vector<Slider> slider;	//Массив слайдеры
    int selected;			//Индекс выбранную слайдер
};
//-------------------------------

//openFrameworks' класс
class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    vector<Particle> p;	  //Частицы
    ofFbo fbo;            //Буфер Off-экран для трасс
    
    float history;        //Контрольный параметр для трасс
    float time0;          //Значение времени для вычисления DT
    
    float bornRate;       //Частицы рождаются ставка за секунду
    float bornCount;      //Встроенный число частиц родиться
    
    //графический интерфейс пользователя
    Interface interf;
    bool drawInterface;
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
};
