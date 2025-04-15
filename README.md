# Sharing is Caring: hyper-redundancies for resilient robot collectives

**Authors:** Kevin Holdcroft, Anastasia Bolotnikova, Antoni Jubes Monforte, Jamie Paik - *Reconfigurable Robotics Lab, EPFL*

This repo contains the code relevant to controlling and communication of RRL's **Mori3** with local resource sharing.

Details for the implementation of the Mori3 can be found at the following repo:
https://github.com/chbelke/rrl-epfl-mori3

# Overview
 - MoriController.X: Main low-level controller
 - ESP8266: Control for ESP8266EX wifi chip
 - External: Overall external control
 - SensorFusionExperiment: Data generation for the sensor fusion experiment
 - TopologyCalculations: Algorithm for calculating the impact of local resource sharing, among all topologies for a given number of modules.


## Mori controller
The main controller of a module consists of a microcontroller driving all core functionalities that interfaces with an ESP8266 over Serial. The 16-bit microcontroller from Microchip, dsPIC33EP512GM604 ([datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/70000689d.pdf)), is programmed via ([MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)).

 ## ESP8266 WiFi
Firmware flashed via Arduino IDE. ESP8266 hardware must be installed: https://arduino-esp8266.readthedocs.io/en/latest/installing.html
The above includes the following libraries:
`ESP8266WiFi.h`
`ArduinoOTA.h`


 ## External:
Runs the GUI for operating the Mori3. 

Execute `python3 mainMori.py` located in the External folder

An MQTT broker is needed handled by a desktop running Ubuntu 20.04 or Raspberry Pi.

The broker should have a fixed local IP of *192.168.1.2*

## SensorFusionExperiment
The data and code for generating the graphs in the motion capture experiments, demonstrating sensor fusion.


## TopologyCalculations
The code used to assess the failure rates of different module topologies. 

`generate_save_graphs.py` creates the topologies / failure mode configurations. The topologies are saved into .pkl into the following folders:
 - `CirclePickles LinePickles TripodPickles DogPickles WheelPickles Pickles1 Pickles2 Pickles3`

You will need to create the folders in the same directory.

`plot_load_different_probs.py` generates the probabilities from the graphs

Both can be modified internally to generate the different topologies, with the functions generating the different topologies in the code.


## Licencing

If you use any of this code within an academic work, cite the following\*:

    @article{
        *INSERT BIBTEX HERE*
     }

\*Manuscript under review