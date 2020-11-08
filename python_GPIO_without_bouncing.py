#!/usr/local/bin/python3
# coding=utf-8

# Copyright (c) 2020 KOBOT kobot9@naver.com.
# All rights reserved.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import Jetson.GPIO as GPIO
import time
button_pin = 33

GPIO.setmode(GPIO.BOARD)
GPIO.setup(button_pin, GPIO.IN) 
GPIO.add_event_detect(button_pin, GPIO.FALLING)
while(1):
	if GPIO.event_detected(button_pin):
		print("yeah!!")
		GPIO.remove_event_detect(button_pin)
		time.sleep(0.3)
		GPIO.add_event_detect(button_pin, GPIO.FALLING)


