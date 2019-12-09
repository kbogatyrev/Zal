import logging
import re
import sys

#
#  Analyzer
#
class Analyzer:

    def __init__(self, lib_path):
        self.lib_path = None
        self.data_path = None
        self.text = []
        self.collection = None
        self.book = None
        self.title = None
        self.date = None

    def handle_metadata_line(self, tag_name, text):
        if 'collection' == tag_name:
            self.collection = text
            return True
        elif 'book' == tag_name:
            self.book = text
            return True
        elif 'title' == tag_name:
            self.title = text
            return True
        elif 'date' == tag_name:
            self.date = text
            return True
        else:
            return False

        return False

    def read(self, data_path):
        try:
            with open (data_path, encoding='utf-16', mode='r') as reader:
                for line in enumerate(reader):
                    line = reader.readline(1000)
                    match = re.match(r"^\<(.+?)\s+(.+)/(.+)>", line)

                    if match != None:
                        start_tag = match.group(1)
                        text = match.group(2)
                        end_tag = match.group(3)

                        if start_tag != end_tag:
                            logging.error ('Tag mismatch: %', line)
                            continue

                        if self.handle_metadata_line(start_tag, text) != True:
                            logging.error('Unable to parse metadata: %', line)

                    self.text.append(line)
        except Exception as e:
            logging.error ('Exception: %s', e)

        print(self.text)
        removeMe = 0

if __name__== "__main__":

    lib_path = '../x64/Release/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_test.db3'

    a = Analyzer(lib_path)
    a.read("../ZalData/Pasternak.txt")

