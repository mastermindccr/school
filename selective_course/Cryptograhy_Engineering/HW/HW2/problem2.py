import hashlib
import time

class hash_function:
    def __init__(self, name, func):
        self.name = name
        self.func = func
        self.speed = 0
        self.hash = ''

algos = [
    hash_function('md5', hashlib.md5()),
    hash_function('sha1', hashlib.sha1()),
    hash_function('sha224', hashlib.sha224()),
    hash_function('sha256', hashlib.sha256()),
    hash_function('sha512', hashlib.sha512()),
    hash_function('sha3-224', hashlib.sha3_224()),
    hash_function('sha3-256', hashlib.sha3_256()),
    hash_function('sha3-512', hashlib.sha3_512())
]

with open('video.mp4', 'rb') as f:
    content = f.read()
    for algo in algos:
        h = algo.func
        start = time.time()
        h.update(content)
        end = time.time()
        algo.speed = end - start
        algo.hash = h.hexdigest()

sorted_algos = sorted(algos, key=lambda x: x.speed)
print("-"*187)
print(f"|{'name':^8}|{'speed(s)':^25}|{'hash value':^150}|")
print("-"*187)
for algo in sorted_algos:
    print(f"|{'':^8}|{'':^25}|{'':^150}|")
    print(f"|{algo.name:^8}|{algo.speed:^25}|{algo.hash:^150}|")
    print(f"|{'':^8}|{'':^25}|{'':^150}|")
    print("-"*187)