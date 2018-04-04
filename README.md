# BT_Demo
This project was created to Demonstrate an interface between a Microcontroller and a PC using a BlueTooth/Serial connection.

The goal is to explore the MVC (Model-View-Controller) Pattern for implementing an interface to a small microcontroller.

The MVC pattern, in a nutshell: The MODEL represents the data, and does nothing else. The MODEL does NOT depend on the CONTROLLER or the VIEW. The VIEW displays the MODEL data, and sends user actions (e.g. button clicks) to the CONTROLLER.

It has been my practive to provide a set of Configuration Data, and a set of Status Data.
In the MVC pattern, the CONTROLLER manipulates the Configuration Data. In the MVC pattern, the MODEL updates the Status Data.

Objective 1 The first objective is to develop the data exchange mechanism. Data will be exchanged bi-directionally via a serial connection. The data payload will be a large amount of binary data.

-Mark
