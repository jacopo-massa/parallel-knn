import subprocess
import argparse
import sys


def check_positive(value):
    i_value = int(value)
    if i_value <= 0:
        raise argparse.ArgumentTypeError("{}} is an invalid positive int value".format(value))
    return i_value


def check_version(vsn):
    if vsn not in ["ff", "stl"]:
        raise argparse.ArgumentTypeError("'version' must be equal to 'ff' or 'stl'".format(vsn))
    return vsn


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Calculate avg timings for KNN C++ implementation')
    parser.add_argument('version', help="'ff' or 'stl'", type=check_version)
    parser.add_argument('iterations', help='number of executions', type=int)
    parser.add_argument('k', help='K-hyperparameter for KNN', type=check_positive)
    parser.add_argument('workers', help='number of workers', type=check_positive)
    parser.add_argument('inputfile', help="name of a file in the subdirectory 'input'")
    args = parser.parse_args()

    version = "./" + args.version
    cmd = [version, str(args.k), str(args.workers), str(args.inputfile)]
    read_avg = knn_avg = 0
    for i in range(args.iterations):
        process = subprocess.run(cmd, stdout=subprocess.PIPE)
        t_res = process.stdout.decode('utf-8')
        t_res = t_res.split("\n", 2)

        read_avg += float(t_res[0].split(" ")[3])
        knn_avg += float(t_res[1].split(" ")[3])
        sys.stdout.write("\r")
        sys.stdout.flush()
        sys.stdout.write("Done {}/{}".format(i+1, args.iterations))

    read_avg /= 1000*args.iterations
    knn_avg /= 1000*args.iterations
    print("\n")
    print("READ avg: {} msec".format(read_avg))
    print("KNN avg: {} msec".format(knn_avg))
