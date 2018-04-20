# GameShowButtons
Game Show setup w/2 player buttons (first to hit 'wins' round) using PJRC Teensy, Neopixel leds, buttons, sounds, etc.  In Process.
This is the electronics for a podium to support a game show akin to Family Feud (etc), where two players try to hit their respective buttons first after hearing a question.  Winner gets to answer the question with their teammates.
The system has 2 large arcade buttons, three smaller mode buttons for the host, Neopixel leds in each of the buttons, some led strips adorning the podium edges, and audio output for various sounds.  It is intended to get power from either a battery system or wall AC.
The system needs to be packed up for mobile travel to.from venues with easy setup.  The physical podium was not designed when we started back in 2016, but in 2018 we have a nice folding design using 80/20 aluminum, acrylic and wood, with fabric covering.  The mobile requirement, and prototyping, made it essential to have connectors between everything.  The project evolved from breadboad wires, to crimp connectors, to RJ-45 cat 5 cables, to lever connectors, to ???  The 7 basic out components had over 100 connections at some points in the design.

Main Components:
* PJRC Teensy 3.2 microcontroller (https://www.pjrc.com/store/teensy32.html)
* PJRC Audio Adapter/SDCard board (https://www.pjrc.com/store/teensy3_audio.html)
* PJRC OctoWS2811 Adapter (https://www.pjrc.com/store/octo28_adaptor.html)
* Adafruit Arcade Buttons (https://learn.adafruit.com/neopixel-arcade-button/overview)
* Adafruit Massive Arcade Buttons (https://www.adafruit.com/product/1187)
* Adafruit Neopixel Ring (https://www.adafruit.com/product/1643)
* WS8211 rgbw pixels (https://www.amazon.com/gp/product/B01K4HTXOC)
* WS8211 rgbw strip (https://www.amazon.com/gp/product/B01MTW2L6B)

The large buttons have a center LED plus a neopixel ring inside (per http://www.instructables.com/id/Neopixel-Dome-Button/).  Each of these then has connections for: 5v power, ground, button sense, led drive, ring drive (5, with gnd going to several points)
The smaller buttons have single neopixel inside. (5v power, ground, button sense, led in, led out) and are daisy chained.
The LED strip is divided into several sections physically and logically, but uses only one pin on the teensy.  A second LED strip may be added to the top platform of the podium. These would be separate control pins and power from the ones on podium base.

(ok maybe all are not Adafruit NeoPixel, but they are compatible. The above linked rgbww singles are smaller than the Adafruit Flora pixels and fit inside buttons better)

A raw photo gallery of project in process is at https://photos.app.goo.gl/xcB1iYuDJkAJ8V432

Wiring is a bitch.
