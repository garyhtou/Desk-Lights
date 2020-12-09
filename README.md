# Desk Lights

An ESP8266 powered system connected to a Firebase Realtime Database to control a LED strip.

This README is more of a "how it works" and doesn't really serve as a great tutorial/guide.

This project started back in January 2019 with the goal of wirelessly controlling an individually addressable LED strip. After some faulty wiring that melted a breadboard and fried a test strip of LEDs... whoops!... I picked it back up in December 2020.

## Get Started

- Wire up the system
- Create a Firebase Project, create a Real-Time Database within the project, and grab your Database URL and auth token.
- Replace the `WIFI_SSID`, `WIFI_PASSWORD`, `FIREBASE_HOST`, and `FIREBASE_AUTH` at the top of [`Desk_Lights.ino`](Desk_Lights/Desk_Lights.ino).
- Download and install Arduino IDE
- Install the CP2104 USB Driver ([more info here](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide))
- Set up your Adafruit Feather HUZZAH with ESP8266 as a board in Arduino IDE
- Upload the updated code via the Arduino IDE
- Enter some values on Firebase
- Plug it in!

View sections below for more information relating to these steps.

## Parts

| Product | Supplier/Link |
|-|-|
| Adafruit NeoPixel Digital RGBW LED Strip - White PCB 60 LED/m 1m | [Adafruit](https://www.adafruit.com/product/2842?length=1) |
| Assembled Adafruit Feather HUZZAH with ESP8266 With Headers | [Adafruit](https://www.adafruit.com/product/3046) |
| 74AHCT125 - Quad Level-Shifter (3V to 5V) | [Adafruit](https://www.adafruit.com/product/1787) |
| 1000µf 25V Capacitor (or any 1000 µF, 6.3V or higher) | I think I bought them from [Amazon](https://www.amazon.com/McIgIcM-1000uf-capacitor-Aluminum-electrolytic/dp/B06WGPNM19) |
| 470 Ohms Resistor (or any between 300 to 500 Ohm) |  |
| Any breadboard and wires |  |
| Soldering tool may be required |  |

## Wiring

- 5V to ESP8266 USB Pin
- ESP8266 GND Pin to Ground
- ESP8266 14 Pin to 74AHCT125 1A Pin
- 74AHCT125 Vcc Pin to 5V
- 74AHCT125 GND Pin to Ground
- 74AHCT125 1OE Pin to Ground
- NeoPixel 5V Pin/wire to 5V
- NeoPixel GND Pin/wire to Ground
- NeoPixel Din Pin/wire to 74AHCT125 1Y Pin
- 1000uf 25V Capacitor across the incoming + and -

Because the ESP8266 runs on 3.3V logic, the data line from Pin 14 must be level-shifted to 5V in order for the NeoPixels to understand. The 74AHCT125 does this for us.

| ![Wiring Full View.JPG](Wiring%20-%20Full%20View.JPG) | ![Wiring - Adafruit Feather Huzzah ESP8266 close up.JPG](Wiring%20-%20Adafruit%20Feather%20Huzzah%20ESP8266%20close%20up.JPG) | ![Wiring - Level-Shifter (3V to 5V) Adafruit 74AHCT125 close up](Wiring%20-%20Level-Shifter%20(3V%20to%205V)%20Adafruit%2074AHCT125%20close%20up.JPG) |
|-|-|-|
| Full View | Adafruit Feather Huzzah ESP8266 close up | Level-Shifter (3V to 5V) Adafruit 74AHCT125 close up |

## Program

I originally tried connecting the ESP8266 via Socket.io to my existing server, however, I had an issue compiling with the [socket.io-client](https://github.com/timum-viw/socket.io-client) library. I gave up and decided to use Firebase ([Firebase-ESP8266](https://github.com/mobizt/Firebase-ESP8266)) to communicate with the ESP8266 in real-time.


### Firebase Settings
Create a Realtime Database within a new Firebase Project. Because the program uses a service account to authenticate, we can set our Realtime Database's rules to Locked Mode. Once the project has been created, manually enter the following data:

``` JSON
rgbw
 |-- r: 255
 |-- g: 0
 |-- b: 0
 |-- w: 0
```

Then, grab your database URL and update the `FIREBASE_HOST` value in [Desk_Lights.ino](Desk_Lights/Desk_Lights.ino).
Go to your Firebase project's Users and Permissions settings **>** Service accounts **>** Database secrets, to grab your database's auth secret. Update the `FIREBASE_AUTH` in [Desk_Lights.ino](Desk_Lights/Desk_Lights.ino) with this value.

And thats pretty much all we need to do with Firebase! If it works, the LEDs should turn red.

### Wifi Settings

This is pretty self explanatory, update your `WIFI_SSID` and `WIFI_PASSWORD` in [Desk_Lights.ino](Desk_Lights/Desk_Lights.ino).

### NeoPixel Settings

If you changed the pin connected to the data link of the NeoPixels, you will need to change the `pin` integer in [Desk_Lights.ino](Desk_Lights/Desk_Lights.ino). Similarly, if the number of LEDs on the strip of NeoPixels is not 60, that will need to be updated (`numLeds`).

## Additional Resources

- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
- https://cdn-shop.adafruit.com/product-files/1787/1787AHC125.pdf
- https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/power-management
- https://cdn-learn.adafruit.com/assets/assets/000/046/249/original/adafruit_products_Huzzah_ESP8266_Pinout_v1.2-1.png?1504885873
- https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation
- https://learn.adafruit.com/neopixels-on-raspberry-pi/raspberry-pi-wiring
- https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html
- https://medium.com/@vibrologic/serverless-iots-with-firebase-realtime-database-and-esp8266-e624304c3197
- https://github.com/mobizt/Firebase-ESP8266

--------------

There are so many ways to expand this project. For example:

- Create a website with a color picker that updates the values in Firebase. Now anyone on the internet can change the colors of the LEDS!
- Instead of updating the color of all the LEDs at once, you an trickle the new color down the line.
- Implement an on/off switch (either physical or virtual). You could run a webserver on the ESP8266 with a website that allows anyone on the network to turn it on/off. Or, simpily store a boolean for on/off on Firebase.
