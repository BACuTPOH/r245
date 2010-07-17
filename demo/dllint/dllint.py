#!/usr/bin/env python
# -*-coding: utf-8 -*-

from ctypes import *
class R245dll():
	def __init__(self, adr):
		"""R245dll(adr) adr		|reader adress
		export R2x245.dll library's functions"""
		self.reader = windll.R2x245
		self.vshort = c_short()
		self.vdlong = c_long
		self.adress = adr
	def connect(self, portn):
		"""connect(portn)
		portn	|port where to connect
		open port <portn> for writing and reading"""
		self.reader.InterfaceEnumerate() 	#don't know why, but this function is required
											#to get the reader workable
		return self.reader.InterfaceOpen(portn) 
	def disconnect(self):
		"""disconnect()
		close previously opened port"""
		return self.reader.InterfaceClose()
	def set_damping(self, channel, damp):
		"""set_damping(channel, damp)
		channel	|select reader channel
		damp	|channel damping [0..31]
		attenuate signal in channel <channel> for <damp>"""
		if channel == 1:
			return self.reader.SetDamping_1(self.adress, damp)
		elif channel == 2:
			return self.reader.GetDamping_2(self.adress, damp)
		else:
			return 1
	def set_tag_mem_time(self, channel, time):
		"""set_tag_mem_time(channel, time)
		channel	|select reader channel
		time	|time in milliseconds to store tag ID in reader buffer"""
		if channel == 1:
			return self.reader.SetTagMemTime_1(self.adress, time)
		elif channel == 2:
			return self.reader.SetTagMemTime_2(self.adress, time)
		else:
			return 1
	def set_channel_activity(self, channel, act):
		"""set_channel_activity(chanel, act)
		channel	|select reader channel
		act		|channel activity [0/1]
		activate/deactivate channel <channel>"""
		if channel == 1:
			return self.reader.SetChannelActivity_1(self.adress, act)
		elif channel == 2:
			return self.reader.SetChannelActivity_2(self.adress, act)
		else:
			return 1
	def get_num_tag(self, channel):
		"""get_num_tag(channel)
		channel	|select reader channel
		get number of tags in channel <channel> buffer"""
		num_tags = c_short()
		if channel == 1:
			self.reader.GetNumTags_1(self.adress, byref(num_tags))
			return num_tags.value
		if channel == 2:
			self.reader.GetNumTags_2(self.adress, byref(num_tags))
			return num_tags.value
	def get_first_tag(self, channel):
		"""get_first_tag(channel)
		channel	|select reader channel
		get first tag from channel <channel> buffer"""
		tag_id = c_long()
		if channel == 1:
			self.reader.GetFirstTag_1(self.adress, byref(tag_id))
			return tag_id.value
		if channel == 2:
			self.reader.GetFirstTag_2(self.adress, byref(tag_id))
			return tag_id.value
	def get_next_tag(self, channel):
		"""get_next_tag(channel)
		channel	|select reader channel
		get next tag from channel <channel> buffer"""
		tag_id = c_long()
		if channel == 1:
			self.reader.GetNextTag_1(self.adress, byref(tag_id))
			return tag_id.value
		if channel == 2:
			self.reader.GetNextTag(self.adress, byref(tag_id))
			return tag_id.value
	def get_transact(self):
		"""get_transact()
		get next transact from transaction buffer of reader"""
		codelst = {0x01:'Питание включено',\
				0x02:'Питание выключено',\
				0x10:'Обнаружен таг',\
				0x11:'Таг потерян',\
				0x20:'Вибродатчик сработал',\
				0x21:'Вибродатчик восстановлен',\
				0x23:'Тампер корпуса восстановлен',\
				0x24:'Батарейка тэга села',\
				0x25:'Батарейка тэга восстановлена',\
				0x124: 'Таг передан'}
		for i in xrange(0x12E):
			if i not in codelst.keys():
				codelst[i] = str(hex(i))
		code = c_short()
		ch = c_short()
		tid = c_long()
		year = c_long()
		month = c_long()
		day = c_long()
		hour = c_long()
		min = c_long()
		sec = c_long()
		dow = c_long()
		if self.reader.GetNextTransaction(self.adress, byref(code), byref(ch), byref(tid),\
				byref(year), byref(month), byref(day), byref(hour), byref(min), byref(sec), byref(dow)) == 0:
			return "%s \t\t\t %d \t\t %s \t\t %d.%d.%d %d:%d:%d \t\t\t %d" % \
					(codelst[code.value], ch.value, hex(tid.value),\
					day.value, month.value, year.value, hour.value, min.value, sec.value, dow.value)
		else:
			return 1
