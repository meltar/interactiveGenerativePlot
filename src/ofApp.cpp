#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    delayTimer = ofGetElapsedTimeMillis();
    generateNext = false;

    // Connect AxiDraw automatically (declaration in h file)
    // Connect Arduino
    #ifdef _CONNECT_TO_ARDUINO_
        scanSerial();
    #endif


    ofBackground(0);
//    ofColor(255, 0, 130);
    ofColor(127);
//    ofSeedRandom(0);

//    ofPolyline outline;
//    outline.addVertex(0,0);
//    outline.addVertex(0, ofGetHeight());
//    outline.addVertex(ofGetWidth(), ofGetHeight());
//    outline.addVertex(ofGetWidth(), 0);
//    outline.addVertex(0,0);
//    lines.push_back(outline);
//    linesCurrent.push_back(outline);

    setupTriangles();
    maxTime = 120000;
}

//--------------------------------------------------------------
void ofApp::update(){
    #ifdef _CONNECT_TO_ARDUINO_
        readSensors();
    #endif


    // wait between generation calls
    actualTime = ofGetElapsedTimeMillis();
//    if (actualTime < maxTime) {
        if (true) {
        if (actualTime - delayTimer >= 20) {
    //    generateNext = true;

        if (generateNext) {
            generateNext = true;
            delayTimer = ofGetElapsedTimeMillis();
            // Iterate through triangle tree leaf nodes
            tree<Triangle>::leaf_iterator iter=tr.begin_leaf();
            while (tr.is_valid(iter)) {
                if (tr.depth(iter) < 15 && (*iter).canGenerate){
                    vector<Triangle> results;
                    int type = ofRandom(8);
                    switch (type){
                        case 0:
                        case 1:
                        case 2:
                            results = divideTriangleByTwo(iter);
                            break;
                        case 3:
                        case 4:
                            results = divideTriangleByFour(iter);
                            break;
                        case 5:
                            results = nestTriangle(iter);
                            break;
    //                    case 3:
    //                        results = divideTriangleByThree(iter);
    //                        break;
//                        case 2:
//                            (*iter).canGenerate = false;
//                            break;
                        default:
                            break;
                    }

                    // use the current iterator location for appending children
                    tree<Triangle>::leaf_iterator append_iter = iter;

                    // advance to the next node
                    iter++;

                    // add new generated triangles
                    for (int i = 0; i < results.size(); i++) {
                        if (tr.depth(append_iter) > 4 && ofRandom(100) < 5) {
                            results[i].canGenerate = false;
                        }
                        if (tr.depth(append_iter) > 12) {
                            results[i].canGenerate = false;
                        }
                        tr.append_child(append_iter, results[i]);
                    }
                } else {
                    iter++;
                }
            }
        }
            if (!linesCurrent.empty()) {
                ofPolyline current = linesCurrent.front();
                lines.push_back(current);
                linesCurrent.erase(linesCurrent.begin());
                if (actualTime > 60000){
                    current = linesCurrent.front();
                    lines.push_back(current);
                    linesCurrent.erase(linesCurrent.begin());
                    ofPolyline current = linesCurrent.front();
                    lines.push_back(current);
                    linesCurrent.erase(linesCurrent.begin());
                }
            }
        }
    } else {
        maxTime += 120000;
        delayTimer = ofGetElapsedTimeMillis();
        lines.clear();
        linesCurrent.clear();
        tr.clear();
//        iter = tr.begin_leaf();
        setupTriangles();
        ofSleepMillis(1000);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

//    ofColor(255, 0, 130);
//    ofBeginSaveScreenAsSVG("output.svg");
    // draw all lines on the screen
    for(int i = 0; i < lines.size(); i++){
//        ofSleepMillis(500);
//        ofSetColor(127 + i/75, 50, 25 + i/100);
//        if(i % 5 == 0){
//            ofSetColor(255, 100, 140);
//        } else {
//            ofSetColor(127 + lines.size()*.005, 50 + lines.size()*.00005, 25 + lines.size()*.01);
//        }
        if(i % 5 == 0){
            ofSetColor(0,  255, 140);
        } else {
            ofSetColor(0 + lines.size()*.00005, 127 + lines.size()*.005, 25 + lines.size()*.01);
        }
        lines[i].draw();
    }
//    ofEndSaveScreenAsSVG();

//    // draw new lines with AxiDraw
//    if (actualTime -  >= 1000) {
//        if (!linesCurrent.empty())
//        {
//            ofPolyline current = linesCurrent.back();
//            // TODO: send instructions to AxiDraw
//            lines.push_back(current);
//    //        current.draw();
//            linesCurrent.pop_back();
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            //plotter.motorsOff();
            break;
        case 'u':
            //plotter.raiseBrush();
            break;
        case 'd':
            //plotter.lowerBrush();
            break;
        case 'n':
            generateNext = true;
            break;
        case 'p':
            //plotter.pause();
            break;
        case 'h':
            //plotter.raiseBrush();
            //plotter.moveToXY(0, 0);
            linesCurrent.clear();
            break;
        case 'x':
            img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
            img.save("screenshot.png");
    }
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

//--------------------------------------------------------------
void ofApp::scanSerial(){
    int portCount = 0;
    //    String portName;
    //    String str1, str2;
    //    int j;
    //
    //    int OpenPortList[];
    //    OpenPortList = new int[0];
    vector <ofSerialDeviceInfo> deviceList;

//    serialOnline = false;
    Boolean serialErr = false;

    try {
        deviceList = serialArduino.getDeviceList();
        portCount = deviceList.size();
    }
    catch (exception e) {
        cout << "ERROR" << endl;
        serialErr = true;
    }

    if (serialErr == false) {
        string count = std::to_string(portCount);
        cout << "\nI found " + count + " serial ports, which are:" << endl;
//        cout << serial.getDeviceList(); << endl;

        //        String  os=System.getProperty("os.name").toLowerCase();
        //        boolean isMacOs = os.startsWith("mac os x");
        //        boolean isWin = os.startsWith("win");

        int baudRate = 9600;
        serialArduino.setup("/dev/cu.SLAB_USBtoUART", baudRate);
        cout << std::to_string(serialArduino.available()) << endl;
//        serial.setup("/dev/cu.usbmodem14301", baudRate);
//        cout << std::to_string(serial.available()) << endl;
        serialOnline = true;
    }
    // Can change to be the name of the port you want, e.g., COM5.
    // The default value is "/dev/cu.usbmodem"; which works on Macs.

    //        str1 = str1.substring(0, 14);
    //
    //        for (int j = 0; j < portCount; j++) {
    //            str2 = Serial.list()[j].substring(0, 14);
    //            if (str1.equals(str2) == true)
    //                OpenPortList =  append(OpenPortList, j);
    //        }
    //        boolean portErr;

    //         j = 0;
    //        while (j < OpenPortList.length) {
    //            portErr = false;
    //            portName = Serial.list()[OpenPortList[j]];
    //
    //            try {
    //                myPort = new Serial(this, portName, 38400);
    //            }
    //            catch (Exception e) {
    //                SerialOnline = false;
    //                portErr = true;
    //                println("Serial port "+portName+" could not be activated.");
    //            }
    //
    //            if (portErr == false) {
    //                myPort.buffer(1);
    //                myPort.clear();
    //                println("Serial port "+portName+" found and activated.");
    //
    //                String inBuffer = "";
    //
    //                myPort.write("v\r");  //Request version number
    //                delay(50);  // Delay for EBB to respond!
    //
    //                while (myPort.available () > 0) {
    //                    inBuffer = myPort.readString();
    //                    if (inBuffer != null) {
    //                        println("Version Number: "+inBuffer);
    //                    }
    //                }
    //
    //                str1 = "EBB";
    //                if (inBuffer.length() > 2) {
    //                    str2 = inBuffer.substring(0, 3);
    //                    if (str1.equals(str2) == true) {
    //                        // EBB Identified!
    //                        SerialOnline = true;    // confirm that this port is good
    //                        j = OpenPortList.length; // break out of loop
    //                        println("Serial port "+portName+" confirmed to have EBB.");
    //                    } else {
    //                        myPort.clear();
    //                        myPort.stop();
    //                        println("Serial port "+portName+": No EBB detected.");
    //                    }
    //                }
    //            }
    //            j++;
    //        }
    //    }
}

void ofApp::readSensors(){
    // TODO: read and store sensor values
    if (serialArduino.available() > 0) {
        // read the incoming bytes:
        try {
            arduinoData = ofxGetSerialString(serialArduino, '\n');
            if (!arduinoData.empty()) {
                int loc = arduinoData.find(",");
                if(loc > 0){

                    int vertical = std::stoi(arduinoData.substr(0, loc));
                    int horizontal = std::stoi(arduinoData.substr(loc + 1, arduinoData.length() - 1));
                    if (vertical < 10000){
                        sensorVertical.push_front(vertical);
                        if(sensorVertical.size() >= 5) {
                            sensorVertical.pop_back();
                        }
                    }
                    if (horizontal < 10000){
                        sensorHorizontal.push_front(horizontal);
                        if(sensorHorizontal.size() >= 5) {
                            sensorHorizontal.pop_back();
                        }
                    }
//                cout << "Horizontal: " + std::to_string(sensorHorizontal.front()) << endl;
//                cout << "Vertical: " + std::to_string(sensorVertical.front()) << endl;
                }
            }
        } catch(exception& e){
            cout << "Error reading sensor" << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::setupTriangles(){
    float x1, x2, x3 = 0;
    float y1, y2, y3 = 0;

    float width = ofGetWidth();
    float height= ofGetHeight();

    float adjustment = ofRandom(350, 350);
    int side = ofRandom(1, 5);

    // set up iterator to add triangles
    top = tr.begin();

    // TODO: control adjustment with sensors
    side = 1;
    switch (side)
    {
        case 1: // top
            x1 = 10;
            y1 = height - 10;

            x2 = width/2 + ofRandom(-width/4, width/4);
            y2 = 10;

            x3 = width-10;
            y3 = height-10;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3), true });
            tr.insert(top, { ofVec2f(10, 10), ofVec2f(x2, y2), ofVec2f(x1, y1), true });
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(width-10, 10), true });
            break;
        case 2: // left
            x1 = width;
            y1 = 0;

            x2 = 0;
            y2 = height/2 + ofRandom(-height/4, height/4);

            x3 = width;
            y3 = height;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3), true });
            tr.insert(top, { ofVec2f(0, 0), ofVec2f(x2, y2), ofVec2f(x1, y1), true });
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(0, height), true });
            break;
        case 3: // bottom
            x1 = width;
            y1 = 0;

            x2 = width/2 + ofRandom(-width/4, width/4);
            y2 = height;

            x3 = 0;
            y3 = 0;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3), true });
            tr.insert(top, { ofVec2f(0, 0), ofVec2f(x2, y2), ofVec2f(0, height), true });
            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(width, height), true });
            break;
        default: // right
            x1 = 0;
            y1 = 0;

            x2 = width;
            y2 = height/2 + ofRandom(-height/4, height/4);

            x3 = 0;
            y3 = height;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3), true });
            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(width, 0), true });
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(width, height), true });
            break;
    }

    ofPolyline divider;
    divider.addVertex(x1, y1);
    divider.addVertex(x2, y2);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(x2, y2);
    divider.addVertex(x3, y3);
