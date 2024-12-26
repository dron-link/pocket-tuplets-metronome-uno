# Pocket Tuplets Metronome for Arduino UNO

A **Rhythm Division Metronome**

## Features

- Only has 4 components
- You can carry it in your pocket
- Choose from duplet to 12-tuplet
- Manually change downbeat speed, or set a N-tuplet as the new downbeat speed

Warning: This sketch works, but it sees no active development, and the effects of longterm use are unknown.

## Parts

- Arduino UNO + power for this Arduino
- Jumper that can connect any pin to a ground pin
- 5V Active Buzzer connected across A0 pin and ground pin

## How to use

- Connect the buzzer module between pin A0 and the closest GND
- Put one end of a jumper in GND
- Power the arduino

The initial speed is 1 strong beat per second.

### To establish a manual strong beat speed

- Introduce the ground jumper in pin A2. Sound will stop.
- Touch pin A4 then touch pin A5 at the pace that you want the strong beat to have. tip: doing this on the underside of the board is easier.

### Subdivisions of the strong beat

- Insert the ground jumper in one of the pins numbered 2 to 12. The number of the pin will correspond to the subdivision.
- To return to base speed (strong beat only speed) insert the ground jumper in pin A5.

### Set current beats as the strong beat

- Insert the ground jumper in pin A3. This will enter you into the "Set Current Beat As" config mode.
- Insert the ground jumper in pin A5. The beat you listen to is now the base speed.
Do this, if you want what you hear to be the strong beat, and you want to subdivide that.

### Set current beat speed as the Nth subdivision of the new base beat

- Insert the ground jumper in pin A3. This will enter you into the "Set Current Beat As" config mode.
- Insert the ground jumper in the pin N (choose from pin 2 ...to pin 12). The beat you listen to is now the subdivision number N of some new base speed.

The beat you hear won't change, but if you return to strong beat speed (pin A5) and then touch the same subdivision pin as before, you will notice that you were listening *to what is now the Nth subdivision* of the base beat (strong beat) speed.
Use this in case you want to reimagine the speed and groupings of the beats you hear.

## Author

*dron-link*

*2024*
