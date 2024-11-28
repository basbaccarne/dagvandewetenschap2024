// libraries
import processing.serial.*;

// main variables
String scene = "booting"; // states
PFont font1, font2; // fonts
Serial myPort; // port
String val; // serial data

// el setup
void setup() {
  fullScreen();
  background(#222222);

  font1 =  createFont("JosefinSans-VariableFont_wght.ttf", 48);
  font2 = createFont("Jost-VariableFont_wght.ttf", 48);
  textFont(font1); // Set the font
  textAlign(CENTER, CENTER);
  

  // Initialize the serial port
  String portName = Serial.list()[0]; // set correct port
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n'); // Buffer until newline character
}

// big draw function & scene/state machine
void draw() {
  
  // check for serial data
  SerialCheck();

  // state machine
  switch(scene) {
  case "booting":
    booting();
    break;
  case "idle":
    idle();
    break;
  case "challenge1":
    challenge1();
    break;
  case "challenge1_debrief":
    challenge1_debrief();
    break;
  case "challenge2":
    challenge2();
    break;
  case "challenge2_debrief":
    challenge2_debrief();
    break;
  case "challenge3":
    challenge3();
    break;
  case "challenge3_debrief":
    challenge3_debrief();
    break;
  case "conclusion":
    conclusion();
    break;
  }
}

// boot screen waits for a signal
void booting() {
  // always wipe screen first
  background(#222222);

  // main title
  fill(#8A8A8A);
  textSize(round(width*0.05));
  text("Hello, World!", width / 2, height / 2);

  // header 2
  fill(#FFE600);
  textSize(round(width*0.025));
  text("Druk op spatie om verder te gaan, of klop op de zak.", width / 2, height / 2 + 80);
}

void idle() {
  background(#222222);
}

void challenge1() {
}

void challenge1_debrief() {
}

void challenge2() {
}

void challenge2_debrief() {
}

void challenge3() {
}

void challenge3_debrief() {
}

void conclusion() {
}

// function to handle serial communication
void SerialCheck() {

  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('\n'); // read it and store it in val
  }

  // print the result for debugging
  print("serial incoming: ");
  println(val);

}
// function to handle key presses
void keyPressed() {
  if (key == ' ') {
    scene = "idle";
  }
}
