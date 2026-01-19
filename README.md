# NextStep Neuro – EEG–EMG Controlled Wheelchair
NextStep Neuro is an assistive mobility project that enables a wheelchair to be controlled using EEG (brainwave) and EMG (muscle) signals. The system is designed to support individuals with paralysis by providing hands-free, wireless control using a custom Brain–Computer Interface (BCI).
The project focuses on affordability, safety, and scalability, using non-invasive biosignal sensing and Bluetooth Low Energy (BLE) communication.
## Project Overview
The system captures EEG and EMG signals through electrodes placed on the scalp and muscles. These signals are processed by a biopotential sensing board and transmitted wirelessly via BLE to an ESP32-based receiver. The ESP32 interprets the data and controls wheelchair motors using a motor driver.
## Key Features
- EEG-based intentional control (focus/mental state)
- EMG-based muscle confirmation and steering
- Wireless BLE communication
- ESP32-based motor control
- Modular and low-cost design
- Scalable toward AI-based signal classification
## System Architecture
EEG / EMG Electrodes  
→ Biopotential Sensing Board (BCI)  
→ Bluetooth Low Energy (BLE)  
→ ESP32 Receiver  
→ Motor Driver  
→ Wheelchair Motors
## Hardware Components
### Biopotential Sensing
- Biopotential sensing board (EEG + EMG)
- Electrodes and snap cables
- Skin preparation materials (Nuprep, alcohol wip
### Wheelchair Control
- ESP32 microcontroller (BLE-enabled)
- Motor driver module (MX1508 / MX1616)
- DC motors / wheelchair motors
- LiPo battery and power switch
## Repository Purpose
This repository documents:
- Hardware design and connections
- Signal flow and control logic
- Firmware for EEG/EMG processing and motor control
- Journal logs tracking project development
- Future improvements and testing results
## Safety Note
This is a prototype research project and is **not a medical device**. All testing must include safety cutoffs, supervised operation, and manual override mechanisms.
## Future Work
- AI-based EEG/EMG classification
- Improved noise filtering
- Emergency stop logic
- Real wheelchair integration and testing
- Mobile app for calibration and monitoring