//    lines.push_back(divider);
    linesCurrent.push_back(divider);
}

vector<Triangle> ofApp::divideTriangleByTwo(tree<Triangle>::iterator pos) {
    vector<Triangle> newTriangles;

    ofVec2f newPointA;
    ofVec2f newPointB;
    ofVec2f newPointC;

    int sideAB = (*pos).pointA.distance((*pos).pointB);
    int sideBC = (*pos).pointB.distance((*pos).pointC);
    int sideCA = (*pos).pointC.distance((*pos).pointA);

    if (sideAB > sideBC && sideAB > sideCA) {
        newPointA = (*pos).pointC;
        newPointB = (*pos).pointA;
        newPointC = (*pos).pointB;
    } else if (sideBC > sideCA && sideBC > sideAB){
        newPointA = (*pos).pointA;
        newPointB = (*pos).pointB;
        newPointC = (*pos).pointC;
    } else {
        newPointA = (*pos).pointB;
        newPointB = (*pos).pointC;
        newPointC = (*pos).pointA;
    }

    float newPointX = (newPointB.x + newPointC.x)/2;
    float newPointY = (newPointB.y + newPointC.y)/2;

    newTriangles.push_back({ newPointA, ofVec2f(newPointX, newPointY), newPointC, true });
    newTriangles.push_back({ newPointA, ofVec2f(newPointX, newPointY), newPointB, true });

    ofPolyline divider;
    divider.addVertex(newPointA.x, newPointA.y);
    divider.addVertex(newPointX, newPointY);
//    lines.push_back(divider);
    linesCurrent.push_back(divider);

    return newTriangles;
}

