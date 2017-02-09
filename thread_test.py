import _thread
import time
import random

count = 0


def loop(threadname, delay=.1):
    for i in range(100):
        global count
        temp = count
        count = temp + 1


for i in range(10):
    _thread.start_new_thread(loop, ("Thread-" + str(i), random.uniform(0, 1)))
    

while 1:
    pass

