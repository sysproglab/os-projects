/*
  P0, P2 - I2C lines (taboo);
  P1, P4 - can be used as PWM;
  P2(A1), P3(A3), P4(A2), P5(A0) - can be used as input of ADC;
  
  Pull up resistors for GPIO are neccessary;
*/

#include "TinyWireS.h"         	// wrapper class for I2C slave routines

// GPIO
#define GPIO_I2C_SDA   	 	0   	// P0. I2C Data line  (unuse definition)
#define GPIO_DISPLAY    	1   	// P1. PWM ping for brightness of DISPLAY
#define GPIO_I2C_SCK    	2   	// P2. I2C Clock line (unuse definition)
#define GPIO_BTN        	3   	// P3. Button 
#define GPIO_RST        	4   	// P4 (quick LOW - reset)
#define GPIO_ADC        	0   	// P5 is A0; [P2(A1); P4(A2); P3(A3);]
#define GPIO_LED        	1   	// LED (P1). Debug.

// I2C
#define I2C_SLAVE_ADDR  	0x26    // slave address (0x26h=38d)
// i2c.commands by master
#define I2C_CMD_BATTERY 	0xBB    // "Check battery status" (frequency == 60 sec.)
#define I2C_CMD_CHECK   	0xCC    // "Check general status" (frequency == 1 sec.)
#define I2C_CMD_BRIGTH  	0xDD    // "Set display brightness" (async.)
#define I2C_CMD_SHUT    	0x88    // "RPi shut down" (async.)
									// OTHER commands are invalid

// GENERAL
#define DELAY           	1       // time delay between iterations in loop (it should be: 1<=x<= 10)
#define TIME_FOR_LOADING	10000		// seconds for load of RPI ***
#define MAX_CLICK_TIME  	1800    // (PressTime < MAX_CLICK_TIME) => click;
									// (PressTime >= MAX_CLICK_TIME) => pression;

// function for incriment/decrement state counters(btnPressCnt, btnReleaseCnt)
void safeIncrement();
// poor man's display (debug)
void blink(byte led, byte times);
// check connection with RPI through I2C
void checkConnection(byte cmd);



// BUTTON
bool btnPressed = false;        // flag: true - button is pressed;
// button.(protection by tinkling of contacts)
int btnPrsCnt = 0;              // counter for containing of series of moments, when button was Pressed
int btnRslCnt = 0;              // counter for containing of series of moments, when button was Released

// DISPLAY
bool blockDisplay = true;       // flag: true - display is turned ON; false - display is turned OFF (blacked)
byte blackLigth = 255;          // 0   - display is blacked;
                                // 255 - max. brightness;

// general
unsigned long time = 0;
bool shutDownEvnt = false;      // event of long pressing (User tryes to turn off the device).
bool statusOfRPi = true;       // flag status of RPi:  true - the RPi works;
                                //                      false - the device sleeps;

// the setup routine runs once when you press reset:
void setup() {    
    // initialize the digital/analog pins
    pinMode(GPIO_RST, OUTPUT);      // Reset
    pinMode(GPIO_DISPLAY, OUTPUT);  // PWM for brightness of Display
    pinMode(GPIO_BTN, INPUT);       // Button for Block/Unblock
    pinMode(GPIO_ADC, INPUT);       // ADC for Battery voltage

	// set GPIO_RST to 1 (default state); "quick 0" - reset;
    digitalWrite(GPIO_RST, HIGH);
	
    // we will see quick blinking
    blink(GPIO_LED, 3);             // poor man's display

    // init I2C Slave mode
    TinyWireS.begin(I2C_SLAVE_ADDR);
    
    // for safing (waiting of finish of all initialized commands)  
    delay (50);
}



