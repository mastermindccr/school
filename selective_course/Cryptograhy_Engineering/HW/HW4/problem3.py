import random
from copy import deepcopy
cards = [1, 2, 3, 4]

# naive algorithm
def naive_shuffle(cards):
    for i in range(len(cards)):
        n = random.randint(0, len(cards)-1)
        cards[i], cards[n] = cards[n], cards[i]
    return cards

# Fisher-Yates shuffle
def fisher_yates_shuffle(cards):
    for i in range(len(cards)-1, 0, -1):
        n = random.randint(0, i)
        cards[i], cards[n] = cards[n], cards[i]
    return cards

# test a million times
naive_count = {}
fisher_yates_count = {}

for i in range(1000000):
    naive_shuffle_result = naive_shuffle(deepcopy(cards))
    fisher_yates_shuffle_result = fisher_yates_shuffle(deepcopy(cards))
    if not tuple(naive_shuffle_result) in naive_count:
        naive_count[tuple(naive_shuffle_result)] = 0
    else:
        naive_count[tuple(naive_shuffle_result)] += 1
    if not tuple(fisher_yates_shuffle_result) in fisher_yates_count:
        fisher_yates_count[tuple(fisher_yates_shuffle_result)] = 0
    else:
        fisher_yates_count[tuple(fisher_yates_shuffle_result)] += 1

naive_count = sorted(naive_count.items(), key=lambda x: x[0])
fisher_yates_count = sorted(fisher_yates_count.items(), key=lambda x: x[0])

# output result
print('Naive algorithm:')
for i in naive_count:
    print(f'{i[0]}: {i[1]}')
print('Fisher-Yates algorithm:')
for i in fisher_yates_count:
    print(f'{i[0]}: {i[1]}')