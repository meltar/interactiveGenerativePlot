#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    delayTimer = ofGetElapsedTimeMillis();

    scanSerial();
    setupPlotter();

    ofBackground(0);
    ofColor(255);
//    ofSeedRandom(0);

    ofPolyline outline;
    outline.addVertex(0,0);
    outline.addVertex(0, ofGetHeight());
    outline.addVertex(ofGetWidth(), ofGetHeight());
    outline.addVertex(ofGetWidth(), 0);
    outline.addVertex(0,0);
    lines.push_back(outline);
    linesCurrent.push_back(outline);
    setupTriangles();
}

//--------------------------------------------------------------
void ofApp::update(){
    readSensors();

    // wait between generation calls
    actualTime = ofGetElapsedTimeMillis();

    // TODO: if there is nothing left to draw, generate next division
    // Set timer to 2 sec
    if (actualTime - delayTimer >= 2000) {
        delayTimer = ofGetElapsedTimeMillis();
//        cout << std::to_string(tr.size()) << endl;
//        cout << "2 sec" << endl;
        // Iterate through triangle tree leaf nodes
        tree<Triangle>::leaf_iterator iter=tr.begin_leaf();
        while(iter!=tr.end()) {
//                old = iter;
//                tr.append_child(old, { ofVec2f(444, 0), ofVec2f(444, 444), ofVec2f(0, 444)});
            cout << (*iter).pointA << endl;
//                iter.skip_children();
            ++iter;
        }
        cout << std::to_string(tr.size()) << endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw all lines on the screen
    for(int i = 0; i < lines.size(); i++){
        lines[i].draw();
    }

    // draw new lines with AxiDraw
    while (!linesCurrent.empty())
    {
        ofPolyline current = linesCurrent.back();
        // TODO: send instructions to AxiDraw
        linesCurrent.pop_back();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            motorsOff();
            break;
        case 'u':
            raiseBrush();
            break;
        case 'd':
            lowerBrush();
            break;
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
void ofApp::raiseBrush(){
    //    int waitTime = NextMoveTime - millis();
    //    if (waitTime > 0)
    //    {
    //        raiseBrushStatus = 1; // Flag to raise brush when no longer busy.
    //    } else
    //    {
    //        if (BrushDown == true) {
    //            if (SerialOnline) {
    //                myPort.write("SP,0," + str(delayAfterRaisingBrush) + "\r");
    //                BrushDown = false;
    //                NextMoveTime = millis() + delayAfterRaisingBrush;
    //            }
    //            //      if (debugMode) println("Raise Brush.");
    //        }
    //        raiseBrushStatus = -1; // Clear flag.
    //    }
    if (brushDown == true) {
        if (serialOnline) {
            string message = "SP,0," + std::to_string(10) + "\r";
            int length = message.length();
            char char_array[length + 1];
            strcpy(char_array, message.c_str());

            serial.writeBytes(char_array, 9);
            brushDown = false;
            cout << "Raise Brush." << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::lowerBrush(){
    //    int waitTime = NextMoveTime - millis();
    //    if (waitTime > 0)
    //    {
    //        lowerBrushStatus = 1;  // Flag to lower brush when no longer busy.
    //        // delay (waitTime);  // Wait for prior move to finish:
    //    } else
    //    {
    //        if  (BrushDown == false)
    //        {
    //            if (SerialOnline) {
    //                myPort.write("SP,1," + str(delayAfterLoweringBrush) + "\r");
    //
    //                BrushDown = true;
    //                NextMoveTime = millis() + delayAfterLoweringBrush;
    //                //lastPosition = new PVector(-1,-1);
    //            }
    //            //      if (debugMode) println("Lower Brush.");
    //        }
    //        lowerBrushStatus = -1; // Clear flag.
    //    }
    if (brushDown == false) {
        if (serialOnline) {
            string message = "SP,1," + std::to_string(10) + "\r";
            int length = message.length();
            char char_array[length + 1];
            strcpy(char_array, message.c_str());

            serial.writeBytes(char_array, length + 1);
            brushDown = true;
            cout << "Lower Brush." << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::motorsOff(){
    if (serialOnline) {
        string message = "EM,0,0\r";
        int length = message.length();
        char char_array[length + 1];
        strcpy(char_array, message.c_str());

        serial.writeBytes(char_array, length + 1);
        cout << "Motors off." << endl;
    }
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

    serialOnline = false;
    Boolean serialErr = false;

    try {
        deviceList = serial.getDeviceList();
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
        serial.setup("/dev/cu.usbmodem14301", baudRate);
        cout << std::to_string(serial.available()) << endl;
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
    }
}

//--------------------------------------------------------------
void ofApp::setupPlotter() {
    if (serialOnline) {
        cout << "CONNECTED" << endl;
        string message = "EM,1,1\r";
        int length = message.length();
        char char_array[length + 1];
        strcpy(char_array, message.c_str());
        //Configure both steppers in 1/16 step mode
        serial.writeBytes(char_array, length + 1);

        // Configure brush lift servo endpoints and speed
        float servoPaint = 7500 + 175 * 30;
        message = "SC,4," + std::to_string(servoPaint) + "\r";
        length = message.length();
        char_array[length + 1];
        strcpy(char_array, message.c_str());
        // Brush DOWN position, for painting
        serial.writeBytes(char_array, length + 1);

        float servoUp = 7500 + 175 * 70;
        message = "SC,5," + std::to_string(servoUp) + "\r";
        length = message.length();
        char_array[length + 1];
        strcpy(char_array, message.c_str());
        // Brush UP position
        serial.writeBytes(char_array, length + 1);

        message = "SC,10,65535\r";
        length = message.length();
        char_array[length + 1];
        strcpy(char_array, message.c_str());
        // Set brush raising and lowering speed.
        serial.writeBytes(char_array, length + 1);

        // Ensure that we actually raise the brush:
        brushDown = true;
        raiseBrush();
        sleep(2);
        lowerBrush();
        sleep(2);
        motorsOff();
        sleep(2);

        cout << "SUCCESS" << endl;
    } else {
        cout << "ERROR DURING SETUP" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::setupTriangles(){
    float x1, x2, x3 = 0;
    float y1, y2, y3 = 0;

    float width = ofGetWidth();
    float height= ofGetHeight();

    float adjustment = ofRandom(-450, 450);
    int side = ofRandom(1, 4);

    // set up iterator to add triangles
    top = tr.begin();

    // TODO: control adjustment with sensors
    adjustment = 0;
    switch (side)
    {
        case 1: // top
            x1 = 0;
            y1 = height;

            x2 = width/2 + adjustment;
            y2 = 0;

            x3 = width;
            y3 = height;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3)});
            tr.insert(top, { ofVec2f(0, 0), ofVec2f(x2, y2), ofVec2f(x1, y1)});
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(width, 0)});
            break;
        case 2: // left
            x1 = width;
            y1 = 0;

            x2 = 0;
            y2 = height/2 + adjustment;

            x3 = width;
            y3 = height;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3)});
            tr.insert(top, { ofVec2f(0, 0), ofVec2f(x2, y2), ofVec2f(x1, y1)});
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(0, height)});
            break;
        case 3: // bottom
            x1 = width;
            y1 = 0;

            x2 = width/2 + adjustment;
            y2 = height;

            x3 = 0;
            y3 = 0;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3)});
            tr.insert(top, { ofVec2f(0, 0), ofVec2f(x2, y2), ofVec2f(0, height)});
            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(width, height)});
            break;
        default: // right
            x1 = 0;
            y1 = 0;

            x2 = width;
            y2 = height/2 + adjustment;

            x3 = 0;
            y3 = height;

            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(x3, y3)});
            tr.insert(top, { ofVec2f(x1, y1), ofVec2f(x2, y2), ofVec2f(width, 0)});
            tr.insert(top, { ofVec2f(x3, y3), ofVec2f(x2, y2), ofVec2f(width, height)});
            break;
    }

    ofPolyline divider;
    divider.addVertex(x1, y1);
    divider.addVertex(x2, y2);
    divider.addVertex(x3, y3);
    lines.push_back(divider);
    linesCurrent.push_back(divider);
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
