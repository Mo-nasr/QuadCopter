#!/usr/bin/env python
 
import sys, os
import rospy 
from decision_make.msg import *
from std_msgs.msg import String
import smbus
from multimaster_msgs_fkie.srv import DiscoverMasters,DiscoverMastersResponse
from multimaster_msgs_fkie.msg import MasterState,ROSMaster





class make_decision:
    def __init__(self):
        #initializing the attributes
        self.motor_1=0
        self.motor_2=0
        self.motor_3=0
        self.motor_4=0
        self.current_flag=0
        self.battry=100
        #initializing the node
        rospy.init_node('make_decision')
        self.diagnose_sub = rospy.Subscriber('current_battery', diagnose_msg, self.diagnose)
        self.matlab_sub = rospy.Subscriber('rpm', rpm_msg, self.assign)
        self.status_pub = rospy.Publisher('state',String,queue_size = 1)
        self.motors_pub = rospy.Publisher('motors',rpm_msg,queue_size = 1)
        # i2c init
        self.bus = smbus.SMBus(1)
        self.address = 0x40


    def diagnose(self,msg):
		rospy.wait_for_service('/master_discovery/list_masters')
		masters_service=rospy.ServiceProxy('/master_discovery/list_masters',DiscoverMasters)
		masters = masters_service().masters
		names=[]

		for i in masters:
			names.append(i.name)

		if 'badra-mrslab' not in names:
			self.status_pub.publish('WARNING:Connection Lost with the HeadUnit')
			self.emergency()

		if msg.current_1 > 16: ##the current limit
			self.current = 1
			self.status_pub.publish('WARNING: The current in motor 1 over 16 Amps ')
		if msg.current_2 > 16: #elif
			self.current = 1
			self.status_pub.publish('WARNING: The current in motor 2 over 16 Amps ')
		if msg.current_3 > 16: #elif
			self.current = 1
			self.status_pub.publish('WARNING: The current in motor 3 over 16 Amps ')
		if msg.current_4 > 16: #elif
			self.current = 1
			self.status_pub.publish('WARNING: The current in motor 4 over 16 Amps ')
		if msg.battery < 20: #elif
			self.battry=1
			self.status_pub.publish('WARNING: The battery is lower than 20 %')
		if self.current_flag == 0 and self.battry == 0:
			a='Every Thing is OK ENJOY :)','Battery : ',msg.battery
			self.status_pub.publish(a)
    #reset flags methods
    def clear_current(self):
        self.current = 0


    def clear_battery(self):
        self.battry = 0

    def assign(self,msg):
        if self.current == 0 and self.battry == 0:##add the master_disc condition
            self.motor_1=msg.motor_1
            self.motor_2=msg.motor_2
            self.motor_3=msg.motor_3
            self.motor_4=msg.motor_4


        else:
            self.emergency()

        self.motor_control()

    def emergency(self):
		#os.sys("rosrun control stabilization.py")
        self.motor_1 = 0
        self.motor_2 = 0
        self.motor_3 = 0
        self.motor_4 = 0


    def motor_control(self):
        rpm =rpm_msg()
        rpm.motor_1 = self.motor_1
        rpm.motor_2 = self.motor_2
        rpm.motor_3 = self.motor_3
        rpm.motor_4 = self.motor_4
        self.motors_pub.publish(rpm)
        self.bus.write_byte(self.address, msg.motor_1)
        self.bus.write_byte(self.address, msg.motor_2)
        self.bus.write_byte(self.address, msg.motor_3)
        self.bus.write_byte(self.address, msg.motor_4)
	#bus.write_i2c_block_data(self.address, self.motor_1, [self.motor_2, self.motor_3, self.motor_4])




if __name__ == '__main__':
	dec_node = make_decision()
	rospy.spin()
