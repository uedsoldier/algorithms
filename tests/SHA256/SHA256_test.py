from argparse import ArgumentParser
from hashlib import sha256
from binascii import hexlify

# Parse command-line arguments
parser = ArgumentParser(description='SHA-256 testing')
parser.add_argument('--plaintext', required=True, help='Input plaintext')
args = parser.parse_args()

plaintext = args.plaintext
plaintext_encoded = str.encode(plaintext)
# Print some information
print('Plaintext to hex:', hexlify(plaintext_encoded, sep=' '))

# Hash the plaintext
result = sha256(plaintext_encoded)
digest = result.hexdigest()

print('Hashed:',digest)

digest_bytes = bytearray.fromhex(digest)

print('SHA-256 hash to export as string:')
for char_value in digest_bytes:
    hex_repr = f'{char_value:02x}'
    print(f'\\x{hex_repr.upper()}',end='')