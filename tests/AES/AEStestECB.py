import pyaes, pbkdf2, binascii, os, secrets, sys

# Derive a AES encryption key from the password
password = sys.argv[1]
aes_std = sys.argv[2]

match aes_std:
    case 'AES-128':
        KEY_LEN = 16
    case 'AES-192':
        KEY_LEN = 24
    case 'AES-256':
        KEY_LEN = 32
    case _:
        raise ValueError('Invalid AES KEY_LEN')

passwordSalt = os.urandom(KEY_LEN)
key = pbkdf2.PBKDF2(password, passwordSalt).read(KEY_LEN)


plaintext = 'HolaHolaHola\0\0\0\0'
print('AES encryption key:', binascii.hexlify(key,sep=' '))

aes = pyaes.AESModeOfOperationECB(key)
ciphertext = aes.encrypt(plaintext)

print('Encrypted:', binascii.hexlify(ciphertext))

print('AES encryption key to export:')
bytes_data = key

for char_value in bytes_data:
    print(f'0x{char_value:02x}, ',end='')


