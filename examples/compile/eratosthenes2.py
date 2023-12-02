#!/usr/bin/python
def eratosthenes(n):
  numbers=list(range(2,n+1))
  while numbers:
    print(int(numbers[0]))
    numbers=list(filter(
      lambda x: x%numbers[0]!=0,
      numbers))
eratosthenes(int(input("")))
