import os
#import cdll
from ctypes import *
import re
import logging
import sys
import json

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
                    line = reader.readline()
                    match = re.match(r"^\<(.+?)\s+(.+)/(.+)>", line)

                    if match != None:
                        start_tag = match.group(1)
                        text = match.group(2)
                        end_tag = match.group(3)

                        if start_tag != end_tag:
                            logging.error ('Tag mismatch: %', line)
                            continue

                        if self.handle_metadata_line(start_tag, text) != True:
                            logging.error('Unable to parse metadata: %s', line)

                    self.text.append(line)
        except Exception as e:
            logging.error ('Exception: %s', e)

        print(self.text)

#
#  DB maintenance
#
class Database:
    def __init__(self, lib_path, db_path):
        try:
            self.zal_lib = cdll.LoadLibrary(lib_path)
            if self.zal_lib is None:
                return False

            ret = self.zal_lib.Init(db_path)
            if not ret:
                return False

        except Exception as e:
            print(e)

    def add_lexeme_hashes(self):
        self.zal_lib.AddLexemeHashes()

#
#  Transcription
#
class Transcriber:
    # Insert a row of data
    c.execute("INSERT INTO stocks VALUES ('2006-01-05','BUY','RHAT',100,35.14)")

    # Save (commit) the changes
    conn.commit()

    # We can also close the connection if we are done with it.
    # Just be sure any changes have been committed or they will be lost.
    conn.close()
    def __init__(self, lib_path, db_path, json_path):

        self.insert_query = 'INSERT INTO transcription_rules_vowels VALUES tg.source, tg.gram_hash, \
        tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, lit.source, tg.first_word_position, \
        lit.number_of_words, txt.name, lit.line_number \
        FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id \
        INNER JOIN text AS txt ON txt.id = lit.text_id ORDER BY tg.source ASC'

        self.vowel_map = {}

        try:
            self.zal_lib = cdll.LoadLibrary(lib_path)
            if self.zal_lib is None:
                return False

            ret = self.zal_lib.Init(db_path)
            if not ret:
                return False

            self.db_path = db_path
            self.json_path = json_path

        except Exception as e:
            print(e)

    def load_rules(self):
        dict_rules = {}
        with open(self.json_path, mode='r', encoding='utf-8') as rules_file:
            decoder = json.JSONDecoder()
            dict_rules = decoder.decode(rules_file.read())

        # Test:
        vowels_rules = dict_rules['VOWELS']
        for vowel in vowels_rules:
            for rule in vowel.items():
#                self.vowel_map[rule.key] = rule.value
#                print (rule, end='\n')
                for vowel in rule:
                    for sub_rule in vowel:
                        print(sub_rule)
#                        for vvv in sub_rule:
#                            print (vvv, ' -> ', sub_rule[vvv])


if __name__== "__main__":

#    db_connection = sqlite3.connect ('../Zal-Windows/ZalData/ZalData.db3')
#    db_cursor = db_connection.cursor()

#    db_edited_connection = sqlite3.connect ('../Zal-Windows/ZalData/ZalData_Edited.db3')
#    db_edited_cursor = db_edited_connection.cursor()

    lib_path = '../x64/Release/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_04_18_2020_with_irreg.db3'

    t = Transcriber(lib_path, db_path, 'C:/git-repos/Zal-Windows/ZalData/TranscriptionRules.json')
    t.load_rules()

#    db = Database(lib_path, db_path)
#    db.add_lexeme_hashes()
#    zal_lib = cdll.LoadLibrary(lib_path)
 #   zal_lib.AddLexemeHashes()

#    parser = Parser(lib_path, db_path)
#    parser.parse_word_form('собака')

#    a = Analyzer(lib_path)
#    a.read("../ZalData/Pasternak.txt")

os._exit(0)
