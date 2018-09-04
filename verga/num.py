'''
Created on 03/09/2018

@author: ernesto
'''

# XXX: http://codeforces.com/problemset/problem/394/B
# XXX: https://www.quora.com/How-does-this-logic-for-finding-GCD-of-bigger-numbers-work
# XXX: https://www.khanacademy.org/computing/computer-science/cryptography/modarithmetic/a/the-euclidean-algorithm
# XXX: https://iclykofte.com/greatest-common-divisor-gcd-of-big-numbers/

from math import gcd


def caca(a, b):
    d = gcd(a, b)
    r = b // d
#    print("r {}".format(r))
    if r < 10:
        return r
    return None


p, x = [int(y) for y in input().strip().split(" ")]
a = int("".join(["9"] * p))
b = 10 * x - 1
# print("p {} x {} a {} b {}".format(p, x,a,b))

# print("1")
r = caca(a, b)
# print("2")
res = None
if r is not None:
    y = r
    res = (y * a) // b
    if len(str(res)) < p:
        res *= x
    if len(str(res)) < p:
        res = None

if res is None:
    print("Impossible")
else:
    print(res)
