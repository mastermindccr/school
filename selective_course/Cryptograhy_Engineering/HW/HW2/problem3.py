import numpy as np
from copy import deepcopy

cipherText = 'UONCS VAIHG EPAAH IGIRL BIECS' \
'TECSW PNITE TIENO IEEFD OWECX' \
'TRSRX STTAR TLODY FSOVN EOECO' \
'HENIO DAARQ NAELA FSGNO PTE'

cipherText = cipherText.replace(' ', '')

length = len(cipherText)

mats = np.array([])

for row in range(1, length+1):
    if length%row == 0:
        col = length//row
        mat = [[cipherText[j*row+i] for j in range(col)] for i in range(row)]
        print(f'row: {row}, col: {col}')
        diff_sum = 0
        for i in range(row):
            vowel = 0
            for j in range(col):
                if mat[i][j] in 'AEIOU':
                    vowel += 1
            diff_sum += abs(col*0.4-vowel)
        mats = np.append(mats, {
            'row': row,
            'col': col,
            'mat': np.array(mat)
        })
        print(f'diff_sum: {diff_sum}')
    else:
        mats = np.append(mats, {
            'row': 0,
            'col': 0,
            'mat': np.array([])
        })

# after observation, we eventually find that 14*7 is the answer
mat = mats[13]
# change columns below
changedCol = [(0, 4), (2, 5), (3, 6), (5, 6)]
for i in changedCol:
    temp = deepcopy(mat['mat'][:, i[0]])
    mat['mat'][:, i[0]] = deepcopy(mat['mat'][:, i[1]])
    mat['mat'][:, i[1]] = deepcopy(temp)

for i in range(mat['col']):
    print(i%10, end=' ')
print()
for i in range(mat['row']):
    for j in range(mat['col']):
        print(mat['mat'][i][j], end=' ')
    print()