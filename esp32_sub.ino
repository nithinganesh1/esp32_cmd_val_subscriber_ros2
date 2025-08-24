#include <micro_ros_arduino.h>
#include <WiFi.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// -------- Wi-Fi Config ----------
char ssid_c[] = "Evolve Robotics JioFiber";
char password_c[] = "Evolve@123";

// -------- Agent Info ----------
char agent_ip_c[] = "192.168.29.34";
uint32_t agent_port = 8888;

// -------- LED Pin ----------
const int LED_PIN = 2;

// -------- micro-ROS objects ----------
rcl_subscription_t subscriber;
rclc_executor_t executor;
rcl_node_t node;
rcl_allocator_t allocator;
rclc_support_t support;
geometry_msgs__msg__Twist msg;

// -------- Callback ----------
void cmd_vel_callback(const void * msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;

  Serial.print("Received linear.x: ");
  Serial.println(msg->linear.x);

  if(msg->linear.x > 0.0) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting ESP32 micro-ROS LED subscriber...");

  pinMode(LED_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid_c, password_c);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  // Set micro-ROS Wi-Fi transport
  set_microros_wifi_transports(ssid_c, password_c, agent_ip_c, agent_port);

  // micro-ROS support init
  allocator = rcl_get_default_allocator();
  rclc_support_init(&support, 0, NULL, &allocator);

  // Create node
  rclc_node_init_default(&node, "esp32_v1_led", "", &support);

  // Subscriber to /cmd_vel
  rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"
  );

  // Executor
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  rclc_executor_add_subscription(&executor, &subscriber, &msg, &cmd_vel_callback, ON_NEW_DATA);

  Serial.println("✅ micro-ROS LED subscriber ready!");
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  delay(50);
}
