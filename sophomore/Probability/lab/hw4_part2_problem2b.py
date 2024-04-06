import numpy

size = [10,10**3,10**5,10**7]
for cur in size:
    x = numpy.random.uniform(-1,1,cur)
    y = numpy.random.uniform(-1,1,cur)
    sum = 0
    for i in range(cur):
        sum += 1 if (x[i]-0.2)**2+(y[i]+0.3)**2<=0.25 else 0
    sum = sum * 4 / cur
    print("n =",cur,":",sum)
