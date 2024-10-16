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
* The core of the installation consists of **five big punching bags** equipped with velostat sensors and an Arduino to read and send data, combined with a [Dwengo](https://www.dwengo.org/) robot.
* When a child punches a bag, a **robot** welcomes them in a playful way and presents a series of challenges. These challenges include hitting the bag as many times as possible in 30 seconds, hitting as hard as they can, and following specific rhythms or patterns.
* The robot provides real-time feedback through sounds and LED strips, making the experience engaging and motivating.
* After completing the challenges, the robot assesses the child's capabilities and preferences based on the collected data. It then provides **personalized advice** on physical activities that match the child's abilities.
* The installation also includes playful physical activities for the **waiting queue**, such as a balancing board and hopscotch, to keep children entertained while they wait.
* The project aims to make physical activity enjoyable and educational, promoting fitness and recovery awareness among young children.
* Overall, the installation combines fun, education, and technology to create a memorable and impactful experience for the children.

# Storyboard
* **Introduction**: As children approach the installation, they see 3 big punching bags hanging in the mdidle of the room. A friendly robot stands next to the bags, ready to welcome them.
* **Welcome**: When a child punches a bag, the robot comes to life with a playful greeting. It introduces itself and explains the fun challenges ahead. The robot's voice is cheerful and encouraging, making the children feel excited to participate.
* **Challenge 1 - Speed**: The robot asks the child to hit the bag as many times as possible in 30 seconds. The punching bag is equipped with velostat sensors and an Arduino to measure the number of hits. The LED strips light up with each punch, and the speaker cheers them on with sounds like "aw" and "oh yeah." The robot provides real-time feedback, encouraging the child to keep going.
* **Challenge 2 - Strength**: Next, the robot challenges the child to hit the bag as hard as they can. The sensors measure the force of each punch, and the LED strips change colors based on the intensity of the punches, providing visual feedback. The robot praises the child's strength and motivates them to give their best effort.
* **Challenge 3 - Coordination**: The robot introduces a coordination challenge, such as hitting the bag in a specific rhythm or pattern. The LED strips guide the child with changing colors, and the robot provides instructions and encouragement. This challenge helps improve the child's timing and coordination skills.
* **Challenge 4 - Endurance**: The final challenge tests the child's endurance by asking them to keep punching the bag for a set period. The robot and LED strips provide continuous encouragement, and the speaker plays upbeat music to keep the child motivated. The sensors track the child's performance throughout the challenge.
* **Assessment**: After the challenges, the robot processes the data collected from the punching bags. It then provides personalized advice on physical activities that match the child's capabilities and preferences. The advice is delivered in a fun and encouraging way, suggesting activities like dancing, playing tag, or simple exercises.
* **Conclusion**: The robot thanks the child for participating and encourages them to stay active. It also provides a brief educational message about the importance of physical activity, especially in recovery situations. The robot's message is designed to be both informative and motivating.
* *add billboard step & scores*
* **Waiting Queue Activities**: To keep the children entertained while waiting for their turn, the installation includes playful physical activities such as a balancing board, hopscotch, and a mini obstacle course. These activities help keep the children engaged and active, even while they wait.
  
# Components
## Waiting cue
The waiting cue is a playfull arena. Children can take a number from a [ticket dispenser](https://www.amazon.com.be/-/en/CYSSJF-Number-Ticket-Dispenser-Waiting/dp/B0BBVB9T41/ref=asc_df_B0BBVB9T41/). A screen shows the number of the next participant who can enter the boxing ring. Meanwhile, the screen also shows (in an alternating way) (1) information on the importance of physical activity and (2) movement advice. There is no cue, but waiting participants are challenged to exercise while waiting.  

- [ ] Ticket dispenser
- [ ] Protopie / PPT for the screen
- [ ] Arena delineation (poles & ribbons)
- [x] [List of activities](content/Input%20Emma%20Tack.docx)

Extra ideas:
* Balance board(s)
* Bridges
* Tunnels
* Jumping spots (different color patterns)
* Stretching station
* Web structure to climb through

## Punching bag
The punhcing bag is ...  
* Velostat (temporary: [RP-S40-ST Thin Film Pressure Sensor](https://www.dfrobot.com/product-1842.html))
* 2x [Arduino Nano ESP32](https://store.arduino.cc/en-be/products/nano-esp32-with-headers) 
* WS2812 RGB [LED strip](https://www.dfrobot.com/product-1835.html)
* Audio Driver with SD card ([DFPlayer](https://www.dfrobot.com/product-1121.html)) + SD card
* [Speaker](https://www.dfrobot.com/product-1506.html) (<3W)
* Power
* Punching bag
* We could wrap things around the punhcing bag with cling film

## Robot
The robot is ...  
* Active plan: our own robot: a WALL-E like head attached to the punching bag (something [like this](https://www.youtube.com/watch?v=6P0FF4LY1e4))
  - 2 x 8x8 LED matrix for the eyes
  - 2 x microservo for simple head movement
  - Core Arduino to process everything
  - 3D printed body
  - tension strap to attach the body to the bag
* Backup plan
   - Dwengo robot (meerwaarde?)
   - Casing for the robot
 
## Screen
The screen is ...
   
# Subchallenges
Before creating the final prototypes, we solve a couple of subchallenges
- [x] Punch: Simple velostat test & 3.3V optimiziation ([test](tests/punch%20simple%20velostat.ino))
- [X] Punch: Calibrate sensor and trigger on punch [[test](tests/punch.ino)]
***
- [X] Oefening 1: Max force ([test](tests/highesthit_hitcounter.ino))
- [ ] Oefening 2: Punch Count ([test](tests/highesthit_hitcounter.ino)) (not optimal yet)
- [ ] Oefening 3: Conditie/slabbaken
***
- [X] Commz: Send trigger from Arduino A to Arduino B over UART ([master](tests/UART_master.ino) & [slave](tests/UART_master.ino))
- [X] Commz: Send trigger from Arduino A to Arduino B over BLE ([sender](tests/BLE_sender.ino) & [receiver](tests/BLE_receiver.ino))
---
- [X] LED: Do some stuff with the LED strip [[rainbow test](tests/LEDstrip_rainbowtest.ino), [punch test](tests/LEDstrip_punchtest.ino)]
---
- [X] Control Room: Build a state machine ([test](tests/statemachine_simplesycle.ino))
---
- [X] Robot: Anthropomorphic stuff with two 8x8 led matrices ([eyes](tests/anthro_eyes.ino))
- [ ] Robot: servo mounts ([head movement](tests/anthro_headmovement.ino)) (almost there)
- [ ] Robot: outer casing (almost there)
- [ ] Robot: Reveive trigger and play a sound ([sound tests](tests/DFplayer.ino))
- [ ] Robot: Making the speaker loud enough (extra line out?)
- [ ] Robot: Sending a signal to a screen for more info
---
- [ ] Extra screen: send signal to Raspi with Protopie
- [ ] Extra screen: show stuff using Protopie with Serial switches
---
- [ ] [Diagram](https://www.figma.com/board/RnfKpI49EF7PNarbsCpS0A/DVDW24?node-id=0-1&t=k4dAAmsSwIFww2ve-1) (probably to ommit)

# Benchmarks 4 inspiration
* [DIY Smart Punching Bag](https://www.instructables.com/Smart-Punching-Bag/)
* [Project HitBox](https://projects.learningplanetinstitute.org/projects/hitbox-interactive-boxing-bag/description)
* [Padipata Interactive Punching Bag](https://www.facebook.com/GIGadgets.Fans/videos/1075264679489205/)
* [Interactive Inflatable Punch Bag for Kids](https://rainbowcolours.com/products/interactive-inflatable-led-musical-punch-bag?srsltid=AfmBOorio1E10Jz_inflCE2i_M-X3OUl_Oub8z6nl7XFRLsQSvkXiS3e)
* [IMU based Punching Pole](https://www.instructables.com/Interactive-Reflex-Bag/) [Variant 2](https://www.instructables.com/Pressure-Punch-Final-Project/)
