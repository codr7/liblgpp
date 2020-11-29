from timeit import default_timer as timer

def coro(n):
    while n >= 0:
        n -= 1
        yield(n)

t = timer()
c = coro(1000000)
while next(c) >= 0: pass
print('{0}us'.format(round((timer() - t) * 1000000)))
