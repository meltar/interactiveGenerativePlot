//
//  plotter.cpp
//  interactiveGenerativePlot
//
//  Created by Melissa Holmes on 5/7/19.
//

#include "plotter.h"

Plotter::Plotter() {
    serialOnline = false;
    // User Settings:
    motorSpeed = 4000.0;  // Steps per second, 1500 default

    servoUpPct = 70;    // Brush UP position, %  (higher number lifts higher).
    servoPaintPct = 30;    // Brush DOWN position, %  (higher number lifts higher).

    reverseMotorX = false;
    reverseMotorY = false;

    delayAfterRaisingBrush = 300; //ms
    delayAfterLoweringBrush = 300; //ms

    debugMode = true;
    //boolean debugMode = false;

    paperSizeA4 = false; // true for A4. false for US letter.

//    boolean segmentQueued = false;
//    PVector queuePt1 = new PVector(-1, -1);
//    PVector queuePt2 = new PVector(-1, -1);

    motorStepsPerPixel = 32.1;// Good for 1/16 steps-- standard behavior.
    pixelsPerInch = 63.3;

    // Hardware resolution: 1016 steps per inch @ 50% max resolution
    // Horizontal extent in this window frame is 740 px.
    // 2032 steps per inch * (11.69 inches (i.e., A4 length)) per 740 px gives 16.05 motor steps per pixel.
    // Vertical travel for 8.5 inches should be  (8.5 inches * 2032 steps/inch) / (32.1 steps/px) = 538 px.
    // PixelsPerInch is given by (2032 steps/inch) / (32.1 steps/px) = 63.3 pixels per inch

    yBrushRestPositionPixels = 6;

    doSerialConnect = true;
    drawingPath = false;
//    lastButtonUpdateX = 0;
//    lastButtonUpdateY = 0;
    subsequentWaitTime = -1;    //How long the following movement will take.

    lastPosition = ofVec2f(-1, -1);

    servoUp = 7500 + 175 * servoUpPct;    // Brush UP position, native units
    servoPaint = 7500 + 175 * servoPaintPct;   // Brush DOWN position, native units.

    nextMoveTime = ofGetElapsedTimeMillis();

    motorX = 0;
    motorY = 0;
    moveDestX = -1;
    moveDestY = -1;

    motorMinX = 0;
    motorMinY = 0;

    // Used for calculating movement for motors
    mousePaperLeft =  0;
    mousePaperRight =  740;
    mousePaperTop =  0;
    mousePaperBottom =  538;

    motorMaxX = int(floor(float(mousePaperRight - mousePaperLeft) * motorStepsPerPixel)) ;
    motorMaxY = int(floor(float(mousePaperBottom - mousePaperTop) * motorStepsPerPixel)) ;

    mousePaperRight = round(pixelsPerInch * 11.0);
    mousePaperBottom = round(pixelsPerInch * 8.5 );
//    if (paperSizeA4) {
//        mousePaperRight = round(mousePaperLeft + pixelsPerInch * 297/25.4);
//        mousePaperBottom = round(mousePaperTop + pixelsPerInch * 210/25.4);
//    } else {
//        mousePaperRight = round(mousePaperLeft + pixelsPerInch * 11.0);
//        mousePaperBottom = round(mousePaperTop + pixelsPerInch * 8.5);
//    }

//#ifdef _CONNECT_TO_AXIDRAW_
    connect();
    setup();
//#endif
}

Plotter::~Plotter(){
}

void Plotter::connect() {
    int portCount = 0;
    //    String portName;
    //    String str1, str2;
    //    int j;
    //
    //    int OpenPortList[];
    //    OpenPortList = new int[0];
    vector<ofSerialDeviceInfo> deviceList;

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
//        string count = std::to_string(portCount);
//        cout << "\nI found " + count + " serial ports, which are:" << endl;
        //        cout << serial.getDeviceList(); << endl;

        //        String  os=System.getProperty("os.name").toLowerCase();
        //        boolean isMacOs = os.startsWith("mac os x");
        //        boolean isWin = os.startsWith("win");

        int baudRate = 9600;
//        serialArduino.setup("/dev/cu.SLAB_USBtoUART", baudRate);
//        cout << std::to_string(serialArduino.available()) << endl;
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

void Plotter::setup() {
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
//        brushDown = false;
//        lowerBrush();
//        sleep(2);
//        raiseBrush();
//        sleep(2);
//
//
//
//        moveToXY2(740, 538);
//        moveToXY2(0, 0);
//        moveToXY2(0, 0);


        motorsOff();
        if (debugMode) {
            cout << "SUCCESS" << endl;
        }
    } else {
        if (debugMode) {
            cout << "ERROR DURING SETUP" << endl;
        }
    }
}

