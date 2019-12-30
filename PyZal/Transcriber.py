from ctypes import *
import json
import logging

#
#  Transcription
#
class Transcriber:

    def __init__(self, lib_path, db_path, json_path):

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

    lib_path = '../x64/Release/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_test.db3'

    t = Transcriber(lib_path, db_path, 'C:/git-repos/Zal-Windows/ZalData/TranscriptionRules.json')
    t.load_rules()

