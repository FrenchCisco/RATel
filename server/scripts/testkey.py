import binascii
from backports.pbkdf2 import pbkdf2_hmac

def generate_PBKDF2_key(password, salt="CISCOTHEBOSS",iteration=10000 ,length=512):
    password = password.encode()
    salt = salt.encode()

    return binascii.hexlify(pbkdf2_hmac("sha256", password, salt, iteration, length)).decode("utf8")

print(generate_PBKDF2_key("cisco"))
'''
source:
- https://cryptobook.nakov.com/mac-and-key-derivation/pbkdf2
- https://fr.wikipedia.org/wiki/PBKDF2#:~:text=Le%20PBKDF2%20(abr%C3%A9viation%20de%20Password,publi%C3%A9e%20dans%20la%20RFC%202898.
'''