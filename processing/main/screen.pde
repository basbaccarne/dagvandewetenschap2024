// libraries
import processing.serial.*;
import gifAnimation.*;

// configuration parameters
int serialPort = 1; // depending in the device, the serial port might be different

// main variables
String scene = "BOOTING"; // states
PFont font1, font2; // fonts
Serial myPort; // port
String val; // serial data
String oldVal = null;

boolean disconnected = true;
String state ="booting";
String oldState;
float punch = 0.0;
float previousPunch = 0.0;
float max_force = 0.0;
boolean firstHit = false;
boolean firstHitSpeed = false;
boolean firstHitRitme = false;
float punch_counter = 0.0;
float punch_variance = 0.0;
float punch_deviance = 0.0;
float mantis = 0.0;
float hummingbird = 0.0;
float maki = 0.0;
float final_animal = 0.0;
float position = 0;
int radius = 0;
String subtitles = "";

int x;
int m = millis();

Gif myAnimation;
PImage img1, img2, img3;

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
  rectMode(CENTER);

  // animated GIF
  myAnimation = new Gif(this, "data/boxer.gif");

  // animal images
  img1 = loadImage("data/mantis_circle.png");
  img2 = loadImage("data/hummingbird_circle.png");
  img3 = loadImage("data/maki_circle.png");

  img1.resize(height/3, height/3);
  img2.resize(height/3, height/3);
  img3.resize(height/3, height/3);

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
  case "BOOTING":
    booting();
    break;
  case "IDLE":
    idle();
    break;
  case "CHALLENGE1":
    challenge1();
    break;
  case "CHALLENGE1_DEBRIEF":
    challenge1_debrief();
    break;
  case "CHALLENGE2":
    challenge2();
    break;
  case "CHALLENGE2_DEBRIEF":
    challenge2_debrief();
    break;
  case "CHALLENGE3":
    challenge3();
    break;
  case "CHALLENGE3_DEBRIEF":
    challenge3_debrief();
    break;
  case "CONCLUSION":
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
  fill(#525252);
  if (subtitles != "") {
    rectMode(CENTER);
    rect(width/2, width*0.03, (subtitleLength*width*0.008)+width*0.03, width*0.03, width/100);
  }
  fill(#D8D8D8);
  textSize(round(width*0.02));
  text(subtitles, width/2, width*0.03);
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
}

void challenge1() {
  // screen wipe
  background(#222222);

  // text: uitdaging 1
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 1: Kracht", width / 2, height / 6);

  // text: main title
  if (!firstHit) {
    fill(#8A8A8A);
    textSize(round(width*0.05));
    text("Sla zo hard als je kan!", width / 2, height / 2);
  }

  // moving progress bar
  int duration = 15;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);

  if (punch > 0) {
    // remove challenge on first hit
    firstHit = true;

    // ball: max force
    fill(#525252);
    circle(max_force*width*0.00166, height/2, height/5);
    textSize(round(width*0.05));
    text(round(max_force), max_force*width*0.00166, height/2-height/7);

    // ball: last hit
    if (punch != previousPunch) {
      position = punch*width*0.00166;
      fill(#FFE600);
      circle(punch*width*0.00166, height/2, height/10);
      radius = height/10;
      previousPunch = punch;
    }

    // slowly move left
    position -= width/600;
    radius -= width/1000;
    fill(#FFE600);
    circle(position, height/2, max(radius, 0));
  }
}

void challenge1_debrief() {
  // screen wipe
  background(#222222);

  // text jouw krachtscore
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 1: Jouw krachtscore", width / 2, height-height / 6);

  // score
  fill(#525252);
  rectMode(CENTER);
  rect(width/2, height/2, width/7, width/7, 60);
  fill(#FFFFFF);
  textSize(round(width*0.060));
  text(round(max_force), width/2, height/2);

  // moving progress bar
  int duration = 12;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);
}

void challenge2() {
  // screen wipe
  background(#222222);

  // text: uitdaging 2
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 2: Snelheid", width / 2, height / 6);

  // text: main title
  if (!firstHitSpeed) {
    fill(#8A8A8A);
    textSize(round(width*0.05));
    text("Sla zo vaak als je kan in 30 seconden!", width / 2, height / 2);
  }

  // moving progress bar
  int duration = 30;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);

  if (punch > 0) {
    // remove challenge on first hit
    firstHitSpeed = true;
    // score
    fill(#525252);
    rectMode(CENTER);
    rect(width/2, height/2, width/7, width/7, 60);
    fill(#FFFFFF);
    textSize(round(width*0.070));
    text(round(punch_counter), width/2, height/2);
  }
}

void challenge2_debrief() {
  // screen wipe
  background(#222222);

  // text jouw snelheidsscore
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 2: Jouw snelheid", width / 2, height-height / 6);

  // score
  fill(#525252);
  rectMode(CENTER);
  rect(width/2, height/2, width/7, width/7, 60);
  fill(#FFFFFF);
  textSize(round(width*0.060));
  text(round(punch_counter), width/2, height/2);

  // moving progress bar
  int duration = 15;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);
}

void challenge3() {
  // screen wipe
  background(#222222);

  // text: uitdaging 3
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 3: Ritme", width / 2, height / 6);

  // text: main title
  if (!firstHitRitme) {
    fill(#8A8A8A);
    textSize(round(width*0.05));
    text("Sla zo regelmatig mogelijk!", width / 2, height / 2);
  }

  // moving progress bar
  int duration = 30;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);

  // variance
  if (punch > 0) {
    // remove challenge on first hit
    firstHitRitme = true;

    // zero stokes
    stroke(#525252);
    line(width/4, height/4, width/4, 3*height/4);
    line(3*width/4-width/8, height/2, 3*width/4+width/8, height/2);

    // deviance bar
    textSize(round(width*0.02));
    if (punch_deviance > 0) {
      rectMode(CORNERS);
      fill(#FFC800);
      rect(width/4, height/2+height/10, width/4 + min(punch_deviance, width/2), height/2-height/10);
      String message = abs(round(punch_deviance))+ " milliseconden \n te snel";
      text(message, width/4+width/10, 3*height/4);
    } else if (punch_deviance < 0) {
      rectMode(CORNERS);
      fill(#FFC800);
      rect(width/4, height/2+height/10, width/4 + punch_deviance, height/2-height/10);
      String message = abs(round(punch_deviance))+ " milliseconden \n te traag";
      text(message, width/4-width/10, 3*height/4);
    }

    // variance bar
    rectMode(CORNERS);
    fill(#FFC800);
    rect(3*width/4-width/10, height/2-punch_variance, 3*width/4+width/10, height/2+punch_variance);
    text("gemiddelde fout: \n" + abs(round(punch_variance)), 3*width/4-width/5, height/2);
  }
}

void challenge3_debrief() {
  // screen wipe
  background(#222222);

  // text jouw ritmescore
  fill(#FFE600);
  textSize(round(width*0.020));
  text("Uitdaging 2: Jouw ritmescore \n (gemiddelde afwijking van het rimte, in milliseconden)", width / 2, height-height / 6);

  // score
  fill(#525252);
  rectMode(CENTER);
  rect(width/2, height/2, width/7, width/7, 60);
  fill(#FFFFFF);
  textSize(round(width*0.060));
  text(round(punch_variance), width/2, height/2);

  // moving progress bar
  int duration = 5;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);
}

void conclusion() {
  // screen wipe
  background(#222222);

  // selection circle
  fill(#FFC800);

  // replace with actual stuff
  if (final_animal ==1) {
    circle(width*0.25, height/2, height/2.7);
  } else if (final_animal == 2) {
    circle(width/2, height/2, height/2.7);
  } else if (final_animal == 3) {
    circle(width*0.75, height/2, height/2.7);
  }

  // images
  image(img1, (width*0.25) - (img1.width/2), (height/2) - (img1.width/2));
  image(img2, (width/2) - (img2.width/2), (height/2) - (img2.width/2));
  image(img3, (width*0.75) - (img3.width/2), (height/2) - (img3.width/2));

  // stars mantis
  if (mantis == 1) {
    star(width*0.25, height/5, height*0.03, height*0.015, 5);
  } else if (mantis == 2) {
    star(width*0.25-(height*0.04), height/5, height*0.03, height*0.015, 5);
    star(width*0.25+(height*0.04), height/5, height*0.03, height*0.015, 5);
  } else if (mantis == 3) {
    star(width*0.25-(height*0.08), height/5, height*0.03, height*0.015, 5);
    star(width*0.25, height/5, height*0.03, height*0.015, 5);
    star(width*0.25+(height*0.08), height/5, height*0.03, height*0.015, 5);
  }

  // stars humming
  if (hummingbird == 1) {
    star(width/2, height/5, height*0.03, height*0.015, 5);
  } else if (hummingbird == 2) {
    star(width/2-(height*0.04), height/5, height*0.03, height*0.015, 5);
    star(width/2+(height*0.04), height/5, height*0.03, height*0.015, 5);
  } else if (hummingbird == 3) {
    star(width/2-(height*0.08), height/5, height*0.03, height*0.015, 5);
    star(width/2, height/5, height*0.03, height*0.015, 5);
    star(width/2+(height*0.08), height/5, height*0.03, height*0.015, 5);
  }

  // stars maki
  if (maki == 1) {
    star(width*0.75, height/5, height*0.03, height*0.015, 5);
  } else if (maki == 2) {
    star(width*0.75-(height*0.04), height/5, height*0.03, height*0.015, 5);
    star(width*0.75+(height*0.04), height/5, height*0.03, height*0.015, 5);
  } else if (maki == 3) {
    star(width*0.75-(height*0.08), height/5, height*0.03, height*0.015, 5);
    star(width*0.75, height/5, height*0.03, height*0.015, 5);
    star(width*0.75+(height*0.08), height/5, height*0.03, height*0.015, 5);
  }

  // scores
  fill(#525252);
  rectMode(CENTER);
  rect(width*0.25, height*0.7, (width/7)+width*0.05, width*0.03, width/100);
  rect(width/2, height*0.7, (width/7)+width*0.05, width*0.03, width/100);
  rect(width*0.75, height*0.7, (width/7)+width*0.05, width*0.03, width/100);

  fill(#D8D8D8);
  textSize(round(width*0.02));
  text("krachtscore: " + round(max_force), width*0.25, height*0.7);
  text("aantal slagen: "+ round(punch_counter), width/2, height*0.7);
  text("ritmefout (ms): "+ round(punch_variance), width*0.75, height*0.7);

  // final animal
  fill(#8A8A8A);
  textSize(round(width*0.05));
  if (final_animal == 0) {
    text("Jij bent een ...", width / 2, height * 0.8);
  } else if (final_animal == 1) {
    text("mantis!", width / 2, height * 0.8);
  } else if (final_animal == 2) {
    text("kolibri!", width / 2, height * 0.8);
  } else if (final_animal == 3) {
    text("Japanse sneeuwaap!", width / 2, height * 0.8);
  }

  // moving progress bar
  int duration = 20;
  fill(#FFC800);
  float postion = width - ((millis()-m))*(width/duration)/1000;
  rectMode(CORNER);
  rect(0, height-height/20, postion, height-height/20);

  // text: timer
  fill(#FFE600);
  textSize(round(width*0.05));
  int timer = duration-(round(millis()-m)/1000);
  text(timer, width*0.95, height*0.9);
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
        // max force
      } else if (val.startsWith("max_force||")) {
        String[] parts = val.split("\\|\\|");
        print("max_force: ");
        max_force = float(parts[1]);
        println(max_force);
        // punchcounter
      } else if (val.startsWith("punch_counter||")) {
        String[] parts = val.split("\\|\\|");
        print("punch_counter: ");
        punch_counter = float(parts[1]);
        println(punch_counter);
        // punch deviance
      } else if (val.startsWith("punch_deviance||")) {
        String[] parts = val.split("\\|\\|");
        print("punch_deviance: ");
        punch_deviance = float(parts[1]);
        println(punch_deviance);
        // punch variance
      } else if (val.startsWith("punch_variance||")) {
        String[] parts = val.split("\\|\\|");
        print("punch_variance: ");
        punch_variance = float(parts[1]);
        println(punch_variance);
        // mantis
      } else if (val.startsWith("mantis||")) {
        String[] parts = val.split("\\|\\|");
        print("mantis: ");
        mantis = float(parts[1]);
        println(mantis);
        // hummingbird
      } else if (val.startsWith("hummingbird||")) {
        String[] parts = val.split("\\|\\|");
        print("hummingbird: ");
        hummingbird = float(parts[1]);
        println(hummingbird);
        // punch variance
      } else if (val.startsWith("maki||")) {
        String[] parts = val.split("\\|\\|");
        print("maki: ");
        maki = float(parts[1]);
        println(maki);
        // final_animal
      } else if (val.startsWith("final_animal||")) {
        String[] parts = val.split("\\|\\|");
        print("final_animal: ");
        final_animal = float(parts[1]);
        println(final_animal);
        // subtitles
      } else if (val.startsWith("subtitles||")) {
        String[] parts = val.split("\\|\\|");
        print("subtitle: ");
        subtitles = parts[1];
        println(subtitles);
      }
    }
  }

  setState();
}

void setState() {
  if (state != oldState) {
    oldState = state;
    println("Changing state to: " + state);
    scene = state;
    resetVariables();
    m = millis();
    if (oldState == "CONCLUSION"){
      resetMain();
    }
  }
}

void resetVariables() {
  // reset all the stuff besides main variables
  punch = 0.0;
  previousPunch = 0.0;
  firstHit = false;
  firstHitSpeed = false;
  firstHitRitme = false;
  punch_deviance = 0.0;
  position = 0;
  radius = 0;
}

void resetMain() {
  max_force = 0.0;
  punch_counter = 0.0;
  punch_variance = 0.0;
  mantis = 0.0;
  hummingbird = 0.0;
  maki = 0.0;
  final_animal = 0.0;
}


// function to handle key presses
void keyPressed() {
  if (key == ' ') {
    switch(scene) {
    case "BOOTING":
      scene = "IDLE";
      resetVariables();
      m = millis();
      break;
    case "IDLE":
      scene = "CHALLENGE1";
      resetVariables();
      m = millis();
      break;
    case "CHALLENGE1":
      scene = "CHALLENGE1_DEBRIEF";
      m = millis();
      break;
    case "CHALLENGE1_DEBRIEF":
      scene = "CHALLENGE2";
      resetVariables();
      m = millis();
      break;
    case "CHALLENGE2":
      scene = "CHALLENGE2_DEBRIEF";
      m = millis();
      break;
    case "CHALLENGE2_DEBRIEF":
      scene = "CHALLENGE3";
      resetVariables();
      m = millis();
      break;
    case "CHALLENGE3":
      scene = "CHALLENGE3_DEBRIEF";
      m = millis();
      break;
    case "CHALLENGE3_DEBRIEF":
      scene = "CONCLUSION";
      resetVariables();
      m = millis();
      break;
    case "CONCLUSION":
      scene = "IDLE";
      resetVariables();
      resetMain();
      m = millis();
      break;
    }
  }
}


void star(float x, float y, float radius1, float radius2, int npoints) {
  float angle = TWO_PI / npoints;
  float halfAngle = angle/2.0;
  beginShape();
  for (float a = 0; a < TWO_PI; a += angle) {
    float sx = x + cos(a) * radius2;
    float sy = y + sin(a) * radius2;
    vertex(sx, sy);
    sx = x + cos(a+halfAngle) * radius1;
    sy = y + sin(a+halfAngle) * radius1;
    vertex(sx, sy);
  }
  endShape(CLOSE);
}
