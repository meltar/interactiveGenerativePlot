#pragma once

#include "ofMain.h"
#include "tree.hh"

#define _USE_AXIDRAW_

struct Triangle {
    ofVec2f pointA;
    ofVec2f pointB;
    ofVec2f pointC;
    
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
    bool ready;
    vector<ofPolyline> lines;
    vector<ofPolyline> linesCurrent;
//    vector<Triangle> triangles;
private:
    void raiseBrush();
    void lowerBrush();
    void scanSerial();
    void setupPlotter();
    void motorsOff();
    Boolean serialOnline;
    Boolean brushDown;
    void setupTriangles();
};
