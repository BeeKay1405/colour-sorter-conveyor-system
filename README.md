# Color Sorting Conveyor System

## Project Overview

An Arduino-based automated color sorting conveyor system that detects object color using a TCS3200 color sensor and directs objects into separate bins using servo-actuated gates. The system integrates real-time sensing, motor control, and LCD status feedback to perform autonomous object classification and sorting.

---

## Hardware Used

* Arduino Uno / Nano
* TCS3200 Color Sensor
* IR Sensors
* Servo Motor(s)
* DC Motor
* LCD Display
* Conveyor Assembly

---

## System Architecture

```text
IR Detection
     ↓
Stop Conveyor
     ↓
Read Color
     ↓
Classify Object
     ↓
Move Servo
     ↓
Resume Conveyor
```

---

## Features

* Automatic color classification
* Real-time object detection
* Servo-based sorting mechanism
* LCD status feedback
* Sensor calibration support
* Synchronized conveyor stopping for accurate measurements

---

## Results

### Demonstration

Add the following project artifacts:

* System photographs
* Wiring diagram
* Circuit schematic
* Demonstration video or GIF

A short demonstration video is highly recommended to showcase real-time operation and sorting accuracy.

---

## Technical Challenges

### Lighting Variations

Color sensor readings were affected by ambient lighting conditions, requiring calibration and threshold tuning to maintain classification accuracy.

### Conveyor Synchronization

The conveyor motor and servo mechanism had to be coordinated to ensure objects remained stationary during color measurement and sorting.

### Sampling Timing

An appropriate halt duration had to be determined before color acquisition to obtain stable sensor readings and prevent misclassification.

### Sensor Calibration

Color thresholds were experimentally calibrated to reliably distinguish between different object colors under repeated testing conditions.

---

## Future Improvements

* Increase the number of supported color classes
* Implement adaptive calibration for varying lighting environments
* Add object counting and sorting statistics
* Integrate wireless monitoring and control
* Improve classification accuracy using machine learning techniques

---

## Authors

* Akshat Kashyap
* Saarthak Singh
* Karan Kandpal
* Pranava Sworup
* Puttaraja Beedinalmath
* Anirudh Rao
* Tarun RS
