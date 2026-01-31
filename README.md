# Colour Sorting System (CSS) – Arduino Project

This project is an Arduino-based Colour Sorting System designed to detect the colour of tokens and automatically sort them using a servo-driven mechanism. The system uses a colour sensor to read RGB values, classifies the token colour, displays results on an OLED screen, and physically separates accepted and rejected tokens.


## Project Overview

The system simulates an industrial colour-sorting application used in quality control. Tokens of different colours are presented to a colour sensor. Based on the detected colour, the system decides whether to accept or reject the token and moves it into the appropriate container using a servo motor.

The OLED display provides real-time feedback, including RGB readings, detected colour, and counters for accepted and rejected tokens.


## System Features

- Detects token colour using a TCS230 colour sensor  
- Displays RGB values and detected colour on a 0.96” OLED display  
- Uses a servo-driven sweeper mechanism to sort tokens  
- Separates a chosen colour (Green) from all other colours  
- Tracks and displays number of accepted and rejected tokens  
- Provides audio feedback using a buzzer  
- Processes a set number of tokens automatically  


## How the System Works

1. The user presses a button to start the sorting process.  
2. A token is placed in front of the colour sensor.  
3. The sensor outputs pulse widths for red, green, and blue filters.  
4. The Arduino maps these readings to a 0–255 RGB scale using calibrated values.  
5. A rule-based classifier determines the detected colour.  
6. The servo rotates to move the token to the **accepted** or **rejected** side.  
7. The OLED display updates with RGB values, detected colour, and token counts.  

---

## Sorting Logic

The colour classification is based on calibrated threshold values for RGB readings.

- Green tokens are marked as **accepted**  
- All other detected colours are **rejected**  
- Black, white, or unclear readings are filtered using threshold checks  

This rule-based approach was chosen for:
- Simplicity  
- Real-time performance  
- Reliability on a microcontroller platform  


## Hardware Used

- Arduino microcontroller  
- TCS230 Colour Sensor Module  
- 0.96” I2C OLED Display  
- SG90 Servo Motor  
- Buzzer  
- Push Button  
- LEDs for status indication  


## Mechanical Mechanism

A servo motor drives a sweeper arm attached to the servo horn.  
Depending on the detected colour, the servo rotates to one of two positions:

- **Accept Position** → Token moved into the accepted container  
- **Reject Position** → Token moved into the rejected container  

The structure supports:
- Sensor mounting  
- Display mounting  
- Wiring management  
- Servo sorting mechanism  


## Design Considerations

- The colour sensor is sensitive to ambient light, so the sensing area is kept shaded.  
- Tokens are placed consistently in the same position for reliable readings.  
- Calibration values were tuned experimentally under fixed lighting conditions.  
- Servo angles were adjusted to ensure consistent sorting.  

## Future Improvements

- Add a menu system to select which colour to accept  
- Implement colour averaging or filtering for higher accuracy  
- Add an automatic token feeding mechanism  
- Improve enclosure design to further reduce ambient light interference  
- Expand sorting to include additional colours  

## What I Learned

- Interfacing sensors with microcontrollers  
- Calibration of real-world sensor data  
- Integrating hardware and software in embedded systems  
- Designing modular, readable Arduino code  
- Developing simple but effective decision-making logic
  ## Project Documentation

A full presentation covering the system design, PCB schematic, CAD models, block diagrams, and system demonstration is available below:

[View Full Project Presentation (PDF)](documentation/CSS ppt.pdf)


