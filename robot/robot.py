#! /usr/bin/python3

""" RobotTCP | BIE-PSI HW1 | katipber@fit.cvut.cz """

import re
import sys
import time
import struct
import socket
import threading
from queue import Queue
from random import randint
import codecs

BUFF_SIZE = 1024

def recvall(sock):
	data = ""
	while True:
		try:
			part = sock.recv(BUFF_SIZE)
		except socket.error as e:
			print("Error receiving data: " + str(e))
			return False
		if len(part) < 1: 
			return False
		data += part.decode('latin-1')
		if re.search('\r\n$', data) != None: 
			break
	return data
	
	
""" ********* """
""" * ROBOT * """
""" ********* """
class Robot(object):

	def __init__(self):
		self.x = randint(-17,17)
		self.y = randint(-17,17)
		self.dir = randint(0,3)
		self.dur = 0
		self.block = None
		self.name = "Megamind"
		self.secret = "YOU WILL DIE"
		
	def parse(self, data):
		try:
			data = re.search(self.name + ' (.*)', data).group(1)
		except Exception:
			return False
		return data.split(" ")
		
	def reply(self, code):
		if code == 210:
			return (True, "210 Hello, My name is " + self.name + ". Let's move!\r\n")
		if code == 240:
			return (True, "240 OK (" + str(self.x) + "," + str(self.y) + ")\r\n")
		if code == 260:
			return (False, "260 SUCCESS " + self.secret + "\r\n")
		if code == 500:
			return (True, "500 UNKNOWN COMMAND\r\n")
		if code == 530:
			return (False, "530 CRASHED\r\n")
		if code == 550:
			return (False, "550 CANNOT PICK UP THE SIGN\r\n")
		if code == 571:
			return (False, "571 THIS BLOCK IS OK\r\n")
		if code == 572:
			return (False, "572 ROBOT FELLS TO PIECES\r\n")
		if code == 580:
			return (True, "580 FAILURE OF BLOCK #" + str(self.block) + "\r\n")
			
	def step(self):
		if self.block != None:
			return self.reply(572)
		self.dur += randint(1,10)
		if self.dur >= 10:
			self.block = randint(1,9)
			return self.reply(580)
		if self.dir == 0:
			self.y += 1
		elif self.dir == 1:
			self.x -= 1
		elif self.dir == 2:
			self.y -= 1
		elif self.dir == 3:
			self.x += 1
		if abs(self.x) > 18 or abs(self.y) > 18:
			return self.reply(530)
		return self.reply(240)
		
	def turn(self):
		self.dir = (self.dir+1)%4
		return self.reply(240)
		
	def repair(self, data):
		block = int(data)
		if block < 1 or block > 9:
			return self.reply(500)
		if self.block != block:
			return self.reply(571)
		self.block = None
		self.dur = 0
		return self.reply(240)
	
	def pickup(self):
		if self.x != 0 or self.y != 0:
			return self.reply(550)
		return self.reply(260)
	
	def process(self, p):
		if not p:
			return self.reply(500)
		if p[0] == "LEFT":
			return self.turn()
		if p[0] == "STEP":
			return self.step()
		if len(p) < 2:
			return self.reply(500)
		if p[0] == "REPAIR":
			if not p[1].isdigit():
				return self.reply(500)
			return self.repair(p[1])
		if p[0] == "PICK" and p[1] == "UP":
			return self.pickup()
		return self.reply(500)


""" ********** """
""" * SERVER * """
""" ********** """
class Server(object):
	
	def __init__(self):
		self.serv = ('localhost', int(sys.argv[1]))
		self.sock = None
	
	def setSock(self):
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		except socket.error as e:
			print("Socket creation error: ", str(e))
			exit(1)
		try:
			print(self.serv[0], self.serv[1])
			self.sock.bind(self.serv)
		except socket.error as e:
			print("Socket binding error: ", str(e))
			exit(1)
		return
	
	def acceptCli(self):
		self.sock.listen(5)
		while True:
			try:
				conn, addr = self.sock.accept()
			except socket.error as e:
				print("Error accepting connection: ", str(e))
				continue
			print("HERE COMES A NEW CHALLENGER --> {0}({1})".format(addr[0], addr[1]))
			robot = Robot()
			t = threading.Thread(target=self.talk, args=(conn, addr, robot,))
			t.daemon = True
			t.start()
			print("Thread count: ", threading.active_count())
	
	def talk(self, c, a, r):
		ans = (False, "")
		c.send(str.encode(r.reply(210)[1]))
		while True:
			data = recvall(c)
			if not data: break
			data = data.split('\r\n')
			for i in range(0,len(data)-1):
				cmd = r.parse(data[i])
				ans = r.process(cmd)
				if cmd:	cmd = ' '.join(cmd)
				else: cmd = "N/A"
				cmd = ("{0}({1}) {2} [{3:3d},{4:3d}] {5:2d} {6}".format(a[0], a[1], cmd.ljust(9), r.x, r.y, r.dur, ans[1]))
				print(cmd)
				c.send(str.encode(ans[1]))
			if ans[0] == False: break
		if ans[1].split(" ")[0] == "260": rez = "WON"
		else: rez = "LOST"
		print("GAME OVER\nPLAYER {0}({1}) {2}".format(a[0], a[1], rez))
		c.close()