//--------------------------------------------------------------
void Plotter::raiseBrush() {
        int waitTime = nextMoveTime - ofGetElapsedTimeMillis();
        if (waitTime > 0) {
            raiseBrushStatus = 1; // Flag to raise brush when no longer busy.
        } else {
            if (brushDown == true) {
                if (serialOnline) {
                    string message = "SP,0," + std::to_string(10) + "\r";
                    int length = message.length();
                    char char_array[length + 1];
                    strcpy(char_array, message.c_str());

                    serial.writeBytes(char_array, 9);
                    brushDown = false;
                    nextMoveTime = ofGetElapsedTimeMillis() + delayAfterRaisingBrush;

                    if (debugMode) {
                        cout << "Raise Brush." << endl;
                    }
                }
            }
            raiseBrushStatus = -1; // Clear flag.
        }
}

//--------------------------------------------------------------
void Plotter::lowerBrush() {
    int waitTime = nextMoveTime - ofGetElapsedTimeMillis();
    if (waitTime > 0) {
        lowerBrushStatus = 1;  // Flag to lower brush when no longer busy.
    } else {
        if (brushDown == false) {
            if (serialOnline) {
                string message = "SP,1," + std::to_string(10) + "\r";
                int length = message.length();
                char char_array[length + 1];
                strcpy(char_array, message.c_str());

                serial.writeBytes(char_array, length + 1);
                brushDown = true;
                if (debugMode) {
                    cout << "Lower Brush." << endl;
                }
            }
        }
        lowerBrushStatus = -1; // Clear flag.
    }
}

//--------------------------------------------------------------
void Plotter::motorsOff() {
    if (serialOnline) {
        string message = "EM,0,0\r";
        int length = message.length();
        char char_array[length + 1];
        strcpy(char_array, message.c_str());

        serial.writeBytes(char_array, length + 1);
        if (debugMode) {
            cout << "Motors off." << endl;
        }
    }
}

void Plotter::moveRelativeXY(int xD, int yD) {
    // Change carriage position by (xDelta, yDelta), with XY limit checking, time management, etc.
    int xTemp = motorX + xD;
    int yTemp = motorY + yD;

    moveToXY(xTemp, yTemp);
}

void Plotter::moveToXY2(int xLoc, int yLoc) {
    do {
        moveToXY(xLoc * motorStepsPerPixel, yLoc * motorStepsPerPixel);
        ofSleepMillis(50);
    } while (moveStatus >= 1);
}

void Plotter::moveToXY(int xLoc, int yLoc) {
    moveDestX = xLoc;
    moveDestY = yLoc;

    moveToXY();
}

