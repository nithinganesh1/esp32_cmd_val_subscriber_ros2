# ESP32 micro-ROS LED Control with Docker

This project demonstrates a micro-ROS setup using an ESP32 as a subscriber to control an LED based on user input commands published from a ROS2 node. The project uses Docker for micro-ROS agent setup and ROS2 for communication.

## Features

* ESP32 connects to a Wi-Fi network and subscribes to a ROS2 topic.
* LED on the ESP32 turns ON/OFF based on the received `linear.x` value.
* User can send `linear.x` and `angular.z` commands through a Python ROS2 publisher node.
* Full setup is containerized using Docker for easy ROS2 and micro-ROS agent management.

## Components Used

* ESP32 Dev Kit V1
* LED connected to ESP32 GPIO
* Docker for containerized micro-ROS agent
* ROS2 Humble (inside Docker)
* Arduino IDE for ESP32 programming

## Setup Steps

### ESP32 Subscriber Code

1. Connect the ESP32 to your computer.
2. Install Arduino IDE and required libraries:

   * `WiFi.h`
   * `micro_ros_arduino-kilted`
3. Upload the subscriber sketch to the ESP32:

   * Connect to Wi-Fi
   * Subscribe to `/cmd_vel` topic
   * Control the LED based on received `linear.x` value

### Docker micro-ROS Agent

1. Pull the micro-ROS agent Docker image:

   ```bash
   docker pull microros/micro-ros-agent:humble
   ```
2. Run the agent with UDP4 transport:

   ```bash
   docker run -it --rm --name micro-ros-agent --net=host microros/micro-ros-agent:humble udp4 --port 8888
   ```
3. Ensure it shows `running...` indicating the agent is ready.

### ROS2 Publisher Node

1. Create a Python ROS2 node that takes user input for `linear.x` and `angular.z`.
2. Publish these values to the `/cmd_vel` topic.
3. Example:

   ```bash
   ros2 run your_package cmd_vel_publisher
   ```
4. Input values like `linear.x=4`, `angular.z=0` and observe LED behavior on ESP32.

## Problems Faced and Solutions

* **Issue:** Serial monitor showed garbage characters.

  * **Solution:** Corrected board selection and baud rate in Arduino IDE.
* **Issue:** micro-ROS agent package not found in ROS2 Humble.

  * **Solution:** Used Docker container specifically for micro-ROS agent.
* **Issue:** ROS2 topic publishing syntax errors.

  * **Solution:** Corrected message format using `geometry_msgs.msg.Vector3` structure.
* **Issue:** Port binding error when running micro-ROS agent.

  * **Solution:** Stopped any existing agent containers and ensured the port was free before running a new container.

## How it Works

1. The ESP32 connects to the specified Wi-Fi network.
2. The micro-ROS agent runs in Docker and handles communication between ROS2 and ESP32.
3. User publishes commands via the ROS2 Python node.
4. ESP32 receives commands and toggles the LED accordingly.

## Notes

* Ensure the ESP32 and host computer are on the same Wi-Fi network.
* Always check that the micro-ROS agent is running before publishing commands.
* Use the correct ROS2 message format when publishing to avoid errors.

---

This setup allows testing of real-time control of ESP32 peripherals using ROS2 and micro-ROS, using Docker to simplify dependencies and environment setup.
