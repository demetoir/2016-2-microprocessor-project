# http://127.0.0.1:8080/

import os
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash
from flask_bootstrap import Bootstrap

# create our little application :)
app = Flask(__name__)
app.secret_key = '123123sdfgsdfgerherhasfgwag34yq34hsesdfgegfdg123'

#교도소 엘리베이터
#교도소 도어락
#주차 시스템???
#디지털 냉장고 도어락
#기숙사 공동 냉장고용 디지털 도어락
#refriger locker

@app.route('/')
def show_loginPage():
	print(session)
	return render_template('loginPage.html')

@app.route('/logout', methods = ['post'])
def logout():
	session.pop('userName', None)
	return redirect(url_for('show_loginPage'))

@app.route("/login", methods = ['POST'])
def check_login():
	rootId = 'root'
	rootPwd = 'root'
	userId = 'user'
	userPwd = 'user'

	if request.method == 'POST':
		id = request.form['userName']
		pwd = request.form['password']

		if id == rootId and pwd == rootPwd:
			session['userName'] = request.form['userName']
			return redirect(url_for('show_rootPage'))
		if id == userId and pwd == userPwd:
			session['userName'] = request.form['userName']
			return redirect(url_for('show_mainPage'))

		return 'wrong pwd'
	else :
		return "login fail"

@app.route('/mainPage')
def show_mainPage():
	#if  session out break

	
	page = render_template("mainPage.html", 
			doorLockConnection = 'door Lock connection',	
			doorLockerState = 'door Lock State'
			)
	return page

@app.route('/rootPage')
def show_rootPage():
	return render_template('rootPage.html')

@app.route('/doorLockSetting', methods = ['post'])
def show_DoorLockSetting():
	return render_template('doorLockSetting.html',
			setting1 = 'setting 1', 
			setting2 = 'setting 2',
			setting3 = 'setting 3'
			)

@app.route('/doorLockLog', methods = ['post'])
def show_doorLockLog():
	return render_template('doorLockLog.html', 
			log = "loooog"
			)



if __name__ == "__main__":
	Bootstrap(app)
	app.run(host='127.0.0.1', port=8080)


