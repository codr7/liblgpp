from threading import Thread
from timeit import default_timer as timer
from time import sleep

n = 3

def f(): sleep(n)

ts = []
for _ in range(n):
    t = Thread(target=f)
    ts.append(t)
    t.start();

tm = timer()
for t in ts: t.join()
print('{0}us'.format(round((timer() - tm) * 1000000)))
