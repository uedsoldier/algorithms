from argparse import ArgumentParser
from os import chdir,getcwd,system,mkdir,path
from shutil import rmtree

algorithm_choices = ('BASE64','CRC','XTEA','CHECKSUM8','AES','SHA1','PKCS7')

build_dir = 'build/'

def main():
    #Argument parsing
    CLI = ArgumentParser(add_help=True)
    CLI.add_argument(
        '-a','--algorithm',
        nargs=1,
        type=str,
        required=True,
        choices=algorithm_choices,
        help='Algorithm that will get its binary test file generated'
    )
    args = CLI.parse_args()
    algorithm = args.algorithm[0]
    print(f'Algorithm: {algorithm}')
    chdir(algorithm)
    if path.exists(build_dir):    
        rmtree(build_dir,ignore_errors=True)
    mkdir(build_dir)
    system('cmake -S . -B build/ -G "MinGW Makefiles"')
    chdir(build_dir)
    system('mingw32-make')

if __name__ == '__main__':
    main()