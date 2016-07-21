#!/usr/bin/env python
import time
import socket

def test():
	HOST =  'localhost' 
	PORT = 9999 
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind((HOST, PORT))
	s.listen(1)
	conn, addr = s.accept()
	print 'Connected by', addr
	a = 0
	while a <= 100:
		a += 1
		data = conn.recv(1024)
		time.sleep(0.3)
		print data


test()
