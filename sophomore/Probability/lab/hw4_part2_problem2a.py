import numpy

size = [10**3,10**5]
for cur in size:
    print("size =",cur)
    for it in range(20):
        z = numpy.random.normal(0,1,cur)
        sum = 0
        for i in z:
            sum += numpy.cos(i)+numpy.sin(2*i)
        sum/=cur
        print("iteration",it,":",sum)