void Plotter::moveToXY() {
    int traveltime_ms;
    // Absolute move in motor coordinates, with XY limit checking, time management, etc.
    // Use MoveToXY(int xLoc, int yLoc) to set destinations.

    int waitTime = nextMoveTime - ofGetElapsedTimeMillis();
    if (waitTime > 0)
    {
        moveStatus = 1;  // Flag this move as not yet completed.
    } else
    {
        if ((moveDestX < 0) || (moveDestY < 0))
        {
            // Destination has not been set up correctly.
            // Re-initialize varaibles and prepare for next move.
            moveDestX = -1;
            moveDestY = -1;
        } else {
            moveStatus = -1;
            if (moveDestX > motorMaxX)
                moveDestX = motorMaxX;
            else if (moveDestX < motorMinX)
                moveDestX = motorMinX;

            if (moveDestY > motorMaxY)
                moveDestY = motorMaxY;
            else if (moveDestY < motorMinY)
                moveDestY = motorMinY;

            int xD = moveDestX - motorX;
            int yD = moveDestY - motorY;

            if (debugMode) {
                cout << "moveDestX: " + std::to_string(moveDestX) + ", motorX: " + std::to_string(motorX) + ",  xD: " + std::to_string(xD) + ", motorMaX: " + std::to_string(motorMaxX) << endl;
                cout << "moveDestY: " + std::to_string(moveDestY) + ", motorY: " + std::to_string(motorY) + ",  yD: " + std::to_string(yD) + ", motorMaY: " + std::to_string(motorMaxY) << endl;
            }
            if ((xD != 0) || (yD != 0)){

                motorX = moveDestX;
                motorY = moveDestY;

                int maxTravel = max(abs(xD), abs(yD));
                traveltime_ms = int(floor( float(1000 * maxTravel)/motorSpeed));

                // TODO check this later
                nextMoveTime = ofGetElapsedTimeMillis() + traveltime_ms - ceil(1000 / 60);
                // Important correction-- Start next segment sooner than you might expect,
                // because of the relatively low framerate that the program runs at.

                if (serialOnline) {
                    if (reverseMotorX)
                        xD *= -1;
                    if (reverseMotorY)
                        yD *= -1;

                    //General command "XM,duration,axisA,axisB<CR>"
                    string message = "XM," + std::to_string(traveltime_ms) + "," + std::to_string(xD) + "," + std::to_string(yD) + "\r";
                    if (debugMode) {
                        cout << "Motor X: " + std::to_string(motorX) + "  Motor Y: " + std::to_string(motorY) << " traveltime_ms: " << std::to_string(traveltime_ms) << endl;
                    }
                    int length = message.length();
                    char char_array[length + 1];
                    strcpy(char_array, message.c_str());
                    serial.writeBytes(char_array, length + 1);
                }

                // Calculate and animate position location cursor
                // TODO check this - it was related to animated the cursor and using ani lib
//                int pos = getMotorPixelPos();
//                float sec = traveltime_ms/1000.0;

                if (debugMode) {
                    cout << "Motor X: " + std::to_string(motorX) + "  Motor Y: " + std::to_string(motorY) << endl;
                }
            }
        }
    }
}

// Return the [x,y] of the motor position in pixels
ofVec2f Plotter::getMotorPixelPos() {
    return ofVec2f((int)(float (motorX) / motorStepsPerPixel) + mousePaperLeft,
                   (int)(float (motorY) / motorStepsPerPixel) + mousePaperTop + yBrushRestPositionPixels);
}

// Get float distance between two non-encoded (x,y) positions.
float Plotter::getDistance(int x1, int y1, int x2, int y2) {
    int xdiff = abs(x2 - x1);
    int ydiff = abs(y2 - y1);
    return sqrt(pow(xdiff, 2) + pow(ydiff, 2));
}

void Plotter::zero() {
    // Mark current location as (0,0) in motor coordinates.
    // Manually move the motor carriage to the left-rear (upper left) corner before executing this command.

    motorX = 0;
    motorY = 0;

    moveStatus = -1;
    moveDestX = -1;
    moveDestY = -1;

    // Calculate and animate position location cursor
    ofVec2f pos = getMotorPixelPos();
    float sec = .25;

    if (debugMode) {
        cout << "Motor X: " + std::to_string(motorX) + "  Motor Y: " + std::to_string(motorY) << endl;
    }
}

