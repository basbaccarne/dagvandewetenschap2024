// libraries
import processing.serial.*;
import gifAnimation.*;

// configuration parameters
int serialPort = 1; // depending in the device, the serial port might be different

// main variables
String scene = "booting"; // states
PFont font1, font2; // fonts
Serial myPort; // port
String val; // serial data
String oldVal = null;

boolean disconnected = true;
String state ="booting";
float punch = 0.0;
float previousPunch = 0.0;
boolean firstHit = false;
float position = 0;
int radius = 0;
boolean expanding = false;
String subtitles = "";

int x;
int m = millis();

Gif myAnimation;

// el setup
void setup() {
  fullScreen();
  background(#222222);

  // fonts
  font1 =  createFont("JosefinSans-VariableFont_wght.ttf", 48);
  font2 = createFont("Jost-VariableFont_wght.ttf", 48);
  textFont(font1); // Set the font
  textAlign(CENTER, CENTER);
  
  ellipseMode(CENTER);

  // animated GIF
  myAnimation = new Gif(this, "data/boxer.gif");

  // Initialize the serial port
  String portName = Serial.list()[serialPort]; // set correct port
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

  // overlays 1 : connection status
  if (disconnected) {
    fill(#FF7205);
    circle(width*0.03, width*0.03, width*0.006);
    textSize(round(width*0.01));
    text("No connection", width*0.0655, width*0.0295);
  } else {
    fill(#49CE02);
    circle(width*0.03, width*0.03, width*0.006);
  }


  // overlays 2: subtitles
  int subtitleLength = subtitles.length();
  rectMode(CENTER);
  fill(#525252);
  if (subtitles != "") {
    rect(width/2, width*0.03, (subtitleLength*width*0.008)+width*0.03, width*0.03, width/100);
  }
  fill(#D8D8D8);
  textSize(round(width*0.02));
  text(subtitles, width/2, width*0.03);
  // rect();
  // text();
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
  text("Druk op spatie om verder te gaan, of klop op de zak.", width / 2, height / 1.7);
}

void idle() {

  // screen wipe
  background(#222222);

  // header
  fill(#8A8A8A);
  textSize(round(width*0.05));
  text("Klop op de zak om te beginnen", width / 2, height /4);

  // animated GIF
  myAnimation.play();
  image(myAnimation, width / 2.2, height /2);

  // set variables for next cycle$
  m = millis();

  // reser variables for next state
  punch = 0;
  firstHit = false;
}

void challenge1() {
  // screen wipe
  background(#222222);


  // uitdaging 1
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 1", width / 2, height / 6);

  // main title
  if (!firstHit) {
    fill(#8A8A8A);
    textSize(round(width*0.05));
    text("Sla zo hard als je kan!", width / 2, height / 2);
  }

  // progress bar
  int duration = 15;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);

  // remove challenge on first hit
  if (punch > 0) {
    firstHit = true;
    // baseline ball

    if (punch != previousPunch) {
      position = punch*width*0.00166;
      fill(#FFE600);
      circle(punch*width*0.00166, height/2, height/20);
      radius = height/10;
      previousPunch = punch;
      
    }

    position -= width/600;
    radius -= width/1000;
    fill(#FFE600);
    circle(position, height/2, max(radius,0));
  }
}

void challenge1_debrief() {
  // screen wipe
  background(#222222);
}

void challenge2() {
  // screen wipe
  background(#222222);
}

void challenge2_debrief() {
  // screen wipe
  background(#222222);
}

void challenge3() {
  // screen wipe
  background(#222222);
}

void challenge3_debrief() {
  // screen wipe
  background(#222222);
}

void conclusion() {
  // screen wipe
  background(#222222);
}

// function to handle serial communication
void SerialCheck() {
  // if a signal is detected
  if ( myPort.available() > 0) {
    // read it and store it in val
    val = myPort.readStringUntil('\n');
    // detect the nature of the data and store it in the right variables
    if (val != null) {
      // first trim the data (removes whitespaces at beginning & end)
      val = trim(val);
      // connection
      if (val.equals("Disconnected from central")) {
        disconnected = true;
        println("disconnected");
      } else if (val.equals("Connected")) {
        disconnected = false;
        println("connected");
        // state data
      } else if (val.startsWith("state||")) {
        String[] parts = val.split("\\|\\|");
        state = parts[1];
        print("state: ");
        println(state);
        // punch data
      } else if (val.startsWith("punchforce||")) {
        String[] parts = val.split("\\|\\|");
        print("punch: ");
        punch = float(parts[1]);
        println(punch);
        // subtitles
      } else if (val.startsWith("subtitles||")) {
        String[] parts = val.split("\\|\\|");
        print("subtitle: ");
        subtitles = parts[1];
        println(subtitles);
      }
    }
  }
}
// function to handle key presses
void keyPressed() {
  if (key == ' ') {
    switch(scene) {
    case "booting":
      scene = "idle";
      break;
    case "idle":
      scene = "challenge1";
      break;
    case "challenge1":
      scene = "challenge1_debrief";
      break;
    }
  }
}
