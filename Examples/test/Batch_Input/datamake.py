import sys
import csv

def csv_to_in(csv_path):
    index = 0
    with open(csv_path, 'r') as csv_file:
        datas = csv.reader(csv_file, delimiter=',')
        for data in datas:
            print(data)
            data = data[1:]
            assert (len(data) == 28 * 28)
            with open("{}.in".format(index),"w") as single_file:
                for i in range(28):
                    for j in range(28):
                        single_file.write(str(round(float(data[i*28+j])*255)))
                        single_file.write("\n" if j == 27 else " ")
                index = index + 1

def main(argv):
    assert(len(argv) == 1)
    csv_to_in(argv[0])

if __name__ == "__main__":
    main(sys.argv[1:])
