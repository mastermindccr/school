from copy import deepcopy

alpha = [0, 0, 0, 0, 0, 0, 0, 1] # power of alpha
p = [1, 0, 0, 0, 1, 1, 1, 0, 1] # primitive polynomial
polynomials = []
while not alpha in polynomials:
    polynomials.append(deepcopy(alpha))
    alpha.append(0)
    if alpha[0]==1:
        alpha = [alpha[i] ^ p[i] for i in range(1, len(p))]
    else:
        alpha = alpha[1:]
for i in range(len(polynomials)):
    print(f'{i+1:>3}: {polynomials[i]}')
