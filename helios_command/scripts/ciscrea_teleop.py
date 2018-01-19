#!/usr/bin/env python
# coding=utf-8

# Author : Yacine

import rospy
from helios_command.msg import PwmCmd

# Turbines:
# Vertical_droit : pin=0, min_pwm=920, max_pwm=
# Vertical_gauche: pin=1, min_pwm=, max_pwm=
# Avant_droit    : pin=2, min_pwm=, max_pwm=
# Avant_gauche   : pin=3, min_pwm=, max_pwm=
# Arrière_droit  : pin=4, min_pwm=, max_pwm=
# Arrière_gauche : pin=5, min_pwm=, max_pwm=

pwm_cmd = PwmCmd()
# Initialised at min value for every pin
last_pin0_cmd = 1000
last_pin1_cmd = 1000
last_pin2_cmd = 1000
last_pin3_cmd = 1000
last_pin4_cmd = 1000
last_pin5_cmd = 1000

def teleop_callback(msg):
    rospy.loginfo("Twist msg reception")
    if msg.angular.z < 0:  # z -> rudder
        pwm_cmd.pin = 0
        pwm_cmd.command = last_rudder_cmd - 10
        last_rudder_cmd = pwm_cmd.command
    elif msg.angular.z > 0:
        pwm_cmd.pin = 0
        pwm_cmd.command = last_rudder_cmd + 10
        last_rudder_cmd = pwm_cmd.command
    elif msg.angular.x < 0: # x - > turbine
        pwm_cmd.pin = 1
        pwm_cmd.command = last_turbine_cmd - 5
        last_turbine_cmd = pwm_cmd.command
    elif msg.angular.x > 0:
        pwm_cmd.pin = 1
        pwm_cmd.command = last_turbine_cmd + 5
        last_turbine_cmd = pwm_cmd.command
    else:    # c'est moche mais au cas où ...
        pass
        
    command_pub.publish(command)
    rospy.loginfo("Last turbine command: %f" % last_turbine_cmd)
    rospy.loginfo("Last rudder command: %f" % last_rudder_cmd)    

if __name__ == '__main__':
    
    rospy.init_node('ciscrea_teleop')
    
    rospy.loginfo("ciscrea_teleop Node Initialised")
    
    # Subscribe to default_name of ROS keyteleop
    rospy.Subscriber("/key_vel", Twist, teleop_callback)
    
    command_pub = rospy.Publisher('pwm_cmd', PwmCmd)
    
    




