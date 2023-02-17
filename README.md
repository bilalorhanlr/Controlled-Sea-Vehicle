# Controlled-Sea-Vehicle

We are using LoRaWAN to establish communication between the remote control and the sea vehicle. On the remote control, we have used 2 switches, 1 screen, 1 receiver for the image that is displayed on the screen, 1 switch, and 1 potentiometer for the 3rd motor used to clean the debris. As stated in the "controller.c" file, we have defined the necessary functions and communication protocols for the remote control to send and receive messages from the sea vehicle.

We have IBT-2 motor drivers that can withstand up to 40 amps to drive the DC motors on the sea vehicle, and to prevent damaging the motors by moving quickly forward and backward, we implement gradual decrease and increase in our code, which can be seen in the 'boat.c' file

#### DC Motor Connection diagram

<img src="https://github.com/bilalorhanlr/Controlled-Sea-Vehicle/blob/main/Schematic.png">

#### IBT-2 Connection diagram

<img src="https://github.com/bilalorhanlr/Controlled-Sea-Vehicle/blob/main/IBT_2%20Schematic_2.png">

#### LoraWan Documentation

- [E220-400T22D](https://github.com/bilalorhanlr/Controlled-Sea-Vehicle/blob/main/E220-400T22D_UserManual_EN_v1.0.pdf)
