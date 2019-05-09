#pragma once

#include "ofMain.h"
#include "tree.hh"

#define _USE_AXIDRAW_

struct Triangle {
    ofVec2f pointA;
    ofVec2f pointB;
    ofVec2f pointC;
    Boolean canGenerate;
};
class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofSerial serial;
    ofSerial serialArduino;
    string arduinoData;
    list<int> sensorVertical;
    list<int> sensorHorizontal;
    vector<ofPolyline> lines;
    vector<ofPolyline> linesCurrent;
    tree<Triangle> tr;
    tree<Triangle>::iterator top, old, loc;

    unsigned long actualTime, delayTimer;
private:
    void raiseBrush();
    void lowerBrush();
    void scanSerial();
    void readSensors();
    void setupPlotter();
    void motorsOff();
    Boolean serialOnline;
    Boolean brushDown;
    Boolean generateNext;
    void setupTriangles();
    vector<Triangle> divideTriangleByTwo(tree<Triangle>::iterator pos);
    vector<Triangle> divideTriangleByThree(tree<Triangle>::iterator pos);
    vector<Triangle> divideTriangleByFour(tree<Triangle>::iterator pos);
    string ofxGetSerialString(ofSerial &serialArduino, char until);
    string ofxTrimStringRight(string str);// trim right trailing spaces
    string ofxTrimStringLeft(string str);// trim left trailing spaces
    string ofxTrimString(string str);// trim trailing spaces
};
