from timeit import default_timer as timer

n = 1000000

def coro():
    for _ in range(n): yield

t = timer()
c = coro()
for _ in range(n): next(c)
print('{0}us'.format(round((timer() - t) * 1000000)))
