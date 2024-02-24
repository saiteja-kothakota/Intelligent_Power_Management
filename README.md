### Intelligent_Power_Management

Envision a day without electricity! Even a day in our life is hard to imagine without it. But are we concerned about it? Let’s look into it. Many of us leave the mobile, laptop, or any other similar device for an excess amount of time to fully charge the device. But this results in loss of battery life and electricity wastage. In the long-term, it costs you even considerable amount of money. Efficient Energy Management plays a vital role in the improvement of power consumption and power measurement. To fulfil this problem, we have developed an intelligent smart socket. The system uses the IoT Power Socket as a bridge to control the power outlet over the Internet. With the integration of Blynk cloud technology, the user can use the customized Blynk project to send commands to switch off or on the IoT Power socket. It also comes with a timer  feature. With the availability of the timer feature, the device can allow the power at a specified time intervals set by the user at any point of a day. The user is also provided with the control to stop the power supply before the end time. The advantage of this model is that it allows the power supply to the device (Mobile / Laptop) up to a threshold value specified by the user.


The lists of the components used to build the model are listed below:
• ESP32
• Breadboard
• 5VRelay
• Jumper wires
• Socket

# Hardware Requirements
S.NO Product Function
1. Esp32 Micro controller used to connect to Wi-Fi and perform basic computation.
2. Relay 5V relay module used to control current to modules.
3. AC Plug Receives AC power supply from the main source.
4. Socket Transmits AC power supply to the device.
5. Jumper Wires These are used to connect components in the model.

The major component of the design is a Esp32 microcontroller. It connects to Wi-Fi and is capable of performing calculations. The ESP32 Board runs on a voltage range of 2.2 to 3.6 volts. However, we provide 5V via the Micro-USB connector. There is already an LDO voltage regulator for 3.3V to keep the voltage constant. The ESP32 is powered by a Micro USB connector and a VIN pin (External Supply Pin). ESP32 requires 600mA of electricity since it draws up to 250mA during RF broadcasts. It draws more than 200mA current during boot or Wi-Fi operation. As a result, when we add several sensors or modules to the ESP32 Board, the power supplied by the Micro-USB Cable is insufficient. This is due to the fact that the USB port on a computer can only produce 500mA of current. Since there are no sensors in this project, powering the circuit with a USB connection is sufficient. Relays open and close connections in another circuit to control one electrical circuit. At the top of the relay, the maximum operating current and voltage for both AC and DC loads are also listed. A 5V active high or low signal, in other words, activates the relay by energising its coil. It has three terminals: NC, NO, and COM. It receives signals from the esp32 to turn the power
supply on or off. 


#Software Requirements

S.NO Product Function
1.. Arduino IDE It is used to program the esp32.
2. Blynk App It helps user to interact with the model.


Arduino IDE allows developers to programme microcontrollers based on their intended use. Blynk offers a wide range of options for managing and monitoring IoT devices. Create a project for the model by downloading a mobile app to your phone. Use the Blynk app to access the available widgets. Set the virtual pins and remember to keep track of them in the code.

#Flow Diagram
![image](https://github.com/saiteja-kothakota/Intelligent_Power_Management/assets/68795720/ddd7f062-da87-41e0-bb38-fd760dede950)

The above flow chart gives us the sequence of actions done. Client sends the battery and charging status to the esp32. Blynk communicates with esp32 to send and receive requests. If the Blynk app is not modified or accessed and the battery percentage of the device reaches to 100 percentage then socket is turned OFF else device is in charging state. If the threshold value is updates and if current battery percentage is less than the threshold limit then socket is turned OFF, else the device will be continued in charging state. When the current time matches with the start time then socket is turned ON and when the current time matches with the end time then socket is turned OFF. If you want to set the device in charging state access the button to turn it ON and vice-versa to disable charging. 
