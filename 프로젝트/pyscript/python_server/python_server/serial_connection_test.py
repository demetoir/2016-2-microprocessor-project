false = False
true = True

import serial
import time
import sys
import traceback
import datetime

class arm_m3_kit_server :
	MSG_ENCONDDING_TYPE = "ascii"
	MAX_MSG_LEN = 100
	receiveBuffer =""
	connectionMsgList = ["end\n",
					"send_handshake",
					"receive_handshake",
					"connected_confirm",
					"send",
					"receive",
					"disconnect",
					"success"]
	SLEEP_TIME = 0.05
		
	def delay(self):
		time.sleep(self.SLEEP_TIME)

	class SerialConnectionSetting :
		port = 'COM5'
		baudrate = 115200
		parity = 'N'
		stopbits = 1
		bytesize = 8
		timeout = 0

		def __init__(self, port='COM5', baudrate=115200, parity='N', stopbits=1, bytesize=8, timeout=0):
			self.port = port
			self.baudrate = baudrate
			self.parity = parity
			self.stopbits = stopbits
			self.bytesize = bytesize
			self.timeout = timeout			
			return 

	serialConnectionSetting = SerialConnectionSetting()

	serialConnectionObject = None

	def connectSerial(self, setting):
		try:
			self.serialConnectionObject = serial.Serial(port = setting.port,
										baudrate = setting.baudrate,
										parity = setting.parity,
										stopbits = setting.stopbits,
										bytesize = setting.bytesize,
										timeout = setting.timeout)
		except :
			print("fail to open serial port")
			traceback.print_exc(file = sys.stdout)
			return false
		
		print("success to open serial port")
		print("serial object : ", self.serialConnectionObject)
		print("serial setting : ", self.serialConnectionObject.portstr)
		return true

	def receiveMsg(self):
		return str(self.serialConnectionObject.readline())[2:-1]

	def sendMsg(self, msg, showEcho=true):
		for c in msg:
			bChar = bytes(c, encoding = self.MSG_ENCONDDING_TYPE)
			self.serialConnectionObject.write(bChar)
			echoChar = "" 
			while (1):
				echoChar = self.receiveMsg()
				if (echoChar != ''):
					break
			if (showEcho):
				print("send : %s  echo : %s" % (bChar, echoChar))
		return 

	def makeHandShake(self, showLog=true, timeOut=20):
		curtime = time.clock
		startTime = curtime()
		while 1:	
			self.delay()
			rcvMsg = self.receiveMsg()
			if curtime() - startTime > timeOut:
				break

			if rcvMsg == "":
				continue
			
			if showLog:
				print("server : receive : " + rcvMsg) 

			msgTokenList = rcvMsg.split()
			if msgTokenList[0] == "send_handshake" : 	
				msg = "receive_handshake %s end\n" % (msgTokenList[1])
				print("server : send : <%s> " % (msg))
				self.sendMsg(msg)
				return true

			
		return false

	def disconnectSession(self, showLog=true) :
		curtime = time.clock
		starttime = curtime()	
		dcsMsg = "disconnect end\n"
		dcrMsg = "disconnect success end\\n"

		if showLog:
			print("server send : ", dcsMsg)
		self.sendMsg(dcsMsg)

		if showLog:
			print("server hear : ")
		
		msg = ""
		while true:
			if curtime() - starttime > 1000: 
				print("server : disconnect time out")
				return false
			self.delay()
			s = self.receiveMsg()
			if s == "": continue
			msg += s
			print(msg, s, msg == dcrMsg)
			print(msg, type(msg))
			print(dcrMsg, type(dcrMsg))
			if  dcrMsg in msg:
				break
			if len(msg) > self.MAX_MSG_LEN :
				print("server : disconnection error")
				print("server : resend disconnect msg")
				self.sendMsg(dcsMsg)
		print("sever : discconnet succces")
		return true
	
	def main(self):
		self.connectSerial(self.serialConnectionSetting)
		if self.serialConnectionObject == None:
			print("can not make serial connection")
			exit()

		self.disconnectSession()
		
		print("server : start hand shake")
		while(1):
			if self.makeHandShake() == true:
				break
			print("hand Shake fail")
			print("retry hand shake")	
		print("server : hand shake success")

		curTime = time.clock
		startTime = curTime()
		while true:
			#if curTime() - startTime > 3 :
				#break
			
			#print(curTime())
			self.delay()
			msg = self.receiveMsg()
			if (msg == "") :
				continue
			print("%s server receive : %s"%(str(datetime.datetime.now()), msg))		
		
		self.disconnectSession()
		

	
			

# start
arm_m3_kit_server().main()




