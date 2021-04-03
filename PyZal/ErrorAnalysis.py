import argparse

if __name__== "__main__":
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('in_path', help='Path to the Zal log file')
    arg_parser.add_argument('out_path', help='Path to the output file')

    cmd_line_args = arg_parser.parse_args()

    log_file = open(cmd_line_args.in_path, 'r', encoding='utf-16-le')

    line_list = log_file.readlines()

    lstTuples = []
    for line in line_list:
        segments = line.split('\t')
        if len(segments) < 5:
            continue
        sort_prefix = str(segments[-3]) + '_' + str(segments[-4])
        lstTuples.append((sort_prefix, line))

    lstTuples.sort(key=lambda entry: entry[0])

    with open(cmd_line_args.out_path, 'w', encoding='utf-16-le') as out_file:
        for t in lstTuples:
            out_file.write(str(t[1]))

    print ("Done.")