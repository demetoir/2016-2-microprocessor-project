false = False
true = True

import serial
import time
import sys
import traceback
import datetime
import random 

from time import gmtime, strftime

rootPwd = "1234"

class arm_m3_kit_server :
	MSG_ENCONDDING_TYPE = "ascii"
	MAX_MSG_LEN = 100
	connectionMsgList = ["end\n",
					"send_handshake",
					"receive_handshake",
					"connected_confirm",
					"send",
					"receive",
					"disconnect",
					"success"]
	SLEEP_TIME = 0.1
	CHECK_CONNECTION_INTERVAL = 50
	HAND_SHAKE_TIME_LIMIT = 20
	lockerDB = {}

	def setLockerDB(self, lockerDB):
		self.lockerDB = lockerDB

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
	
	def sendMsg(self, msg, showEcho=false):
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

	def makeHandShake(self, showLog=false, timeOut=HAND_SHAKE_TIME_LIMIT):
		curtime = time.clock
		startTime = curtime()
		handShakeMsg = "send_handshake end\n"
		if showLog :
			print("server send : send handshake msg")
		self.sendMsg(handShakeMsg)
		
		msg = "" 
		while 1:	
			self.delay()
			rcvMsg = self.receiveMsg()
			if curtime() - startTime > 1:
				self.sendMsg(handShakeMsg)
				break

			msg += rcvMsg
			if showLog:
				print("server : receive buffer :" + msg) 

			msgTokenList = msg.split()
			if len(msgTokenList) <2 or msgTokenList[-1] != "end\\n":
				continue

			if msgTokenList[0] == "receive_handshake" and msgTokenList[1] == 'end\\n': 	
				msg = "receive_handshake end\n"

				if showLog:
					print("server receive : <%s> "%(msg))

				return true
			
		return false

	def disconnectToArmKit(self, showLog=false) :
		curtime = time.clock
		starttime = curtime()	
		dcsMsg = "disconnect end\n"
		dcrMsg = "disconnect success end\\n"

		self.sendMsg(dcsMsg)
		if showLog:
			print("server send : ", dcsMsg)
			print("server hear : ")
		
		msg = ""
		while true:
			if curtime() - starttime > 3: 
				if showLog :
					print("server : disconnect time out")
					print("server : resend disconnect msg")
				self.sendMsg(dcsMsg)
			self.delay()
			s = self.receiveMsg()
			if s == "": continue
			msg += s
			if showLog:
				print("buffer :",msg)
			if  dcrMsg in msg:
				break
			if len(msg) > self.MAX_MSG_LEN :
				if showLog :
					print("server : disconnection error")
					print("server : resend disconnect msg")
				self.sendMsg(dcsMsg)
				
		return true
	
	def sendSynTimeMsg(self, showLog = false):
		isOk = true

		if showLog :
			print("current time : %s"%(datetime.datetime.now()))
		now = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
		msg = "Syc_time %s end\n"%(now)
		self.sendMsg(msg)
		if showLog:
			print ("server  send : %s"%(bytes(msg, encoding = self.MSG_ENCONDDING_TYPE)))
		return isOk

	def CheckConnection(self, showLog = false):
		self.sendMsg("check_connection end\n")
		msg = ""	
		startTime = time.time();
		timeLimit = 5
		rcvmsg = "check_connection end\\n"
		while 1:
			if time.time() - startTime > timeLimit:
				if showLog:
					print("check connection time out")
				return false
			msg += self.receiveMsg()
			if showLog :
				print (type(msg), type(rcvmsg))
				print(msg == rcvmsg)
				print("received  :",msg)
				print("target    :", rcvmsg)
			if rcvmsg in msg :
				return true
		return false
		
	def isPemissionMsg(self, msg, showLog= false):
		tokenList = msg.split()

		if len(tokenList) <4 :
			 return false

		if tokenList[0] == "request_permission" and tokenList[3] == 'end\\n':
			return true

		return false
	
	def responsePermissionMsg(self, msg, showLog = false):
		tokenList = msg.split()
		pwd = tokenList[1]
		lockerNum = tokenList[2]
		if showLog:
			print("server recieved : pwd %s , lockNum : %s"%(pwd,lockNum))
		if lockerNum in self.lockerDB and self.lockerDB[lockerNum]["lockerPassword"] == pwd:
			self.lockerDB[lockerNum]["lockerPassword"] = "%04d"%(random.randint(0,9999))
			result = true
		else :
			result = false

		if result == true:
			msg ="reseponse_permission %s end\n"%("yes")	
		else:
			msg ="reseponse_permission %s end\n"%("no")	

		if showLog:
			print("server send : %s"%( msg))
		self.sendMsg(msg)

	def printTimeLog(self, msg, showLog = true):
		if showLog:
			print("%s arm server : %s"%( str(datetime.datetime.now()), msg ))	

	def connectToArmKit(self, showLowg = false):
		self.disconnectToArmKit()
		self.printTimeLog("disconnection success")

		self.printTimeLog("start hand shake")
		while(1):
			if self.makeHandShake() == true:
				break
			self.printTimeLog("retry hand shake")	
		self.printTimeLog("hand shake success")

		self.sendSynTimeMsg()
		self.printTimeLog("sync Time complete")
		return

	def main(self, _lockerDB = {}):
		self.setLockerDB(_lockerDB)
		self.connectSerial(self.serialConnectionSetting)
		if self.serialConnectionObject == None:
			self.printTimeLog("can not make serial connection")
			exit()

		self.connectToArmKit()
	
		checkConnectionTime = self.CHECK_CONNECTION_INTERVAL
		msg = ""
		while true:
			self.delay()

			checkConnectionTime -= 1
			if checkConnectionTime <= 0:
				checkConnectionTime = self.CHECK_CONNECTION_INTERVAL
				isOk = self.CheckConnection()
				if isOk:
					self.printTimeLog("check ok")
				else :
					self.connectToArmKit()

			msg += self.receiveMsg()
			if msg != "" :
				tokenList = msg.split()
				if tokenList[-1] != 'end\\n':
					continue;

				self.printTimeLog("received : %s"%(msg))		
				if self.isPemissionMsg(msg) != false:
					self.responsePermissionMsg(msg)

				msg = ""
				continue					

		self.disconnectToArmKit()
		

	
			

# start
if __name__ == "__main__":
	arm_m3_kit_server().main()