vector<Triangle> ofApp::divideTriangleByThree(tree<Triangle>::iterator pos) {
    vector<Triangle> newTriangles;

    ofVec2f newPointA = (*pos).pointA;
    ofVec2f newPointB = (*pos).pointB;
    ofVec2f newPointC = (*pos).pointC;

    float newPointX = (newPointA.x + newPointB.x + newPointC.x)/3;
    float newPointY = (newPointA.y + newPointB.y + newPointC.y)/3;

    newTriangles.push_back({ ofVec2f(newPointX, newPointY), newPointA, newPointB, true });
    newTriangles.push_back({ ofVec2f(newPointX, newPointY), newPointA, newPointC, true });
    newTriangles.push_back({ ofVec2f(newPointX, newPointY), newPointB, newPointC, true });

    // First new line (3 points including center)
    ofPolyline divider;
    divider.addVertex(newPointA.x, newPointA.y);
    divider.addVertex(newPointX, newPointY);
    divider.addVertex(newPointB.x, newPointB.y);

    lines.push_back(divider);
    linesCurrent.push_back(divider);

    // Second new line (2 points including center)
    divider.addVertex(newPointX, newPointY);
    divider.addVertex(newPointC.x, newPointC.y);

    lines.push_back(divider);
    linesCurrent.push_back(divider);

    return newTriangles;
}

