def fib(n):
    if n<=1:
        return 1
    else:
        return fib(n-1)+fib(n-2)

import sys
print(str(fib(int(sys.argv[1]))))
