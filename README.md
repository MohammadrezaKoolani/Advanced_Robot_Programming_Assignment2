
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
The ARP 2nd Assignment Drone Simulator project is organized into several key directories and files:

### **1. `src/` (Source Code Directory)**
- `main.c`: The main program handling three modes (`publisher`, `subscriber`, and `display`) using command-line arguments.
- `blackboard.c`: Implements the shared blackboard for storing and synchronizing drone, obstacle, and target data.
- `dds_publisher.cpp`: Publishes obstacles and targets using Fast DDS.
- `dds_subscriber.cpp`: Subscribes to obstacle and target data using Fast DDS and updates the blackboard.
- `drone.c`: Implements drone movement, collision detection, and interaction with the blackboard.
- `ObstaclesPubSubTypes.cxx`, `TargetsPubSubTypes.cxx`: Auto-generated files from IDL definitions for Fast DDS serialization.
- `ObstaclesTypeObjectSupport.cxx`, `TargetsTypeObjectSupport.cxx`: Additional Fast DDS type support files.

### **2. `include/` (Header Files Directory)**
- `blackboard.h`: Defines the `Blackboard` structure and functions for shared memory management.
- `dds_publisher.h`: Declares the `DDSPublisher` class and its methods for Fast DDS communication.
- `dds_subscriber.h`: Declares the `DDSSubscriber` class and its methods for Fast DDS communication.
- `globals.h`: Declares global variables and constants used throughout the project.

### **3. `idl_files/` (Fast DDS IDL Files)**
- `Obstacles.idl`: Defines the data structure for obstacles in DDS.
- `Targets.idl`: Defines the data structure for targets in DDS.
*Note: DDS code is generated from these IDL files and manually moved to the `src/` directory for compilation.*

### **4. `bin/` (Compiled Executables)**
- `drone_simulator`: The main executable, which can be run in `publisher`, `subscriber`, or `display` mode using command-line arguments.

### **5. `log/` (Execution Logs and Debugging)**
- `position_log.txt`: Logs the drone’s position over time for analysis and debugging.

### **6. `Makefile`**
- Automates the compilation process, including:
  - Compiling all `.c`, `.cpp`, and `.cxx` source files.
  - Linking with the required Fast DDS libraries.
  - Providing `make`, `make clean`, and `make all` targets for building the project.

---


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

![DDS Communication Model](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/master/dds_model.png)


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

![DDS Communication Model](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/master/dds_layer_model.png)


---


### **Build Instructions**

#### **Steps to Build the Project:**

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2.git
   cd Advanced_Robot_Programming_Assignment2
   ```

2. **Ensure Fast DDS is Installed:**
   ```bash
   sudo apt update
   sudo apt install libfastcdr-dev libfastrtps-dev
   ```

3. **Clean Previous Builds (if any):**
   ```bash
   make clean
   ```

4. **Compile the Project:**
   ```bash
   make
   ```

5. **Running the Project:**
   Run the project in three separate terminals for each mode:

   - **Publisher Terminal:**
     ```bash
     ./drone_simulator publisher
     ```
   - **Subscriber Terminal:**
     ```bash
     ./drone_simulator subscriber
     ```
   - **Display Terminal:**
     ```bash
     ./drone_simulator display
     ```

   > *Make sure all three terminals are in the `Advanced_Robot_Programming_Assignment2` directory before running each command.*

---






## **System Architecture**
The project follows a **modular structure** similar to the first assignment, with additional DDS-based inter-computer communication. The key components are:
- **Drone Dynamics**: Manages drone movement with forces, inertia, and damping.
- **Fast DDS Communication**: Handles all inter-computer messaging.
- **Obstacle/Target Generators**: Spawn and remove obstacles dynamically.
- **Blackboard Server**: Stores the shared state of the environment.
- **Watchdog Process**: Monitors execution and ensures fault detection.
 in the diagram below you can see an example of Fast DDS cumunication between two systems.

![DDS Communication Example](https://github.com/MohammadrezaKoolani/Advanced_Robot_Programming_Assignment2/blob/master/dds_diagram1.png)


---

## **Future Improvements**
- Implement more advanced **collision avoidance** algorithms.
- Optimize **Fast DDS QoS settings** for improved network performance.
- Extend to multi-agent systems for **swarm robotics simulation**.

## **Conclusion**
This assignment successfully integrates **Fast DDS** to enable real-time distributed simulation. By leveraging a **publish/subscribe model**, the system allows seamless **data exchange** between multiple computers, enhancing the interactivity and scalability of the drone simulator.

