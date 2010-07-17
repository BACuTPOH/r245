#!/usr/bin/env python
# -*- coding: utf-8 -*-

import dllint
from Tkinter import *
from threading import * 
import time

class App(Frame):
	def reader_parse(self):
		self.reader = dllint.R245dll(1)
		if self.reader.connect(0) == 1: 
			self.flag = 0
			print "connect error"
		file = open('journal.txt', 'a')
		while self.flag == 1:
			try: 
				x = self.reader.get_transact() + '\n'
			except:
				x = ''
				print "Error: could not read data."
# why wait?			time.sleep(0.5)
			file.write(x)
			print x
			try:
				self.area_fill(x)
			except:
				pass
		if self.reader != 0:
			self.reader.disconnect
		file.close()

	def __init__(self, master=None, wts=0):
		self.wts = wts
		self.flag = 1
		self.dump = 5
		self.time = 5
		Frame.__init__(self, master)
		self.grid()
		root.protocol("WM_DELETE_WINDOW", self.delete_window)
		wdgc = Thread(target=self.create_widgets, name='cw')
		rparse = Thread(target=self.reader_parse, name='rp')
		rparse.start()
		wdgc.start()

	def create_widgets(self):
# Parameters
		fntMain = ("Courier", "12")
# Output head
		thead = Label(font = fntMain, text = 'Событие \t\t Канал \t\t ID метки \t Дата|Время \t\t День недели')
		thead.grid(row = 0, column = 0, columnspan = 2, sticky="W")
# Text area
		fr = Frame()
		fr.grid(row = 1, column = 0, columnspan = 2)
		Scroll = Scrollbar(fr, relief=FLAT)
		Scroll.pack(side=RIGHT, fill=Y)
		self.Area = Text(fr, font = fntMain, width=120, relief = FLAT, height = 30)
		Scroll.config(command=self.Area.yview)
		self.Area.configure(yscrollcommand=Scroll.set)
		self.Area.pack(side=LEFT)
		self.Area.config(state = DISABLED)
# Config area
		frc = Frame()
		frc.grid(row = 2, column = 0, columnspan = 2)
		dumplab = Label(frc, font = fntMain, text = 'Дальность считывания:   ')
		dumplab.pack(side = LEFT)
		self.Dump = Scale(frc, from_=1, to=31, orient=HORIZONTAL)
		self.Dump.pack(side = LEFT, fill=Y)
		self.Dump.bind('<ButtonRelease-1>', self.dump_scale)
		tlab = Label(frc, font = fntMain, text = 'Время памяти тага:  ')
		tlab.pack(side = LEFT)
		self.Timesc = Scale(frc, from_=1, to=255, orient=HORIZONTAL)
		self.Timesc.pack(side = LEFT, fill=Y)
		self.Timesc.bind('<ButtonRelease-1>', self.time_scale)

	def time_scale(self, event=None):
		if self.flag == 0:
			self.reader.set_tag_mem_time(1, sctime)
			self.reader.set_tag_mem_time(2, sctime)
		self.area_fill("time = %d\n" % (self.Timesc.get()))

	def dump_scale(self, event=None):
		if self.flag == 0:
			self.reader.set_damping(1, scdump)
			self.reader.set_damping(2, scdump)
		self.area_fill("dump = %d\n" % (self.Dump.get()))


	def stop(self):
		self.flag = 0

	def area_fill(self, text):
		self.Area.config(state = NORMAL)
		self.Area.insert(END, "%s" % text)
		self.Area.see(END)
		self.Area.config(state = DISABLED)

	def delete_window(self):
		try:
			self.flag = 0
			root.destroy()
		except:
			pass

root = Tk()
app = App(master=root)
root.mainloop()
