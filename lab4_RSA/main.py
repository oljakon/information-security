import sys
import random
import math
import struct


def get_e(phi):
    while True:
        e = random.randrange(phi)
        if euclid(e, phi) == 1:
            return e


def get_d(phi, e):
    while True:
        gcd, d = advanced_euclide(phi, e)
        if gcd == 1:
            break
    if d < 0:
        d = phi + d
    return d


def euclid(a, b):
    if a < b:
        a, b = b, a
    while b != 0:
        a, b = b, a % b
    return a


def advanced_euclide(a, b):
    x2, x1, y2, y1 = 1, 0, 0, 1
    while b > 0:
        q = a // b
        r = a - q * b

        x = x2 - q * x1
        y = y2 - q * y1
        x2 = x1
        x1 = x
        y2 = y1
        y1 = y

        a = b
        b = r

    d = a
    x = x2
    y = y2

    return d, y


def get_prime_number(bits):
    while True:
        num = random.getrandbits(bits)
        if num % 2 != 0 and miller_rabin(num):
            return num


def miller_rabin(n):
    rounds = int(math.log(n, 2) + 1)
    t = n - 1
    s = 0

    while t % 2 == 0:
        t //= 2
        s += 1

    for _ in range(rounds):
        a = random.randrange(2, n - 2)
        x = pow(a, t, n)
        if x == 1 or x == n - 1:
            return True
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == 1:
                return False
            if x == n - 1:
                return True
        return False

    return True


class RSA:

    def __init__(self, bit_count=512):
        self.bit_count = bit_count
        self.e, self.d, self.n = self.get_keys()

    def get_keys(self):
        p = get_prime_number(self.bit_count)
        q = get_prime_number(self.bit_count)
        n = p * q
        phi = (p - 1) * (q - 1)
        e = get_e(phi)
        d = get_d(phi, e)
        return e, d, n

    def encrypt(self, fl, enc):
        data = fl.read()
        for item in data:
            enc_item = pow(item, self.e, self.n)
            enc.write(str(enc_item) + "\n")

    def decrypt(self, enc, dec):
        line = enc.readline()
        while line:
            num = int(line)
            byte = pow(num, self.d, self.n)
            dec.write(struct.pack('B', byte))
            line = enc.readline()


def main():
    filename = 'data/test.zip'
    enc_filename = 'data/enc_test.zip'
    dec_filename = 'data/dec_test.zip'

    rsa = RSA(1024)

    with open(filename, "rb") as fl_read, open(enc_filename, "w") as enc_write:
        rsa.encrypt(fl_read, enc_write)

    with open(enc_filename, "r") as enc_read, open(dec_filename, "wb") as dec_write:
        rsa.decrypt(enc_read, dec_write)


if __name__ == "__main__":
    main()
