import hashlib
import time
from datetime import timedelta
import multiprocessing

easy = "ef0ebbb77298e1fbd81f756a4efc35b977c93dae"
medium = "0bc2f4f2e1f8944866c2e952a5b59acabd1cebf2"
leet_hacker = "9d6b628c1f81b4795c0266c0f12123c1e09a7ad3"
salt = "dfc3e4f0b9b5fb047e9be9fb89016f290d2abb06"
extra = "44ac8049dd677cb5bc0ee2aac622a0f42838b34d"
cpu_cnt = multiprocessing.cpu_count()

# crack easy
def crack_easy():
    with open('password.txt', 'r') as f_read:
        print("--------------------------------------------")
        print("Cracking easy hash...")
        start = time.time()
        attempt = 1
        for candidate in f_read.read().splitlines():
            s = hashlib.sha1()
            s.update(bytes(candidate, 'utf-8'))
            if s.hexdigest() == easy:
                end = time.time()
                print(f"Hash: {s.hexdigest()}")
                print(f"Password: {candidate}")
                print(f"Took {attempt} attempts to crack input hash. Time Taken: {timedelta(seconds=end - start)}")
                break
            attempt += 1
        print("--------------------------------------------")

# crack medium
def crack_medium():
    with open('10-million-password-list-top-1000000.txt', 'r') as f_read:
        print("--------------------------------------------")
        print("Cracking medium hash...")
        start = time.time()
        attempt = 1
        for candidate in f_read.read().splitlines():
            s = hashlib.sha1()
            s.update(candidate.encode('utf-8'))
            if s.hexdigest() == medium:
                end = time.time()
                print(f"Hash: {s.hexdigest()}")
                print(f"Password: {candidate}")
                print(f"Took {attempt} attempts to crack input hash. Time Taken: {timedelta(seconds=end - start)}")
                break
            attempt += 1
        print("--------------------------------------------")

# crack leet
def crack_leet():
    with open('10-million-password-list-top-1000000.txt', 'r') as f_read:
        print("--------------------------------------------")
        print("Cracking leet-hacker hash...")
        passwords = f_read.read().splitlines()
        start = time.time()
        attempt = 1
        for p_salt_candidate in passwords:
            s = hashlib.sha1()
            s.update(bytes(p_salt_candidate, 'utf-8'))
            if s.hexdigest() == salt:
                p_salt = p_salt_candidate
                break
            attempt += 1
        for candidate in passwords:
            s = hashlib.sha1()
            s.update(bytes(p_salt+candidate, 'utf-8'))
            if s.hexdigest() == leet_hacker:
                end = time.time()
                print(f"Hash: {s.hexdigest()}")
                print(f"Password: {candidate}")
                print(f"Took {attempt} attempts to crack input hash. Time Taken: {timedelta(seconds=end - start)}")
                break
            attempt += 1
        print("--------------------------------------------")

# crack extra - use threading to speed up
def crack_extra(portion):
    with open('10-million-password-list-top-1000000.txt', 'r') as f_read:
        attempt = 0
        passwords = f_read.read().splitlines()
        start = time.time()
        for word1 in passwords[int(portion*1000000/cpu_cnt): int((portion+1)*1000000/cpu_cnt)]:
            for word2 in passwords:
                attempt += 1
                s = hashlib.sha1()
                s.update(bytes(word1+' '+word2, 'utf-8'))
                if s.hexdigest() == extra:
                    end = time.time()
                    print(f"Hash: {s.hexdigest()}")
                    print(f"Password: {word1} {word2}")
                    print(f"Took {attempt*cpu_cnt} attempts to crack input hash. Time Taken: {timedelta(seconds=end - start)}")
                    print("--------------------------------------------")
                    finish.value = True
                    return

def worker_init(f):
    global finish
    finish = f

if __name__ == '__main__':
    crack_easy()
    crack_medium()
    crack_leet()

    # crack extra using multiprocessing
    print("--------------------------------------------")
    print("Cracking extra-credit hash...")
    
    try:
        global finish
        total_attempt = multiprocessing.Value('i', 0)
        finish = multiprocessing.Value('i', False)
        pool = multiprocessing.Pool(cpu_cnt, worker_init, (finish,))
        pool.map_async(crack_extra, range(cpu_cnt))
        while not finish.value:
            time.sleep(1)
        pool.terminate()
        pool.join()
    except KeyboardInterrupt:
        pool.terminate()
        pool.join()
