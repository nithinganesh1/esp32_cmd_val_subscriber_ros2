# ESP32 micro-ROS LED Control with Docker

This project demonstrates how to control an LED on an **ESP32** using **ROS 2** and **micro-ROS**, with the help of a **Dockerized micro-ROS agent**. The ESP32 listens to messages published from ROS 2 and changes the LED state accordingly.

---

## How It Works

### 1. ESP32 as Subscriber

* The ESP32 connects to the Wi-Fi network.
* It subscribes to the ROS 2 topic `/cmd_vel` through micro-ROS.
* The received messages contain `linear.x` and `angular.y` values.
* The ESP32 uses `linear.x` to control the LED:

  * **`linear.x > 0` → LED ON\*\*
  * **`linear.x = 0` → LED OFF\*\*

### 2. micro-ROS Agent in Docker

The ESP32 cannot directly talk to ROS 2, so it communicates with the **micro-ROS agent**.

Run the following commands:

```bash
docker pull microros/micro-ros-agent:humble

docker run -it --rm --name micro-ros-agent --net=host \
  microros/micro-ros-agent:humble udp4 --port 8888
```

* **`--net=host`**: allows ESP32 to directly access the host network.
* **`udp4 --port 8888`**: lightweight communication method over Wi-Fi.

### 3. ROS 2 Publisher Node

On the computer side, a ROS 2 node publishes messages to `/cmd_vel`.

Example command to run a Python publisher:

```bash
ros2 run your_package cmd_vel_publisher
```

This node sends `linear.x` and `angular.y` values. The ESP32 only uses `linear.x` for LED control.

### 4. Communication Flow

**User Input → ROS 2 Publisher → /cmd\_vel → Docker micro-ROS Agent → ESP32 → LED Action**

---

## Why Docker and micro-ROS?

* **Docker**: Simplifies environment setup, ensuring all dependencies are packaged inside a container.
* **micro-ROS**: Extends ROS 2 capabilities to microcontrollers like the ESP32.
* **UDP Transport**: Lightweight and efficient for Wi-Fi communication between ESP32 and the agent.

---

## Troubleshooting

* Ensure both ESP32 and the host computer are connected to the same Wi-Fi network.
* Always start the micro-ROS agent before publishing commands.
* If the agent does not start, check if port **8888** is already in use.
* Allow UDP port **8888** through your system firewall for communication:

```bash
sudo firewall-cmd --add-port=8888/udp --permanent
sudo firewall-cmd --reload
```

---

## Summary

This setup enables **real-time LED control on the ESP32** from ROS 2 commands. The ESP32 acts as a subscriber, the micro-ROS agent bridges communication via Docker, and a ROS 2 publisher node sends the commands. This demonstrates how embedded devices can be integrated into the ROS 2 ecosystem efficiently.
