from copy import deepcopy

_alpha = [0, 0, 0 ,0, 0, 0, 0, 1] # power of alpha
_p = [1, 0, 0, 0, 1, 1, 1, 0, 1] # primitive polynomial
_seed = [0, 0, 0, 0, 0, 0, 0, 1]

def crypt(plaintext):
    alpha = deepcopy(_alpha)
    p = deepcopy(_p)
    seed = deepcopy(_seed)

    b_plaintext = [int(x) for x in ''.join(format(ord(i), '08b') for i in plaintext)]
    ciphertext = ''

    for t in b_plaintext:
        # calculate key
        cur = 0
        for i in range(len(alpha)):
            if alpha[i] == 1:
                cur ^= seed[i]
        ciphertext += str(t ^ cur)

        # calculate next power of alpha
        alpha.append(0)
        if alpha[0]==1:
            alpha = [alpha[i] ^ p[i] for i in range(1, len(p))]
        else:
            alpha = alpha[1:]
    return ciphertext

def bin_to_str(binary):
    return ''.join(chr(int(binary[i:i+8], 2)) for i in range(0, len(binary), 8))

print('----------------------------------------')
print('Problem 2-1:')

plaintext = 'ATNYCUWEARESTRIVINGTOBEAGREATUNIVERSITYTHATTRAN' \
'SCENDSDISCIPLINARYDIVIDESTOSOLVETHEINCREASINGLYCO' \
'MPLEXPROBLEMSTHATTHEWORLDFACESWEWILLCONTINUET' \
'OBEGUIDEDBYTHEIDEATHATWECANACHIEVESOMETHINGMU' \
'CHGREATERTOGETHERTHANWECANINDIVIDUALLYAFTERALLT' \
'HATWASTHEIDEATHATLEDTOTHECREATIONOFOURUNIVERSI' \
'TYINTHEFIRSTPLACE'
b_ciphertext = crypt(plaintext)
b_decrypted_plaintext = crypt(bin_to_str(b_ciphertext))

print(f'Plaintext: {plaintext}')
print('----------------------------------------')
print(f'Ciphertext in binary: {b_ciphertext}')
print('----------------------------------------')
print(f'Decrypted plaintext in binary: {b_decrypted_plaintext}')
print('----------------------------------------')
print(f'Decrypted plaintext: {bin_to_str(b_decrypted_plaintext)}')
print('----------------------------------------')
print('Problem 2-2:')
msbs = [b_ciphertext[i] for i in range(0, len(b_ciphertext), 8)]
# calculate how many bits are a cycle
cycle = [msbs[0]]
tmp = []
cycle_length = 0
for i in range(1, len(msbs)):
    if msbs[i] != cycle[cycle_length]:
        tmp.append(msbs[i])
        while tmp!=cycle[0:len(tmp)]:
            cycle.append(tmp[0])
            tmp.pop(0)
        cycle_length = len(tmp)-1
        
    else:
        cycle_length += 1
        tmp.append(msbs[i])
        if cycle_length>=len(cycle):
            cycle_length -= len(cycle)
print(f'Msbs: {msbs}')
print(f'Cycle length: {len(cycle)}')
