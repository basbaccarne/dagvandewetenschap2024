# PunchPal
*Tinkered with* ❤ *by Bas Baccarne & Jamil Joundi*   
*Supported with*🔥 *by Jeroen Bougonjon, Line Winey, Margot Bulcke, Maike Somers, Renée Bultijnck & Emma Tack*  
<br>
*A [comon](http://www.comon.gent) project, built for [Dag van de Wetenschap 2024](https://www.dagvandewetenschap.be/)    ([24 november 2024](https://comon.gent/evenement/kom-boksen-met-robots-op-dag-van-de-wetenschap))*
***
<img src="img/AIdeation1.png" width="400"> <img src="img/AIdeation2.png" width="400">  
***
**Punchpal is an interactive installation using punching bags and a robot to engage kids in fun physical challenges and provide personalized activity advice**
***
# General goals
* The goal of this installation is to create an engaging and interactive installation that raises awareness about the importance of physical activity, especially in complex situations like recovering after cancer treatment.
* It is designed for young children, approximately 8 years old, and should be fun and quick to participate in.
* The core of the installation consists of **three big punching bags** equipped with velostat sensors and an Arduino to read and send data, combined with a motivating robot.
* When a child punches a bag, a **robot** welcomes them in a playful way and presents a series of challenges. These challenges include hitting the bag as many times as possible in 30 seconds, hitting as hard as they can, and following specific rhythms or patterns.
* The robot provides real-time feedback through sounds and LED strips, making the experience engaging and motivating.
* After completing the challenges, the robot assesses the child's capabilities and preferences based on the collected data. It then provides **personalized advice** on physical activities that match the child's abilities.
* The installation also includes playful physical activities for the **waiting queue**, such as a balancing board and hopscotch, to keep children entertained while they wait.
* The project aims to make physical activity enjoyable and educational, promoting fitness and recovery awareness among young children.
* Overall, the installation combines fun, education, and technology to create a memorable and impactful experience for the children.

# Storyboard
* **Introduction**: As children approach the installation, they see 3 big punching bags hanging in the mdidle of the room. A friendly robot stands next to the bags, ready to welcome them.
* **Welcome**: When a child punches a bag, the robot comes to life with a playful greeting. It introduces itself and explains the fun challenges ahead. The robot's voice is cheerful and encouraging, making the children feel excited to participate.
* **Challenge 1 - Strength**: the robot challenges the child to hit the bag as hard as they can. The sensors measure the force of each punch, and the LED strips change colors based on the intensity of the punches, providing visual feedback. The robot praises the child's strength and motivates them to give their best effort. 
* **Challenge 2 - Speed**: The robot asks the child to hit the bag as many times as possible in 30 seconds. The punching bag is equipped with velostat sensors and an Arduino to measure the number of hits. The LED strips light up with each punch, and the speaker cheers them on with sounds like "aw" and "oh yeah." The robot provides real-time feedback, encouraging the child to keep going.
* **Challenge 3 - Endurance**: The final challenge tests the child's endurance by asking them to keep punching the bag for a set period. The robot and LED strips provide continuous encouragement, and the speaker plays upbeat music to keep the child motivated. The sensors track the child's performance throughout the challenge.
* **Assessment**: After the challenges, the robot processes the data collected from the punching bags. It then provides personalized advice on physical activities that match the child's capabilities and preferences. The advice is delivered in a fun and encouraging way, suggesting activities like dancing, playing tag, or simple exercises.
* **Conclusion**: The robot thanks the child for participating and encourages them to stay active. It also provides a brief educational message about the importance of physical activity, especially in recovery situations. The robot's message is designed to be both informative and motivating.
* *add billboard step & scores*
* **Waiting Queue Activities**: To keep the children entertained while waiting for their turn, the installation includes playful physical activities such as a balancing board, hopscotch, and a mini obstacle course. These activities help keep the children engaged and active, even while they wait.
  
# Components
## Waiting cue
The waiting cue is a playfull arena. Children can take a number from a [ticket dispenser](https://www.amazon.com.be/-/en/CYSSJF-Number-Ticket-Dispenser-Waiting/dp/B0BBVB9T41/ref=asc_df_B0BBVB9T41/). A screen shows the number of the next participant who can enter the boxing ring. Meanwhile, the screen also shows (in an alternating way) (1) information on the importance of physical activity and (2) movement advice. There is no cue, but waiting participants are challenged to exercise while waiting.  

- [x] [List of activities](content/Input%20Emma%20Tack.docx)
- [ ] Ticket dispenser
- [ ] Protopie / PPT for the screen
- [ ] Arena delineation (poles & ribbons)

Extra ideas:
* Balance board(s)
* Bridges
* Tunnels
* Jumping spots (different color patterns)
* Stretching station
* Web structure to climb through

## Punching bag
The punching bag is a boxing bag, hung at the ceiling, with power coming from above.  A microcontroller on the top is power through a 5V power adapter. The punch bag detects punches using velostat. This force is processed in 3 ways: 
1.  The force drives the led strip. A led strip op 120 leds is wrapped around the punch bag. From the middle the leds flare out at both sides. The bigger the punch, the wider the leds spread out.
2.  The force triggers sound effects. Whenever a puch is detected a sound effect is played.
3.  The force is send over BLE to be processed by the robot control center.
### Hardware components
* [Arduino Nano 33 IOT](https://store.arduino.cc/en-be/products/arduino-nano-33-iot?srsltid=AfmBOoqs3picykI2RmGLFcmqDr-7JXn37wr0dZKpgsBNeWgEOX8gPXjQ)
* [Velostat](https://www.dfrobot.com/product-1842.html](https://www.kiwi-electronics.com/nl/pressure-sensitive-conductive-sheet-velostat-linqstat-2808?gad_source=1&gclid=Cj0KCQjw99e4BhDiARIsAISE7P83tbKT8K8KwEgOXDDLqLYTPzCBY2VCoNgPV55UpImcOOfay8eXonoaAoaCEALw_wcB))
* [WS2812 RGB LED strip](https://www.dfrobot.com/product-1835.html)
* [Audio Driver with SD card (DFPlayer)](https://www.dfrobot.com/product-1121.html) + SD card with audio files
* [Speaker](https://www.dfrobot.com/product-1506.html) (<3W)
* 5V Power adapter (> 1A) + power connector
* Some resistors, jumper cables & breadboard(s)
* Punching bag
### Code & casing
* 💼 [Casing](https://a360.co/40fPP0c)
* ⌨️ [Punch bag code & wiring](main/punchbag.ino)

## Robot
The robot is a 3D printed robot with a moving head and LED eyes. The robot processes the data from the punch bag and speaks to the kids ([inspired by this guy](https://www.youtube.com/watch?v=6P0FF4LY1e4)). In entais the following components:
1. It receives BLE readings from the punchbag
2. It moves its head
3. It has LED controlled eyes
4. It speaks to the users
5. It controls a screen with extra information
6. It processes the BLE data (depending on the state)
7. It manages the states of the interaction
   
The robot can be in the following states: 

1.  `BOOTING`: the start up and calibration state, right after the robot is turned on
2.  `IDLE`: the initial state, waiting for a signal to start
3.  `WELCOME`: the state where the robot welcomes the user and gives the first instructions
4.  `CHALLENGE1`: the first challenge state (punch as hard as you can)
5.  `CHALLENGE1_DEBRIEF`: the debrief state after the first challeng
6.  `CHALLENGE2`: the second challenge state (punch as fast as you can)
7.  `CHALLENGE2_DEBRIEF`: the debrief state after the second challenge
8.  `CHALLENGE3`: the third challenge state (punch as accurate/steady as you can)
9.  `CHALLENGE3_DEBRIEF`: the debrief state after the third challenge
10. `CHALLENGE4`: the fourth challenge state (punch as long as you can
11. `CHALLENGE4_DEBRIEF`: the debrief state after the fourth challenge
12. `CONCLUSION`: the final state, where the robot gives the final instructions and the user can leave > goes back the IDLE state
### Hardware components
* [Arduino Nano 33 IOT](https://store.arduino.cc/en-be/products/arduino-nano-33-iot?srsltid=AfmBOoqs3picykI2RmGLFcmqDr-7JXn37wr0dZKpgsBNeWgEOX8gPXjQ)
* [Audio Driver with SD card (DFPlayer)](https://www.dfrobot.com/product-1121.html) + SD card with audio files
* [Speaker](https://www.dfrobot.com/product-1506.html) (<3W)
* 2 x [9g microservo](https://www.robotshop.com/products/feetech-9g-digital-servo-22kg-cm-fs90mg)
* 2 x [Adafruit Mini 8x8 LED Matrix w/I2C Backpack](https://www.adafruit.com/product/870)
* Some jumper cables, resistors and a breadboard

### Code & casing
* 💼 [Casing](https://a360.co/4fcM1B4)
* ⌨️ [Robot code & wiring](main/robot)

## Screen
The screen is a protopie screen with extra text and information. The screens are triggered and 'data fed' thorugh serial messages of the robot arduino. This screen runs on a raspberry pi with and external screen.
   
# Subchallenges
Before creating the final prototypes, we solve a couple of subchallenges
- [x] Punch: Simple velostat test & 3.3V optimiziation ([test](tests/punch%20simple%20velostat.ino))
- [X] Punch: Calibrate sensor and trigger on punch [[test](tests/punch.ino)]
- [x] Punch: Reveive trigger and play a sound ([sound tests](tests/DFplayer.ino))
- [x] Punch: Led strip [punch test](tests/LEDstrip_punchtest.ino) & [simple rainbox test](tests/LEDstrip_rainbowtest.ino)
***
- [X] Oefening 1: Max force ([test](tests/highesthit_hitcounter.ino))
- [ ] Oefening 2: Punch Count ([test](tests/highesthit_hitcounter.ino)) (not optimal yet)
- [ ] Oefening 3: Conditie/slabbaken
***
- [X] Commz: Send trigger from Arduino A to Arduino B over UART ([master](tests/UART_master.ino) & [slave](tests/UART_master.ino))
- [X] Commz: Send trigger from Arduino A to Arduino B over BLE ([sender](tests/BLE_sender.ino) & [receiver](tests/BLE_receiver.ino)) ([simple demo sender](tests/BLE_sender_v2.ino))
---
- [X] Robot: Anthropomorphic stuff with two 8x8 led matrices ([eyes](tests/anthro_eyes.ino))
- [X] Robot: servo mounts ([head movement](tests/anthro_headmovement.ino))([simple test script to test if the servo is still working](tests/servo_tester.ino))
- [X] Robot: [integrated head movement & eye movement](tests/robot_intergration.ino)
- [x] Robot: Reveive trigger and play a sound ([sound tests](tests/DFplayer.ino) // [sound tests with status pin](tests/DFplayer_with_status.ino))
- [ ] Robot: outer casing (almost there)
- [ ] Robot: Making the speaker loud enough (extra line out?)
---
- [ ] Extra screen: send signal to Raspi with Protopie
- [ ] Extra screen: show stuff using Protopie with Serial switches
---
- [x] [State machine](tests/statemachine_simplesycle.ino)
- [x] [.h and .cpp test](tests/multifilescripting)
      
# Benchmarks 4 inspiration
* [DIY Smart Punching Bag](https://www.instructables.com/Smart-Punching-Bag/)
* [Project HitBox](https://projects.learningplanetinstitute.org/projects/hitbox-interactive-boxing-bag/description)
* [Padipata Interactive Punching Bag](https://www.facebook.com/GIGadgets.Fans/videos/1075264679489205/)
* [Interactive Inflatable Punch Bag for Kids](https://rainbowcolours.com/products/interactive-inflatable-led-musical-punch-bag?srsltid=AfmBOorio1E10Jz_inflCE2i_M-X3OUl_Oub8z6nl7XFRLsQSvkXiS3e)
* [IMU based Punching Pole](https://www.instructables.com/Interactive-Reflex-Bag/) [Variant 2](https://www.instructables.com/Pressure-Punch-Final-Project/)
