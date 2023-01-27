from argparse import ArgumentParser

algorithm_choices = ('base64','crc','xtea','checksum')



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


if __name__ == '__main__':
    main()