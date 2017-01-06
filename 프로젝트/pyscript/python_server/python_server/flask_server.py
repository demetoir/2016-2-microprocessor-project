# http://127.0.0.1:8080/

true = True
false = False

import os
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash
from flask_bootstrap import Bootstrap
from flask import *
import threading 
import datetime
import arm_m3_kit_server 
import traceback
import random 
import sys

userDB ={ 
"root" :{"password" : "root"},
"user1":{"password" : "user1"}
}

lockerDB = {
}

LOCKER_SIZE = 18
def initLockerDb():
	for i in range(1,LOCKER_SIZE+1):
		lockerDB[str(i)] = None
	return 

def registerAccount(userId, pwd):
	userDB[userId] = {"password": pwd }
	return true

def isAbleId(id):
	if id in userDB:
		return false
	return true

def sessionPrinter():
	print(datetime.datetime.now(), session)
	return 


# create our little application :)
app = Flask(__name__)
app.secret_key = '123123sdfgsdfgerherhasfgwag34yq34hsesdfgegfdg123'

@app.route("/",methods=['post', 'get'])
def topPage():
	return  redirect(url_for('loginPage'))

@app.route('/logoutPage', methods = ['POST'])
def logoutPage():
	if request.method == 'POST':
		##session.clear()
		print("before:",session)
		id = request.form['id']
		print("logout id : ", id)
		session.pop(id, None)
		print("after: ",session)
		return redirect(url_for('loginPage'))
	else:
		return "logout fail"

@app.route('/loginPage')
def loginPage():	
	print("login Page : session:", session)
	return render_template('loginPage.html')

@app.route("/checkLogin", methods = ['POST'])
def checkLogin():
	print("check login : session :", session)
	
	if request.method == 'POST':
		user_id = request.form['userId']
		pwd = request.form['password']
		
		if user_id in userDB and userDB[user_id]["password"] == pwd:
			session[user_id] = user_id
			if user_id == "root":
				return redirect(url_for('rootMainPage', id=user_id))
			else:
				return redirect(url_for('userMainPage', id=user_id))
		return 'login fail'
	else :
		return "login fail"

@app.route("/completeMakeNewAccountPage", methods = ['post','get'])
def completeMakeNewAccountPage():
	return render_template("completeMakeNewAccountPage.html")

@app.route("/checkNewAccount", methods =['POST'])
def checkNewAccount():
	userId = request.form['userId']
	pwd = request.form['pwd']
	confirmPwd = request.form['confirmPwd']

	if pwd != confirmPwd:
		return redirect( url_for('makeNewAccountPage', isPwdNotConfirm = true) )

	if isAbleId(userId) == false :
		return redirect(url_for('makeNewAccountPage', isIDNotAble = true))
	
	registerAccount(userId, pwd)

	return redirect(url_for('completeMakeNewAccountPage'))

@app.route('/userMainPage', methods = ['POST', 'GET'])
def userMainPage():
	if request.method == 'POST':
		userId = request.form['id']
	else:
		userId = request.args['id']
		print(userId)
		sessionPrinter()

	return render_template(
		"userMainPage.html",
		userId = userId,
		otp_value = "444",
		time_left = "5555",
		userDb = userDB[userId],
		str = str,
		time = str(datetime.datetime.now()),
		lockerDb = lockerDB
		)


@app.route('/makeNewLockerPage', methods = ['post', 'get'] )
def makeNewLockerPage():
	userId = request.form['id']
	
	return render_template( 
			"makeNewLockerPage.html",
			userId = userId,
			lockerDB = lockerDB,
			str = str,
			none = None
			)

##사용자가 요청한 락커 사용 신청을 처리한다
##락커의 비밀번호를 랜덤으로 설정하고 락커를 등록한다
@app.route('/checkMakeNewLocker', methods = ['post'] )
def checkMakeNewLocker():
	userId = request.form['id']
	lockerNumber = request.form['lockerNumber']
	lockerPassword = "%04d"%(random.randint(0,9999))
	lockerDB[str(lockerNumber)] = { "userID" : userId , "lockerPassword" : lockerPassword } 
	return redirect(url_for("completeMakeNewLockerPage", userId = userId))	

@app.route('/completeMakeNewLockerPage', methods = ['post', 'GET'] )
def completeMakeNewLockerPage():
	userId = request.args['userId']
	return render_template(
			"completeMakeNewLockerPage.html",
			userId = userId
				)

##사용한 락커를 반납하는 페이지
@app.route('/returnLocker', methods = ['POST', 'GET'])
def returnLocker():
	userId = request.form["userId"]
	lockerNumber = request.form["lockerNumber"]
	lockerDB[ lockerNumber ] = None
	if userId == "root":
		return redirect(url_for('rootMainPage', id = userId))
	else :
		return redirect(url_for('userMainPage', id = userId))


@app.route('/rootMainPage')
def rootMainPage():
	sessionPrinter()
	id = request.args['id']
	return render_template(
			'rootMainPage.html',
			userId = id, 
			lockerDb = lockerDB,
			str = str,
			time = str(datetime.datetime.now()),
			)

@app.route('/killAllSession', methods = ['post'])
def killAllSession():
	session.clear()	
	return redirect(url_for('loginPage'))

@app.route('/makeNewAccountPage' , methods = ['post', 'get'])
def makeNewAccountPage():
	return render_template("makeNewAccountpage.html")

##웹서버와 키트와의 UART통신을 하기위해 두개의 스레드로 각각 실행하도록 만든다
if __name__ == "__main__":
	initLockerDb()
	try:
		threadList = []
		armServer = arm_m3_kit_server.arm_m3_kit_server()
		thArmServer = threading.Thread(target = armServer.main, kwargs={'_lockerDB': lockerDB} )
		thArmServer.start()
		#thFlaskServer = threading.Thread(target = app.run, args = ('192.168.0.2', 8080) )
		thFlaskServer = threading.Thread(target = app.run, args = ('127.0.0.1', 8080) )
		thFlaskServer.start()
		threadList +=[thArmServer, thFlaskServer]
	
		for t in threadList:
			t.join()
	except:
		traceback.print_exc(file = sys.stdout)
	




