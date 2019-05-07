//
//  triangle.cpp
//  interactiveGenerativePlot
//
//  Created by Melissa Holmes on 5/7/19.
//

#include <stdio.h>
#include <triangle.h>

Triangle::Triangle(ofPoint pointA, ofPoint pointB, ofPoint pointC, int parentDepth){
    depth = parentDepth - 1;
    threshold = 12;
    a = pointA;
    b = pointB;
    c = pointC;
    divide();
}

Triangle::~Triangle(){
}

void Triangle::divide(){
    // TODO: also support?
    // divide(2);
    
    divide(4);
}

void Triangle::divide(int subTriangles){
    // create a new point in the center of each line
    // draw lines between each new point (3 lines)
    // create 4 new triangles?
    // return them?
}

bool Triangle::canDivide(){
    return threshold >= depth;
}

vector<ofPoint> Triangle::getPoints(){
    vector<ofPoint> points;
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    return points;
}

vector<ofPoint> Triangle::getNewLines(){
    return divided;
}

vector<Triangle> Triangle::getNewTriangles(){

}
