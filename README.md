
# Advanced Robot Programming - Assignment 2


## Contributors
- **Sarvenaz Ashoori - S6878764**
- **Mohammadreza Koolani - s5467640**

## Overview
This project is the **second assignment** for the Advanced Robot Programming course. It builds upon the **first assignment**, which involved a drone operation interactive simulator, by introducing a **distributed communication system** using **Fast DDS**.

📌 **First Assignment Repository:** [Advanced Robot Programming Assignment 1](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment1)

## Objectives
The second assignment focuses on **distributed drone simulation** across two computers in the same LAN. The project implements:

- A **Fast DDS-based** publish/subscribe system for inter-process communication.
- A **dual-instance architecture**, where two computers can either:
  - Control the drone while receiving obstacle and target data from another computer.
  - Generate obstacles and targets while sending data to the other system.
- Real-time **message exchange** for synchronization.
- **Logging, watchdog, and parameter files** as per the first assignment’s specifications.

## Project Structure
The project consists of several key directories and files:

### **1. `src/` (Source Code Directory)**
Contains the core implementation files:
- `drone_simulator.cpp`: The main program for controlling the drone.
- `obstacle_generator.cpp`: Generates dynamic obstacles using DDS.
- `target_generator.cpp`: Spawns new targets in the environment.
- `dds_communication.cpp`: Handles DDS communication for transmitting and receiving messages.

### **2. `include/` (Header Files Directory)**
Includes necessary headers:
- `drone_simulator.h`: Definitions and drone movement logic.
- `dds_communication.h`: Interface for DDS setup and communication.
- `obstacle_generator.h`: Functions for obstacle placement.
- `target_generator.h`: Target generation logic.

### **3. `dds_files/` (Fast DDS Configuration and IDL Files)**
- `Obstacles.idl`: Defines the DDS data structure for obstacles.
- `Targets.idl`: Defines the DDS data structure for targets.
- `dds_config.xml`: Configuration file for Fast DDS settings.

### **4. `bin/` (Compiled Executables)**
Contains compiled versions of the source files.

### **5. `log/` (Execution Logs and Debugging)**
Stores logs generated during execution, useful for debugging and performance analysis.

---

## Fast DDS: Explanation

### **What is Fast DDS?**
Fast DDS (formerly Fast RTPS) is an **open-source** implementation of the **Data Distribution Service (DDS)** standard, developed by eProsima. It allows **real-time, high-performance, and scalable** communication between processes running on different computers or networks.

#### **Why Use Fast DDS?**
- **Low latency and high throughput**: Ensures real-time data sharing.
- **Scalability**: Suitable for multiple nodes.
- **Flexibility**: Works across different OS platforms.
- **Decentralized**: No need for a centralized broker, reducing communication overhead.
- **Reliability**: Built-in QoS (Quality of Service) policies ensure message delivery.

#### **DDS Communication Model**
DDS follows a **publish/subscribe model** for efficient data exchange.
- **Publishers**: Send messages on a specific **topic**.
- **Subscribers**: Receive messages from a topic if subscribed.
- **Topics**: Ensure proper data binding between publishers and subscribers.
- **DomainParticipants**: Manage all entities within a DDS **domain**.

![DDS Communication Model](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/main/dds_model.png)


---

### **Fast DDS Implementation in This Project**
The communication between the two computers follows a **publish/subscribe model**:

1. **Data Publishers**:
   - One computer **publishes target and obstacle data** using DDS topics.
   - The drone's state updates (position, velocity) are also published.

2. **Data Subscribers**:
   - The second computer **subscribes** to the obstacle/target topic and updates the simulation accordingly.
   - Both computers exchange synchronization messages to ensure proper coordination.


---

### **Fast DDS Configuration**
- **Domain ID:** Used to separate multiple instances of DDS.
- **QoS (Quality of Service) Settings:** Ensure real-time updates with minimal delay.
- **Topic Structure:**
  - `/drone/state` (for position updates)
  - `/environment/targets` (for new targets to be reached)
  - `/environment/obstacles` (for obstacle locations)

![DDS Communication Model](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/main/dds_layer_model.png)


---

### **How to Install and Run the System**

#### **1. Install Fast DDS**
```sh
sudo apt install fastdds fastdds-tools
```

#### **2. Set Up the Network**
Ensure both computers are on the same LAN.

#### **3. Compile the Code**
```sh
mkdir build && cd build
cmake ..
make
```

#### **4. Start the First Instance (Drone Operator) on Computer A**
```sh
./bin/drone_simulator
```

#### **5. Start the Second Instance (Obstacle/Target Generator) on Computer B**
```sh
./bin/Targets publisher
./bin/Targets subscriber
or
./bin/Obstacle publishera
./bin/Obstacle subscriber
```


---

## **System Architecture**
The project follows a **modular structure** similar to the first assignment, with additional DDS-based inter-computer communication. The key components are:
- **Drone Dynamics**: Manages drone movement with forces, inertia, and damping.
- **Fast DDS Communication**: Handles all inter-computer messaging.
- **Obstacle/Target Generators**: Spawn and remove obstacles dynamically.
- **Blackboard Server**: Stores the shared state of the environment.
- **Watchdog Process**: Monitors execution and ensures fault detection.
 in the diagram below you can see an example of Fast DDS cumunication between two systems.

![DDS Communication Example](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/main/dds_diagram1.png)


---

## **Future Improvements**
- Implement more advanced **collision avoidance** algorithms.
- Optimize **Fast DDS QoS settings** for improved network performance.
- Extend to multi-agent systems for **swarm robotics simulation**.

## **Conclusion**
This assignment successfully integrates **Fast DDS** to enable real-time distributed simulation. By leveraging a **publish/subscribe model**, the system allows seamless **data exchange** between multiple computers, enhancing the interactivity and scalability of the drone simulator.

