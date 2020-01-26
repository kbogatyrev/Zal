from ctypes import *
import logging
import re
import sys

#
# Helper class
#
if __name__== "__main__":

    lib_path = '../x64/Debug/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_test.db3'
    text_path = "../ZalData/Pasternak_01_12_2020.txt"

    zal_lib = cdll.LoadLibrary(lib_path)
    if zal_lib is None:
        print('Unable to load Zal library.')
        exit(0)

    ret = zal_lib.Init(db_path)
    if not ret:
       print('Unable to initialize Zal library.')
       exit(0)

    author = None
    collection = None
    book = None
    title = None
    date = None

    texts = []
    text = ''

    last_line = ''

    try:
        with open (text_path, encoding='utf-16', mode='r') as reader:
            for line_num, line in enumerate(reader):
                line = line.rstrip()
                if not line:
                    continue
                match = re.match(r'^\<(\w+?)\s+(.+?)\/(\w+)>', line)
                if match != None:
                    start_tag = match.group(1)
                    value = match.group(2)
                    end_tag = match.group(3)

                    if start_tag != end_tag:
                        logging.error('Tag mismatch: %s', line)
                        continue

                    tag_name = start_tag

                    if 'author' == tag_name:
                        author = value
                    elif 'collection' == tag_name:
                        collection = value
                    elif 'book' == tag_name:
                        book = value
                    elif 'title' == tag_name:
                        if title:
                            title += ':'
                        title = value
                    elif 'date' == tag_name:
                        date = value
                    else:
                        print('Unable to parse tag: {0} in: {1}'.format(tag_name, line))
                else:
                    if last_line:
                        if last_line[0] == '<':
                            metadata = 'author = {0} | collection = {1} | book = {2} | title = {3} | date = {4}'.format(author, collection, book, title, date)
                            if text:
                                last_text_id = zal_lib.llParseText(title, metadata, text)
                                text = ''
                    if text:
                        text += '\r\n'
                    text += line

                last_line = line

    except Exception as e:
        logging.error ('Exception: %s', e)
        print(text)

