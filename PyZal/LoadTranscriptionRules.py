from ctypes import *
import json
import sqlite3
import logging

#
#  Transcription
#
class TranscriptionRules:

    def __init__(self, lib_path, db_path, json_path):

        self.vowel_map = {}

        try:
#            self.zal_lib = cdll.LoadLibrary(lib_path)
#            if self.zal_lib is None:
#                return False

#            ret = self.zal_lib.Init(db_path)
#            if not ret:
#                return False

            self.db_path = db_path
            self.json_path = json_path

        except Exception as e:
            print(e)

    def load_rules(self):

#  CREATE TABLE transcription_inputs (id INTEGER PRIMARY KEY, input_chars TEXT)
#  CREATE TABLE transcription_rules (id INTEGER PRIMARY KEY, input_id INTEGER, stress TEXT, left_contexts TEXT, \
#   right_contexts TEXT, target TEXT, morpheme_type TEXT, subparadigm TEXT, gramm_number TEXT, gramm_case TEXT, \
#   gramm_gender TEXT, strength TEXT, comment TEXT);

        db_path = '../ZalData/ZalData_05_14_2020_Pasternak.db3'
        db_connect = sqlite3.connect(db_path)
        db_cursor = db_connect.cursor()

        db_cursor.execute('DELETE FROM transcription_inputs')
        db_cursor.execute('DELETE FROM transcription_rules')

        insert_query_inputs = 'INSERT INTO transcription_inputs (input_chars) VALUES (?)'

        dict_rules = {}
        with open(self.json_path, mode='r', encoding='utf-8') as rules_file:
            decoder = json.JSONDecoder()
            dict_rules = decoder.decode(rules_file.read())

        for type in dict_rules.keys():
            rules_for_type = dict_rules[type]
            for inputs in rules_for_type.keys():
                rules = rules_for_type[inputs]

                db_cursor.execute(insert_query_inputs, (inputs,))
                inputs_id = db_cursor.lastrowid

                for rule in rules:
                    print (rule)
                    comment = rule.get('COMMENT')
                    stress = rule.get('STRESS')
                    l_contexts = rule.get('LEFT_CONTEXTS')
                    lst_l_contexts = ''
                    if l_contexts:
                        lst_l_contexts = ','.join(l_contexts)
                    r_contexts = rule.get('RIGHT_CONTEXTS')
                    lst_r_contexts = ''
                    if r_contexts:
                        lst_r_contexts = ','.join(r_contexts)
                    target = rule.get('TARGET')

                    lst_morphemes = ''
                    morphemes = rule.get('MORPHEMES')
                    if morphemes:
                        lst_morphemes = ','.join(morphemes)
                    strength = rule.get('STRENGTH') # recommended/not recommended, etc.
                    subparadigm = rule.get('SUBPARADIGM')
                    number = rule.get('NUMBER')
                    gender = rule.get('GENDER')
                    case = rule.get('CASE')

                    insert_query_rules = 'INSERT INTO transcription_rules (input_id, stress, left_contexts, right_contexts, \
                    target, morpheme_type, subparadigm, gramm_number, gramm_case, gramm_gender, strength, comment) \
                    VALUES (?,?,?,?,?,?,?,?,?,?,?,?)'
    #                       0 1 2 3 4 5 6 7 8 9 0 1 2
                    tuple_columns_rules = (inputs_id, stress, lst_l_contexts, lst_r_contexts, target, lst_morphemes, subparadigm, gender, number, case, strength, comment)

                    db_cursor.execute(insert_query_rules, tuple_columns_rules)

        db_connect.commit()

if __name__== "__main__":

    lib_path = '../x64/Release/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_05_14_2020_Pasternak.db3'

    t = TranscriptionRules(lib_path, db_path, 'C:/git-repos/Zal-Windows/ZalData/TranscriptionRules.json')
    t.load_rules()
