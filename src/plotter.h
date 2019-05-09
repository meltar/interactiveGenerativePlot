//
//  plotter.h
//  interactiveGenerativePlot
//
//  Created by Melissa Holmes on 5/7/19.
//
#include "ofMain.h"
#include "ofSerial.h"

#ifndef plotter_h
#define plotter_h


#endif /* plotter_h */

class Plotter {

    float motorSpeed;
    int servoUpPct;
    int servoPaintPct;
    Boolean reverseMotorX;
    Boolean reverseMotorY;
    int delayAfterRaisingBrush;
    int delayAfterLoweringBrush;
    Boolean debugMode;
    Boolean paperSizeA4;
    Boolean segmentQueued = false;
//    PVector queuePt1 = new PVector(-1, -1);
//    PVector queuePt2 = new PVector(-1, -1);
    float motorStepsPerPixel;
    float pixelsPerInch;

    int mousePaperLeft;
    int mousePaperRight;
    int mousePaperTop;
    int mousePaperBottom;

    int yBrushRestPositionPixels = 6;
    int servoUp;
    int servoPaint;
    int motorMinX;
    int motorMinY;
    int motorMaxX;
    int motorMaxY;
//    ofColor black;
//    ofColor PenColor;
    Boolean firstPath;
    Boolean doSerialConnect = true;
    Boolean serialOnline;
    ofSerial serial;
    int val;
    Boolean brushDown;
    Boolean brushDownAtPause;
    Boolean drawingPath;
    int xLocAtPause;
    int yLocAtPause;
    int motorX;
    int motorY;
    int motorLocatorX;
    int motorLocatorY;
//    PVector lastPosition;
    Boolean forceRedraw;
    Boolean shiftKeyDown;
    Boolean keyup;
    Boolean keyright;
    Boolean keyleft;
    Boolean keydown;
    Boolean hKeyDown;
    int lastButtonUpdateX;
    int lastButtonUpdateY;
    Boolean lastBrushDown_DrawingPath;
    int lastX_DrawingPath;
    int lastY_DrawingPath;
    int nextMoveTime;
    int subsequentWaitTime;
    int UIMessageExpire;
    int raiseBrushStatus;
    int lowerBrushStatus;
    int moveStatus;
    int moveDestX;
    int moveDestY;
    int paintDest;
    Boolean paused;
//    PVector[] ToDoList;  // Queue future events in an array; Coordinate/command
    int indexDone;
    int indexDrawn;
    
    public:
    Plotter();
    ~Plotter();
    void connect();
    void setup();
    void raiseBrush();
    void lowerBrush();
    void motorsOff();
    void moveRelativeXY(int xD, int yD);
    void moveToXY(int xLoc, int yLoc);
    void moveToXY();
    ofVec2f getMotorPixelPos();
    float getDistance(int x1, int y1, int x2, int y2);
    private:
};
