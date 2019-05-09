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

    float MotorSpeed;
    int ServoUpPct;
    int ServoPaintPct;
    Boolean reverseMotorX;
    Boolean reverseMotorY;
    int delayAfterRaisingBrush;
    int delayAfterLoweringBrush;
    Boolean debugMode;
    Boolean PaperSizeA4;
    Boolean segmentQueued = false;
//    PVector queuePt1 = new PVector(-1, -1);
//    PVector queuePt2 = new PVector(-1, -1);
    float MotorStepsPerPixel;
    float PixelsPerInch;
    int yBrushRestPositionPixels = 6;
    int ServoUp;
    int ServoPaint;
    int MotorMinX;
    int MotorMinY;
    int MotorMaxX;
    int MotorMaxY;
//    ofColor black;
//    ofColor PenColor;
    Boolean firstPath;
    Boolean doSerialConnect = true;
    Boolean serialOnline;
    ofSerial serial;
    int val;
    Boolean brushDown;
    Boolean BrushDownAtPause;
    Boolean DrawingPath;
    int xLocAtPause;
    int yLocAtPause;
    int MotorX;
    int MotorY;
    int MotorLocatorX;
    int MotorLocatorY;
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
    int NextMoveTime;
    int SubsequentWaitTime;
    int UIMessageExpire;
    int raiseBrushStatus;
    int lowerBrushStatus;
    int moveStatus;
    int MoveDestX;
    int MoveDestY;
    int PaintDest;
    Boolean Paused;
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
    private:
};
