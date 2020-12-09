import rsa
from hashlib import sha256


def hash_file(filename):
    h = sha256()
    with open(filename, "rb") as f:
        while True:
            buf = f.read(1024)
            if len(buf) == 0:
                break
            h.update(buf)
    return h.digest()


def make_signature(filename, private_key):
    h = hash_file(filename)

    signature = rsa.encrypt(h, private_key)

    signature_filename = 'signature_' + filename
    with open(signature_filename, "wb") as f:
        f.write(signature)


def check_signature(filename, signature_file, public_key):
    h1 = hash_file(filename)

    with open(signature_file, "rb") as f:
        signature = f.read()

    try:
        h2 = rsa.decrypt(signature, public_key)
    except rsa.pkcs1.DecryptionError:
        return False

    return h1 == h2


def main():
    filename = 'test'
    (private_key, open_key) = rsa.newkeys(1024)

    make_signature(filename, private_key)
    print("Digital Signature is done")

    signature_filename = 'signature_test'
    is_verified = check_signature(filename, signature_filename, open_key)

    if is_verified:
        print("Digital Signature is verified")
    else:
        print("Digital Signature is not verified")


if __name__ == '__main__':
    main()