// TODO: Rewrite this
// I think all these todo items are points
void Plotter::checkServiceBrush(ofPolyline currentLine) {
    // 1) move to first x,y position in line
    // 2) brush down
    // 3) move to next x, y position
    // 4) repeat step 3 for all following points
    // 5) brush up

    // Handle pausing
    if (serviceBrush() == false) {
        if (ofGetElapsedTimeMillis() > nextMoveTime) {
//            vector<ofVec3f> points = currentLine.getVertices();

            for (auto & v : currentLine.getVertices()) {
                v.x += ofRandom(-0.5, 0.5);
                v.y += ofRandom(-0.5, 0.5);
            }

            int intTemp = -1;
            float inputTemp = -1.0;
//
            if ((currentLine.size() > (indexDone + 1))   && (!paused)) {
//                toDoItem = ToDoList[1 + indexDone];
//                inputTemp = toDoItem.x;
                indexDone++;
            }

//
//                if (inputTemp >= 0) { // Move the carriage to draw a path segment!
//                    toDoItem = ToDoList[indexDone];
//                    float x2 = toDoItem.x;
//                    float y2 = toDoItem.y;
//
//                    int x1 = round( (x2 - float(mousePaperLeft)) * motorStepsPerPixel);
//                    int y1 = round( (y2 - float(mousePaperTop)) * motorStepsPerPixel);
//
//                    moveToXY(x1, y1);
//                    //println("Moving to: " + str(x2) + ", " + str(y2));
//
//                    if (lastPosition.x == -1) {
//                        lastPosition = toDoItem;
//                        //println("Starting point: Init.");
//                    }
//
//                    lastPosition = toDoItem;
            }
        }
}


Boolean Plotter::serviceBrush() {
    // Manage processes of getting paint, water, and cleaning the brush,
    // as well as general lifts and moves.  Ensure that we allow time for the
    // brush to move, and wait respectfully, without local wait loops, to
    // ensure good performance for the artist.
    // Returns true if servicing is still taking place, and false if idle.

    Boolean serviceStatus = false;

    int waitTime = nextMoveTime - ofGetElapsedTimeMillis();
    if (waitTime >= 0) {
        serviceStatus = true;
        // We still need to wait for *something* to finish!
    } else {
        if (raiseBrushStatus >= 0) {
            raiseBrush();
            serviceStatus = true;
        } else if (lowerBrushStatus >= 0) {
            lowerBrush();
            serviceStatus = true;
        } else if (moveStatus >= 0) {
            moveToXY(); // Perform next move, if one is pending.
            serviceStatus = true;
        }
    }
    return serviceStatus;
}


// TODO: Delete this?
// This is all on screen drawing??
// This allows redrawing on screen.

//void Plotter::drawToDoList() {
//    // Erase all painting on main image background, and draw the existing "ToDo" list
//    // on the off-screen buffer.
//
//    int j = ToDoList.length;
//    float x1, x2, y1, y2;
//
//    float brightness;
//    color white = color(255, 255, 255);
//
//    if ((indexDrawn + 1) < j)
//    {
//
//        // Ready the offscreen buffer for drawing onto
//        offScreen.beginDraw();
//
//        if (indexDrawn < 0) {
//            offScreen.image(imgBackground, 0, 0, 800, 631);  // Copy original background image into place!
//
//            offScreen.noFill();
//            offScreen.strokeWeight(0.5);
//
//            if (PaperSizeA4)
//            {
//                offScreen.stroke(128, 128, 255);  // Light Blue: A4
//                float rectW = PixelsPerInch * 297/25.4;
//                float rectH = PixelsPerInch * 210/25.4;
//                offScreen.rect(float(MousePaperLeft), float(MousePaperTop), rectW, rectH);
//            } else
//            {
//                offScreen.stroke(255, 128, 128); // Light Red: US Letter
//                float rectW = PixelsPerInch * 11.0;
//                float rectH = PixelsPerInch * 8.5;
//                offScreen.rect(float(MousePaperLeft), float(MousePaperTop), rectW, rectH);
//            }
//        } else
//            offScreen.image(imgMain, 0, 0);
//
//        offScreen.strokeWeight(1);
//        //offScreen.stroke(PenColor);
//
//        brightness = 0;
//        color DoneColor = lerpColor(PenColor, white, brightness);
//
//        brightness = 0.8;
//        color ToDoColor = lerpColor(PenColor, white, brightness);
//        x1 = 0;
//        y1 = 0;
//
//        boolean virtualPenDown = false;
//
//        int index = 0;
//        if (index < 0)
//            index = 0;
//        while ( index < j)
//        {
//            PVector toDoItem = ToDoList[index];
//
//            x2 = toDoItem.x;
//            y2 = toDoItem.y;
//
//            if (x2 >= 0) {
//                if (virtualPenDown)
//                {
//                    if (index < indexDone)
//                        offScreen.stroke(DoneColor);
//                    else
//                        offScreen.stroke(ToDoColor);
//
//                    offScreen.line(x1, y1, x2, y2); // Preview lines that are not yet on paper
//
//                    //println("Draw line: "+str(x1)+", "+str(y1)+", "+str(x2) + ", "+str(y2));
//
//                    x1 = x2;
//                    y1 = y2;
//                } else {
//                    //println("Pen up move");
//                    x1 = x2;
//                    y1 = y2;
//                }
//            } else {
//                int x3 = -1 * round(x2);
//                if (x3 == 30)
//                {
//                    virtualPenDown = false;
//                    //println("pen up");
//                } else if (x3 == 31)
//                {
//                    virtualPenDown = true;
//                    //println("pen down");
//                } else if (x3 == 35)
//                {// Home;  MoveToXY(0, 0); Do not draw home moves.
//                    //if (virtualPenDown)
//                    //offScreen.line(x1, y1, 0, 0); // Preview lines that are not yet on paper
//                    x1 = 0;
//                    y1 = 0;
//                }
//            }
//            index++;
//        }
//
//        offScreen.endDraw();
//
//        imgMain = offScreen.get(0, 0, offScreen.width, offScreen.height);
//    }
//}


