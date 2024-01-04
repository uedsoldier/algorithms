from argparse import ArgumentParser
from os import chdir,getcwd,system
from shutil import rmtree

algorithm_choices = ('BASE64','CRC','XTEA','CHECKSUM8')



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
    print(getcwd())
    rmtree('build/',ignore_errors=True)
    system('cmake -S . -B build/ -G "MinGW Makefiles"')
    chdir('build')
    system('mingw32-make')

if __name__ == '__main__':
    main()