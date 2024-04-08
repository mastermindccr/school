import os

with open("random.bin", "wb") as f:
    f.write(os.urandom(1024*1024))