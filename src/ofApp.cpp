#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    scanSerial();
    setupPlotter();

    // TODO:
    // setupArduino

    ofBackground(0);
    ofColor(255);
//    ofSeedRandom(0);

    ofPolyline outline;
    outline.addVertex(0,0);
    outline.addVertex(0, ofGetHeight());
    outline.addVertex(ofGetWidth(), ofGetHeight());
    outline.addVertex(ofGetWidth(), 0);
    lines.push_back(outline);
    setupTriangles();
}

//--------------------------------------------------------------
void ofApp::update(){
    // TODO: read and store sensor values

    // TODO: if there is nothing left to draw, generate next division
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
        //        println(Serial.list());

        //        String  os=System.getProperty("os.name").toLowerCase();
        //        boolean isMacOs = os.startsWith("mac os x");
        //        boolean isWin = os.startsWith("win");

        serial.setup("/dev/cu.usbmodem14201", 9600);
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

    float adjustment = ofRandom(-450, 450);
    int side = ofRandom(1, 4);

    // TODO: control adjustment with sensors
//    adjustment = 0;
    switch (side)
    {
        case 1: // top
            x1 = 0;
            y1 = ofGetHeight();

            x2 = ofGetWidth()/2 + adjustment;
            y2 = 0;

            x3 = ofGetWidth();
            y3 = ofGetHeight();
            break;
        case 2: // left
            x1 = ofGetWidth();
            y1 = 0;

            x2 = 0;
            y2 = ofGetHeight()/2 + adjustment;

            x3 = ofGetWidth();
            y3 = ofGetHeight();
            break;
        case 3: // bottom
            x1 = ofGetWidth();
            y1 = 0;

            x2 = ofGetWidth()/2 + adjustment;
            y2 = ofGetHeight();

            x3 = 0;
            y3 = 0;
            break;
        default: // right
            x1 = 0;
            y1 = 0;

            x2 = ofGetWidth();
            y2 = ofGetHeight()/2 + adjustment;

            x3 = 0;
            y3 = ofGetHeight();
            break;
    }

    ofPolyline divider;
    divider.addVertex(x1, y1);
    divider.addVertex(x2, y2);
    divider.addVertex(x3, y3);
    lines.push_back(divider);
    linesCurrent.push_back(divider);

    // TODO:
    // add all new triangles to collection
}
