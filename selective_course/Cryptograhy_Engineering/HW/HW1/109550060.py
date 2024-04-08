from pprint import pprint
def exchange(a, b, frequency_list):
    first = frequency_list.index(a)
    second = frequency_list.index(b)
    frequency_list[first], frequency_list[second] = frequency_list[second], frequency_list[first]

def exchangeAll(exchange_list, frequency_list):
    for pair in exchange_list:
        exchange(pair[0], pair[1], frequency_list)
cipherText = 'C UYGHARMZ IUWMPRWIR GAIR YVRMP\n' \
'MBHMZWMPUM C VMMXWPE YV PYR VCZ\n'  \
'ZMGYQMD VZYG CXCZG YP CPCXKTWPE CPD MBHXYZM\n' \
'RNM VXYYD YV CDQCPUMD OPYSXMDEM SNWUN MCUN\n' \
'KMCZ LZWPEI SWRN WR\n'
frequency_list = "E A R I O T N S L C U D P M H G B F Y W K V X Z J Q".split()

counter = dict()
for c in range(65, 91):
    counter[chr(c)] = 0
for c in cipherText:
    if c.isalpha():
        counter[c] += 1
# 
# print frequency
pprint(counter)

# solve ciphertext by exchanging the letters
counter = [x[0] for x in sorted(counter.items(), key=lambda x: x[1], reverse=True)]
mapping = dict()

ex_list = ['IO', 'SF', 'IT', 'RN', 'PH', 'WB', 'UL', 'MG', 'DM', 'BY', 'UD', 'VK', 'XK', 'PU', 'US', 'XB']
exchangeAll(ex_list, frequency_list)

for i in range(26):
    mapping[counter[i]] = frequency_list[i]
plainText = ""
for c in cipherText:
    if c.isalpha():
        plainText += mapping[c]
    else:
        plainText += c

print(plainText)