LeopardGeckoTerrarium: Automated Enclosure for Leopard Geckos (Prototype)
This project was developed by Juan Carlos Garcia Jimenez, Josué Francisco Castañeda, Rogelio Hernández Vázquez, and Alejandra Guzmán Jiménez for the First International Congress of Intelligent Systems for the Environment (CISIMA-IPN). 

Overview:

The LeopardGeckoTerrarium is a prototype of an intelligent, automated terrarium designed to monitor and control environmental factors for leopard geckos. The system integrates sensors and actuators to create an optimal habitat, providing a controlled environment for the animal.

Key Features:

  -Temperature Monitoring and Control: The system continuously monitors the temperature within the terrarium and allows for adjustment of heating elements to maintain the required temperature range for the gecko.
 
  -Humidity Monitoring: Humidity levels are also monitored to ensure they are within the suitable range for a healthy living environment.

  -LED Lamp Control via Pulse Width Modulation (PWM): The LED lamp is manually controlled using PWM, allowing the intensity of the light to be adjusted to simulate day-night cycles or replicate specific lighting conditions important for the gecko's well-being. However, this adjustment must be made manually and is not automatically dynamic.
 
  -Real-time(1) Monitoring and Control via Flask Server(2): Temperature and humidity data are displayed on a website hosted by a Flask server.

(1) Real-time is treated as an approximation, since it cannot be precisely defined or perfectly implemented.
(2) A tunnel service like Ngrok is required to make the local Flask server accessible remotely over the internet, allowing for real-time monitoring and adjustments.

Prototype Status:

As this is a prototype, the system is still under development, and it is not recommended for use with any living being at this stage. Several features may need further optimization for reliability and safety before it can be safely applied to the care of animals.

Technological Components:

  -ESP32 or Arduino board (if using an Arduino board, refer to the pinout and make any necessary adjustments).
  
  -DH11 and DS18B20 sensors.
  
  -Relay to activate and deactivate the actuator.
  
  -An LM324 is necessary to isolate the ESP32 or Arduino from the high-power phase and prevent damage to the microcontroller.
  
  -Actuators.
  
  -Flask Server.
  
  -Ngrok: Required for tunneling and exposing the local Flask server to the internet for remote access.
  
Disclaimer:

The LeopardGeckoTerrarium is currently a prototype and is not recommended for use with any living being. The system has been designed for experimental purposes and may not meet the necessary safety standards for animal welfare. As such, the team (Juan Carlos Garcia Jimenez, Josué Francisco Castañeda, Rogelio Hernández Vázquez, and Alejandra Guzmán Jiménez) is not responsible for any harm or damage resulting from the use of this prototype, including but not limited to injury or death of animals, environmental damage, or equipment malfunction.

Users are advised to proceed with caution, fully understanding the risks involved, and ensure that all components are tested and monitored thoroughly before considering any practical application.
