import argparse
import os
import pbkdf2
import binascii
import pyaes

# Parse command-line arguments
parser = argparse.ArgumentParser(description='AES Encryption')
parser.add_argument('--plaintext', required=True, help='Input plaintext')
parser.add_argument('--pwd', required=True, help='Password for key derivation')
parser.add_argument('--AESlen', required=True, choices=['AES-128', 'AES-192', 'AES-256'], help='AES key length')
parser.add_argument('--AESmode', required=True, choices=['ECB', 'CBC'], help='AES mode of operation')
args = parser.parse_args()

# Derive AES encryption key from the password
match args.AESlen:
    case 'AES-128':
        KEY_LEN = 16
    case 'AES-192':
        KEY_LEN = 24
    case 'AES-256':
        KEY_LEN = 32

passwordSalt = os.urandom(KEY_LEN)
key = pbkdf2.PBKDF2(args.pwd, passwordSalt).read(KEY_LEN)
key = '12345678901234567890123456789012'.encode('ascii')
# Print some information
print('Plaintext to hex:', binascii.hexlify(str.encode(args.plaintext), sep=' '))
print('AES encryption key:', binascii.hexlify(key, sep=' '))

# Choose the AES mode of operation
match args.AESmode:
    case 'ECB':
        aes = pyaes.AESModeOfOperationECB(key)
    case 'CBC':
        aes = pyaes.AESModeOfOperationCBC(key=key, iv=None)

# Manually pad with zeros for ECB mode
if args.AESmode == 'ECB':
    block_size = 16
    remainder = len(args.plaintext) % block_size
    if remainder != 0:
        zeros_to_add = block_size - remainder
        plaintext = args.plaintext + '\0' * zeros_to_add
    else:
        plaintext = args.plaintext

# Print the length of the plaintext after padding
print('Length of plaintext (with padding):', len(plaintext))

# Encrypt the plaintext
ciphertext = aes.encrypt(plaintext)


print('Encrypted:', binascii.hexlify(ciphertext))

print('AES encryption key to export:')
bytes_data = key

for char_value in bytes_data:
    print(f'0x{char_value:02x}, ',end='')

decrypted = aes.decrypt(ciphertext=ciphertext).decode('utf-8')
print(f'\nDecrypted: { decrypted}')
