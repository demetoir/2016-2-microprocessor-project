# python 3.5.1 64bit
import random
import sys
import os


CRC_set = { "crc-1" : [1,0],
			"crc-8" : [8,2,1,0],
			"crc-16" : [16,12,5,0],
			"crc-32" : [32,26,23,22,16,12,11,10,8,7,5,4,2,1,0]
			}

def makeCRC_value():
	for key in CRC_set:
		val = [0] * (CRC_set[key][0] + 1)
		for index in CRC_set[key]:
			val[index] = 1 
		print(key, "".join(map(str,val)))
	return


def makeCorruptBitArray(bitArray, p):
	return 


def strToBitArray(s):
	bitStream = ''.join(format(ord(x), 'b') for x in s)	

	bitArray = []

	for b in bitStream:
		bitValue = ord(b) - ord('0')
		bitArray += [bitValue]

	return bitArray

def cal_CRC_16(bitArray):
	def CRC_16_hardware(register, inputValue, size = 16):
		newRegister = [0]*size	
		for i in range(size):
			if i == 0:
				newRegister[0] = inputValue ^ register[size -1] 
			elif i == 2:
				newRegister[2] = register[1] ^ inputValue ^ register[size -1] 
			elif i == 15 :
				newRegister[15] = register[14] ^ inputValue ^ register[size - 1] 
			else:			
				newRegister[i] = register[i - 1]
		return newRegister
	
	newRegister = [0] * 16
	register = [0] * 16
	size = 16
	#input bit
	for inputValue in bitArray:
		newRegister = CRC_16_hardware(register, inputValue)
		register = newRegister[:]

	#extra bit for remainder
	for i in range(size-1):
		inputValue = 0
		newRegister = CRC_16_hardware(register, inputValue)
		register = newRegister[:]
		
	#conver bit to hex value
	crcHexValue = 0
	for value in register:
		crcHexValue = crcHexValue*2  + value  
	return crcHexValue




#G(X) = 110101
def cal_CRC_test(bitArray):

	def hardware(register, inputValue):
		newRegister = [0]*5
		for i in range(size):
			if i == 0:
				newRegister[0] = inputValue ^ register[4] 
			elif i == 2:
				newRegister[2] = register[4] ^ register[1]
			elif i == 4:
				newRegister[4] = register[4] ^ register[3]
			else:			
				newRegister[i] = register[i - 1]
		return newRegister
	
	newRegister = [0] * 5
	register = [0] * 5
	size = 5
	print(0,register)
	index = 1
	#input bit
	for inputValue in bitArray:
		newRegister = hardware(register, inputValue)
		register = newRegister[:]
		print(index, register)
		index +=1

	#extra bit for remainder
	for i in range(size):
		inputValue = 0
		newRegister = hardware(register, inputValue)
		register = newRegister[:]
		print(index, register)
		index +=1

	#reverse register array
	register = register[::-1]
	return register

#G(X) = 110101
def cal_CRC_testFast(bitArray):
	def hardware(register, inputValue):
		newRegister = [0]*5
		for i in range(size):
			if i == 0:
				newRegister[0] = inputValue ^ register[4] 
			elif i == 2:
				newRegister[2] = inputValue ^ register[4] ^ register[1] 
			elif i == 4:
				newRegister[4] = inputValue ^ register[4] ^ register[3] 			
			else:			
				newRegister[i] = register[i - 1]
		return newRegister
	
	newRegister = [0] * 5
	register = [0] * 5
	size = 5
	print(0,register)
	index = 1
	#input bit
	for inputValue in bitArray:
		newRegister = hardware(register, inputValue)
		register = newRegister[:]
		print(index, register)
		index += 1

	#reverse register array
	register = register[::-1]
	return register


def cal_CRC_32(bitArray):
	Gx = [26,23,22,16,12,11,10,8,7,5,4,2,1]
	pholinomialSize = 32
	def hardware(register, inputValue, pholinomialSize = 32):
		newRegister = [0]*pholinomialSize
		lastRegisterIndex = size - 1
		#shift reigster
		#newRegister = 
		newRegister[0] = inputValue ^ register[lastRegisterIndex] 			
		#Gx 
		for i in Gx:
			newRegister[i] = inputValue ^ register[lastRegisterIndex] ^ register[i-1] 

		return newRegister
	
	newRegister = [0] * pholinomialSize
	register = [0] * pholinomialSize
	print(0,register) 
	index = 1
	#input bit
	for inputValue in bitArray:
		newRegister = hardware(register, inputValue)
		register = newRegister[:]
		print(index, register)
		index += 1

	#reverse register array
	register = register[::-1]
	return register




def cal_internetCheckSum(bitArray):
	checkSumValue = 0

	return checkSumValue



		
def countFileLines():
	totalLine =0
	for path in os.listdir():
		lineCount = 0
		print(path)
		file = open(path)
		for line in file:
			lineCount += 1
		print( lineCount, path)
		totalLine += lineCount
		print (totalLine)
	return 

if  __name__  == "__main__":
	countFileLines()



	#file = open("sample.txt")
	#frames = []
	#for line in file:
	#	frames +=[line]
	#	print("ascii : ", line)
	#	bitArray = strToBitArray(line)
	#	print("bit array : ", bitArray)
	
	a = [1,0,1,0,0, 0,1,1,0,1]
	print(cal_CRC_test(a))
	print ()
	print(cal_CRC_testFast(a))
	pass