""" ************ """
""" * OPERATOR * """
""" ************ """
class Operator(object):
	
	def __init__(self):
		self.robot = None
		self.x = None
		self.y = None
		self.curr = "STEP"
		self.last = "LEFT"
		self.que = Queue()
		
	def getName(self, data):
		try:
			data = re.search('My name is ([^.]*)', data).group(1)
		except Exception:
			print("Name not recognized\nDisconnecting...")
			return None
		self.robot = data + " "
		return "LEFT"

	def getCord(self, data):
		try:
			data = re.search('[(](-*[0-9]*),(-*[0-9]*)[)]', data)
		except Exception:
			print("Coordinates not recognized\nDisconnecting...")
			return None
		rx = int(data.group(1))
		ry = int(data.group(2))
		return (rx, ry)
			
	def getInfo(self, cord):
		x = cord[0]
		y = cord[1]
		if 		y >  0 	and x >= 0:	region = 1
		elif 	y >= 0 	and x <  0:	region = 2
		elif 	y <  0 	and x <= 0:	region = 3
		elif 	y <= 0 	and x >  0:	region = 4
		if 	 self.y < y:					direction = 'n'
		elif self.x < x:					direction = 'e'
		elif self.x > x:					direction = 'w'
		elif self.y > y:					direction = 's'
		else: return None
		return (region, direction)
	
	def setQue(self, cord):
		dx = abs(cord[0])
		dy = abs(cord[1])
		info = self.getInfo(cord)
		if info == None:
			if self.curr == None:
				self.curr = self.last
			return
		
		if info[0] == 1 and info[1] == 'n':
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
		elif info[0] == 1 and info[1] == 'w':
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
		elif info[0] == 1 and info[1] == 's':
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 1 and info[1] == 'e':
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")

		elif info[0] == 2 and info[1] == 'n':
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 2 and info[1] == 'w':
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 2 and info[1] == 's':
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 2 and info[1] == 'e':
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")

		elif info[0] == 3 and info[1] == 'n':
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 3 and info[1] == 'w':
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
		elif info[0] == 3 and info[1] == 's':
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
		elif info[0] == 3 and info[1] == 'e':
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")

		elif info[0] == 4 and info[1] == 'n':
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 4 and info[1] == 'w':
			for i in range(0,dx):
				self.que.put("STEP")
			self.que.put("LEFT")
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
		elif info[0] == 4 and info[1] == 's':
			self.que.put("LEFT")
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		elif info[0] == 4 and info[1] == 'e':
			self.que.put("LEFT")
			for i in range(0,dy):
				self.que.put("STEP")
			self.que.put("LEFT")
			for i in range(0,dx):
				self.que.put("STEP")
		
	def move(self, data):
		cord = self.getCord(data)
		if cord[0] == 0 and cord[1] == 0:
			self.rlast = "PICK UP"
			return "PICK UP"
		if self.x == None:
			self.x = cord[0]
			self.y = cord[1]
		elif self.que.empty():
			self.setQue(cord)
		self.x = cord[0]
		self.y = cord[1]
		if self.curr == None:
			self.curr = self.que.get()
		data = self.curr
		self.last = self.curr
		self.curr = None
		return data

	def repair(self, data):
		try:
			data = re.search('BLOCK.*([0-9])', data).group(1)
		except Exception:
			print("Part not recognized\nDisconnecting...")
			return None
		if self.curr == None: self.curr = self.last
		self.last = "REPAIR " + data
		return self.last
		
	def repeat(self):
		return self.rlast
		
	def process(self, data):
		code = data.split(" ")
		if 	 code[0] == "210": data = self.getName(data)
		elif code[0] == "240": data = self.move(data)
		elif code[0] == "260": data = None
		elif code[0] == "500": data = self.repeat()
		elif code[0] == "530": data = None
		elif code[0] == "550": data = None
		elif code[0] == "571": data = None
		elif code[0] == "572": data = None
		elif code[0] == "580": data = self.repair(data)
		else: 								 data = self.repeat()
		if data == None: return None
		return self.robot + data + '\r\n'
	
		
""" ********** """
""" * CLIENT * """
""" ********** """
class Client(object):

	def __init__(self):
		self.cli = (sys.argv[1], int(sys.argv[2]))
		self.sock = None
		self.op = Operator()
		self.rname = None
		self.rx = None
		self.ry = None
		self.rmove = "STEP"
		self.rlast = "LEFT"
		self.mqueue = Queue()
	
	def setSock(self):
		try:
			self.sock = socket.socket()
		except socket.error as e:
			print("Socket creation error: ", str(e))
			exit(1)
		try:
			print(self.cli[0], self.cli[1])
			self.sock.connect(self.cli)
		except socket.error as e:
			print("Socket connecting error: ", str(e))
			exit(1)
		return
		
	def talk(self):
		data = recvall(self.sock)
		print(data)
		data = self.op.process(data)
		while True:
			print(data)
			self.sock.send(str.encode(data))
			data = recvall(self.sock)
			print(data)
			data = self.op.process(data)
			if data == None: break
		print("Session ended")
		self.sock.close()


""" ******** """
""" * MAIN * """
""" ******** """
def main():
	
	if len(sys.argv) == 1 or len(sys.argv) > 3:
		print("USAGE:\nServer:\t" + sys.argv[0] + " <port>\nClient:\t" + sys.argv[0] + " <server> <port>")
		exit(1)
		
	if len(sys.argv) == 2:
		serv = Server()
		serv.setSock()
		serv.acceptCli()
		
	else:
		cli = Client()
		cli.setSock()
		cli.talk()

def test():
	data = "asd\r\nafsd"
	print(data.find('\r\n'))
	try:
		rx = re.search('\r\n$', data)
	except Exception:
		print(False)
	print(rx)
	data = data.split('\r\n')
	l = len(data) -1
	print(l)
	i = 0
	for x in range(0,l):
		print(i,x)
		i += 1
	
main()