void Plotter::pause() {
    if (debugMode) {
        cout << "Pause detected." << endl;
    }
    if (paused) {
        if (debugMode) {
            cout << "Resuming AxiDraw." << endl;
        }

        paused = false;
        if (brushDownAtPause) {
            int waitTime = nextMoveTime - ofGetElapsedTimeMillis();
            if (waitTime > 0)
            {
                sleep(waitTime);  // Wait for prior move to finish:
            }

            if (brushDown) {
                raiseBrush();
            }

            waitTime = nextMoveTime - ofGetElapsedTimeMillis();
            if (waitTime > 0)
            {
                sleep(waitTime);  // Wait for prior move to finish:
            }

            moveToXY(xLocAtPause, yLocAtPause);

            waitTime = nextMoveTime - ofGetElapsedTimeMillis();
            if (waitTime > 0)
            {
                sleep(waitTime);  // Wait for prior move to finish:
            }

            lowerBrush();
        }
    } else {
        if (debugMode) {
            cout << "Pausing AxiDraw." << endl;
        }
        paused = true;

        if (brushDown) {
            brushDownAtPause = true;
            raiseBrush();
        } else
            brushDownAtPause = false;

        xLocAtPause = motorX;
        yLocAtPause = motorY;
    }
}

//void Plotter::generateArtwork(float xStart, float yStart, float radius, int steps)
//{
//    int i = 0;
//    float r;
//    float xPos = xStart;
//    float yPos = yStart;
//
//    ToDoList = (PVector[]) append(ToDoList, new PVector(-30, 0)); //Command 30 (raise pen)
//
//    // Command Code: Move to first (X,Y) point
//    ToDoList = (PVector[]) append(ToDoList, new PVector(xPos, yPos));
//
//    ToDoList = (PVector[]) append(ToDoList, new PVector(-31, 0)); //Command 31 (lower pen)
//
//
//    // Trivial example of a generative method:
//    // Construct a random walk of constant-length steps.
//    // Continue walking until maximum number of steps OR
//    //     until we hit the walls of our page.
//
//    while (i < steps)
//    {
//        r = random(TWO_PI);
//
//        xPos = xPos + (radius * cos(r));
//        yPos = yPos + (radius * sin(r));
//
//        if ((xPos < MousePaperLeft) || (xPos > MousePaperRight))
//            break;
//        if ((yPos < MousePaperTop) || (yPos > MousePaperBottom))
//            break;
//
//        // Command Code: Move to (X,Y)
//        ToDoList = (PVector[]) append(ToDoList, new PVector(xPos, yPos));
//
//        i++;
//    }
//
//    ToDoList = (PVector[]) append(ToDoList, new PVector(-30, 0)); //Command 30 (raise pen)
//}
