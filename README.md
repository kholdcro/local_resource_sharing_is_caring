# local_resource_sharing_is_caring

**Authors:** Kevin Holdcroft, Anastasia Bolotnikova, Antoni Jubes Monforte, Jamie Paik - *Reconfigurable Robotics Lab, EPFL*

This repo contains all the code relevant to controlling and communication of RRL's **Mori3**, updated with algorithms for local resource sharing.

Details for the implementation of the Mori3 can be found at the following repo:
https://github.com/chbelke/rrl-epfl-mori3

# Overview
 - MoriController.X: Main low-level controller
 - ESP8266: Control for ESP8266EX wifi chip
 - External: Overall external control
 - SensorFusionExperiment: Data generation for the sensor fusion experiment
 - TopologyCalculations: Algorithm for calculating the impact of local resource sharing, among all topologies for a given number of modules.