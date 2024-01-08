import pyaes, pbkdf2, binascii, os, secrets, sys

# Derive a 256-bit AES encryption key from the password
password = sys.argv[1]
passwordSalt = os.urandom(16)
key = pbkdf2.PBKDF2(password, passwordSalt).read(16)


plaintext = 'HolaHolaHola\0\0\0\0'
print('AES encryption key:', binascii.hexlify(key,sep=' '))

aes = pyaes.AESModeOfOperationECB(key)
ciphertext = aes.encrypt(plaintext)

print('Encrypted:', binascii.hexlify(ciphertext))

print('AES encryption key to export:')
bytes_data = key

for char_value in bytes_data:
    print(f'0x{char_value:02x}, ',end='')


