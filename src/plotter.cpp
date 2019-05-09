//
//  plotter.cpp
//  interactiveGenerativePlot
//
//  Created by Melissa Holmes on 5/7/19.
//

#include "plotter.h"

Plotter::Plotter(){
    serialOnline = false;
    // User Settings:
    MotorSpeed = 4000.0;  // Steps per second, 1500 default

    ServoUpPct = 70;    // Brush UP position, %  (higher number lifts higher).
    ServoPaintPct = 30;    // Brush DOWN position, %  (higher number lifts higher).

    reverseMotorX = false;
    reverseMotorY = false;

    delayAfterRaisingBrush = 300; //ms
    delayAfterLoweringBrush = 300; //ms

    debugMode = true;
    //boolean debugMode = false;

    PaperSizeA4 = false; // true for A4. false for US letter.

//    boolean segmentQueued = false;
//    PVector queuePt1 = new PVector(-1, -1);
//    PVector queuePt2 = new PVector(-1, -1);

    float MotorStepsPerPixel = 32.1;// Good for 1/16 steps-- standard behavior.
    float PixelsPerInch = 63.3;

    // Hardware resolution: 1016 steps per inch @ 50% max resolution
    // Horizontal extent in this window frame is 740 px.
    // 2032 steps per inch * (11.69 inches (i.e., A4 length)) per 740 px gives 16.05 motor steps per pixel.
    // Vertical travel for 8.5 inches should be  (8.5 inches * 2032 steps/inch) / (32.1 steps/px) = 538 px.
    // PixelsPerInch is given by (2032 steps/inch) / (32.1 steps/px) = 63.3 pixels per inch

    yBrushRestPositionPixels = 6;

//    black = ofColor(25, 25, 25);

    doSerialConnect = true;
    DrawingPath = false;
    lastButtonUpdateX = 0;
    lastButtonUpdateY = 0;
    SubsequentWaitTime = -1;    //How long the following movement will take.

    connect();
    setup();
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
        string count = std::to_string(portCount);
        cout << "\nI found " + count + " serial ports, which are:" << endl;
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
void Plotter::raiseBrush(){
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
void Plotter::lowerBrush(){
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
void Plotter::motorsOff(){
    if (serialOnline) {
        string message = "EM,0,0\r";
        int length = message.length();
        char char_array[length + 1];
        strcpy(char_array, message.c_str());

        serial.writeBytes(char_array, length + 1);
        cout << "Motors off." << endl;
    }
}
