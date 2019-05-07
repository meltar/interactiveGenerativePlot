//
//  triangle.h
//  interactiveGenerativePlot
//
//  Created by Melissa Holmes on 5/7/19.
//

#ifndef triangle_h
#define triangle_h


#endif /* triangle_h */
//
//  WorkSurface3D.h
//  example-surface-following
//
//  Created by mad on 4/26/16.
//
//

#pragma once
#include "ofMain.h"

class Triangle {
    ofPoint a;
    ofPoint b;
    ofPoint c;
    vector<ofPoint> divided;
    int depth;
    int threshold;
public:
    Triangle(ofPoint pointA, ofPoint pointB, ofPoint pointC, int parentDepth);
    ~Triangle();

    void divide();
    void divide(int subTriangles);
    bool canDivide();
    vector<ofPoint> getPoints();
    vector<ofPoint> getNewLines();
    vector<Triangle> getNewTriangles();

private:


};
