#BT_Demo

##Getting Started
This project was created to Demonstrate an interface between a Microcontroller and a PC using a BlueTooth/Serial connection.

##Environment
###Hardware
Microchip PIC18F46K22 microcontroller

[HC-06 Bluetooth Module] <http://wiki.sunfounder.cc/index.php?title=Bluetooth_Transceiver_Module_HC-06>

The prototype system comprises the PIC processor mounted to a PICkit(tm) 44-Pin Demo Board, to provide a hardware platform with connector for the HC-06 BlueTooth module.  The Demo Doard also provides a connector for the PICkit(tm)3 debug tool.  Power to the system is provided via the PICkit3

###Tools
MPLABx IDE v4.15

Microchip XC8(v1.45) Compiler

##Goal 1
Develop components




##Follow-on work

### Data Exchange Implementation 
The objective is to develop the data exchange mechanism. Data will be exchanged bi-directionally via a serial connection. The data payload will be a large amount of binary data.

The goal is to explore the MVC (Model-View-Controller) Pattern for implementing an interface to a small microcontroller.

The MVC pattern, in a nutshell: The MODEL represents the data, and does nothing else. The MODEL does NOT depend on the CONTROLLER or the VIEW. The VIEW displays the MODEL data, and sends user actions (e.g. button clicks) to the CONTROLLER.

It has been my practive to provide a set of Configuration Data, and a set of Status Data.
In the MVC pattern, the CONTROLLER manipulates the Configuration Data. In the MVC pattern, the MODEL updates the Status Data.


-Mark