// the loop routine runs over and over again forever:
void loop() {
	byte cmd = -1;

turboBright:
	// ******* I2C support *******
    // got I2C command from Master!
    if (TinyWireS.available()) {
		cmd = TinyWireS.receive();                 // get the byte-command from master
		
        // I2C_CMD_BRIGTH-command "Set display brightness" (async.)
        if (cmd == (byte)I2C_CMD_BRIGTH) {
            if (TinyWireS.available()) {
                blackLigth = TinyWireS.receive();       // get the blackLigth value from master
                analogWrite(GPIO_DISPLAY, blackLigth);  // update the brightness
                TinyWireS.send(blackLigth);             // send current value of brightness back to master
            }
			goto turboBright;
        }

        // I2C_CMD_BATTERY-command "Check battery status" (frequency == 60 sec.)
        else if (cmd == (byte)I2C_CMD_BATTERY) {
            int batteryValue = analogRead(GPIO_ADC);
            TinyWireS.send(batteryValue & 0xFF);        // send low byte
            TinyWireS.send(batteryValue >> 8 & 0xFF);   // send high byte
			goto turbo;
        }

        // I2C_CMD_CHECK-command "Check general status" (frequency == 1 sec.)
        else if (cmd == (byte)I2C_CMD_CHECK) {
            byte buf = 0x00;
            buf |= blockDisplay ? 0x00 : 0x01;
            buf |= shutDownEvnt ? 0x00 : 0x02;
            TinyWireS.send(buf);                        // send it back to master
			shutDownEvnt = false;						// reset flag, becouse RPi is warned
        }

        // I2C_CMD_SHUT-command "RPi shut down" (async.)
        else if (cmd == (byte)I2C_CMD_SHUT) {
            TinyWireS.send(cmd);
            analogWrite(GPIO_DISPLAY, 0);               // display is blacked
            statusOfRPi = false;                        // click and quick pressing the button is ignored  
        }
        
        // Other commands are Invalid
        else {
            //blink(GPIO_LED, 3); // debug stump ***
			//digitalWrite(GPIO_LED,LOW);
			//delay(10);
			TinyWireS.send(cmd);
		}
    }
	
// ******* BUTTON support *******
    btnPressed = (digitalRead(GPIO_BTN) == LOW);    // LOW - button is pressed
    safeIncrement();    // to incriment/decrement counters of Button states

    // quick Press or Click (button is pressed 30ms and it`s released 10ms yet,
    // we do our jon only by event - FALLING)
    if (statusOfRPi && btnPrsCnt >= 30/DELAY && btnRslCnt >= 10/DELAY && btnPrsCnt < MAX_CLICK_TIME/DELAY) {
        // block/unblock the display
        blockDisplay = !blockDisplay;

        // it uses the kept value of brightness for ublocking;
        analogWrite(GPIO_DISPLAY, blockDisplay ? 0 : blackLigth);
        
        // button is released - empty the counter
        btnPrsCnt = 0;
    }
    
    // long Press (button is pressed more than MAX_CLICK_TIME mSec.)
    if (btnPrsCnt >= MAX_CLICK_TIME/DELAY) {
		// RPi sleeps
        if (!statusOfRPi) {
			digitalWrite(GPIO_RST, LOW);				// set GPIO_RST to 0 (reset)
			delay (100);								// "quick 0" (<=100ms)
			digitalWrite(GPIO_RST, HIGH);				// back GPIO_RST to 1 (normal work)
			
			// RPi has got 10sec for starting
			statusOfRPi = true;							// RPi works
			time = millis();
        }
		// RPi works
		else {
			// flag for sending message "TURN OFF" through I2C to RPi
			shutDownEvnt = true;
		}
    }



turbo:

// ******* DELAY and CHECK_CONNECTION between itterations of cycle *******
    checkConnection(cmd);
    delay(DELAY);
}


// poor man's display (debug)
void blink(byte led, byte times) {
  for (byte i=0; i< times; i++) {
    digitalWrite(led,HIGH); delay (50);
    digitalWrite(led,LOW);  delay (50);
  }
}


// function for incriment/decrement state counters(btnPressCnt, btnReleaseCnt)
void safeIncrement() {
    // the button is PRESSED
    if (btnPressed) {
        btnPrsCnt++;			// increment the counter
        btnRslCnt = 0;
    }
    // the button is RELEASED
    else {
        btnPrsCnt--;        	// decrement the counters
        btnRslCnt++;        	// increment the counter of Released series of moments
    }
    
    // protection against an exit for time frames
    if (btnPrsCnt < 0) btnPrsCnt = 0;
    if (btnPrsCnt > 2000/DELAY) btnPrsCnt = 2000/DELAY;
    if (btnRslCnt > 2000/DELAY) btnRslCnt = 2000/DELAY;
}


// function for checking of connection
void checkConnection(byte cmd) {
	if (cmd == I2C_CMD_BATTERY
	 || cmd == I2C_CMD_CHECK
	 || cmd == I2C_CMD_BRIGTH) {
		statusOfRPi = true;		// RPi works
		time = millis();
	}
	// if connection was lost more than 10 sec.
	else if (abs(millis() - time) > TIME_FOR_LOADING * 1000) {
		statusOfRPi = false;	// RPi sleeps
		delay(1000);			// sleep 1 sec.
	}
}

