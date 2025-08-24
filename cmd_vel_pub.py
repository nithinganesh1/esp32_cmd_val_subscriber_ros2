
#!/usr/bin/python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class CmdVelPublisher(Node):
    def __init__(self):
        super().__init__('cmd_vel_publisher')
        self.publisher_ = self.create_publisher(Twist, 'cmd_vel', 10)
    
    def send_cmd(self, linear_x, angular_z):
        msg = Twist()
        msg.linear.x = linear_x
        msg.angular.z = angular_z
        self.publisher_.publish(msg)
        self.get_logger().info(f'Published: linear_x={linear_x}, angular_z={angular_z}')

def main(args=None):
    rclpy.init(args=args)
    node = CmdVelPublisher()

    try:
        while rclpy.ok():
            linear = float(input("Enter linear.x: "))
            angular = float(input("Enter angular.z: "))
            node.send_cmd(linear, angular)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