vector<Triangle> ofApp::divideTriangleByFour(tree<Triangle>::iterator pos) {
    vector<Triangle> newTriangles;
    ofVec2f newPointA = (*pos).pointA;
    ofVec2f newPointB = (*pos).pointB;
    ofVec2f newPointC = (*pos).pointC;

    float midpointABX = (newPointA.x + newPointB.x)/2;
    float midpointABY = (newPointA.y + newPointB.y)/2;

    float midpointBCX = (newPointB.x + newPointC.x)/2;
    float midpointBCY = (newPointB.y + newPointC.y)/2;

    float midpointCAX = (newPointC.x + newPointA.x)/2;
    float midpointCAY = (newPointC.y + newPointA.y)/2;

    newTriangles.push_back({ ofVec2f(midpointABX, midpointABY), ofVec2f(midpointBCX, midpointBCY), newPointB, true });
    newTriangles.push_back({ ofVec2f(midpointBCX, midpointBCY), ofVec2f(midpointCAX, midpointCAY), newPointC, true });
    newTriangles.push_back({ ofVec2f(midpointCAX, midpointCAY), ofVec2f(midpointABX, midpointABY), newPointA, true });

    newTriangles.push_back({ ofVec2f(midpointABX, midpointABY), ofVec2f(midpointBCX, midpointBCY), ofVec2f(midpointCAX, midpointCAY), true });

    ofPolyline divider;
    divider.addVertex(midpointABX, midpointABY);
    divider.addVertex(midpointBCX, midpointBCY);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(midpointBCX, midpointBCY);
    divider.addVertex(midpointBCX, midpointBCY);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(midpointBCX, midpointBCY);
    divider.addVertex(midpointCAX, midpointCAY);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(midpointCAX, midpointCAY);
    divider.addVertex(midpointABX, midpointABY);
//    lines.push_back(divider);
    linesCurrent.push_back(divider);

    return newTriangles;
}

vector<Triangle> ofApp::nestTriangle(tree<Triangle>::iterator pos) {
    vector<Triangle> newTriangles;

    ofVec2f newPointA = (*pos).pointA;
    ofVec2f newPointB = (*pos).pointB;
    ofVec2f newPointC = (*pos).pointC;

    float newPointX = (newPointA.x + newPointB.x + newPointC.x)/3;
    float newPointY = (newPointA.y + newPointB.y + newPointC.y)/3;

    ofPolyline segment1;
    ofPolyline segment2;
    ofPolyline segment3;
    segment1.addVertex(newPointA.x, newPointA.y);
    segment1.addVertex(newPointX, newPointY);

    segment2.addVertex(newPointB.x, newPointB.y);
    segment2.addVertex(newPointX, newPointY);

    segment3.addVertex(newPointC.x, newPointC.y);
    segment3.addVertex(newPointX, newPointY);

    newPointA = segment1.getPointAtPercent(.04f);
    newPointB = segment2.getPointAtPercent(.04f);
    newPointC = segment3.getPointAtPercent(.04f);

    newTriangles.push_back({ newPointA, newPointB, newPointC, true });

    ofPolyline divider;
    divider.addVertex(newPointA.x, newPointA.y);
    divider.addVertex(newPointB.x, newPointB.y);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(newPointB.x, newPointB.y);
    divider.addVertex(newPointC.x, newPointC.y);
    linesCurrent.push_back(divider);
    divider.clear();
    divider.addVertex(newPointC.x, newPointC.y);
    divider.addVertex(newPointA.x, newPointA.y);
    linesCurrent.push_back(divider);

//    lines.push_back(divider);
//    linesCurrent.push_back(divider);
    return newTriangles;
}


string ofApp::ofxGetSerialString(ofSerial &serialArduino, char until) {
    static string str;
    stringstream ss;
    char ch;
    int ttl = 1000;
    while ((ch = serialArduino.readByte())>0 && (ttl--) > 0 && ch != until) {
        ss << ch;
    }
    str += ss.str();
    if (ch == until) {
        string tmp = str;
        str = "";
        return ofxTrimString(tmp);
    }
    else {
        return "";
    }
}

// trim right trailing spaces
string ofApp::ofxTrimStringRight(string str) {
    size_t endpos = str.find_last_not_of(" \t\r\n");
    return (string::npos != endpos) ? str.substr(0, endpos + 1) : str;
}

// trim left trailing spaces
string ofApp::ofxTrimStringLeft(string str) {
    size_t startpos = str.find_first_not_of(" \t\r\n");
    return (string::npos != startpos) ? str.substr(startpos) : str;
}

// trim trailing spaces
string ofApp::ofxTrimString(string str) {
    return ofxTrimStringLeft(ofxTrimStringRight(str));;
}
