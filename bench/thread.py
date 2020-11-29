from threading import Thread
from timeit import default_timer as timer
from time import sleep

def f(): sleep(1)

tm = timer()
t = Thread(target=f)
t.start();
t.join()
print('{0}us'.format(round((timer() - tm) * 1000000)))
