import logging
import re
import sys
import sqlite3

class Data:
    source_with_stress = ''
    position = -1
    text = ''
    line_number = -1
    reverse_line_number = -1

class Handler:

    def __init__(self, db_path):

        self.db_connect = None
        self.db_cursor = None
        self.syll_count_query = 'SELECT tg.source, lit.source, tg.first_word_position, txt.name, lit.line_number FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id \
INNER JOIN text AS txt ON txt.id = lit.text_id WHERE num_of_syllables = {} ORDER BY lit.source ASC'
        self.syll_count_query_with_stress = 'SELECT tg.source, lit.source, tg.first_word_position, txt.name, lit.line_number, tg.reverse_stressed_syllable FROM tact_group AS tg INNER JOIN \
lines_in_text AS lit ON tg.line_id = lit.id INNER JOIN text AS txt ON txt.id = lit.text_id WHERE num_of_syllables = {0} AND stressed_syllable = {1}'

        self.data_dict = {}

        self.ready = False

        try:
            self.db_connect = sqlite3.connect(db_path)
            self.db_cursor = self.db_connect.cursor()
            self.ready = True

        except Exception as e:
            self.ready = False
            print(e)


    def get_words_by_num_of_syllables(self, num_of_syllables):
        if not self.ready:
            print('Error: Handler not initialized.')
            return
        try:
            query = self.syll_count_query.format(num_of_syllables)
            self.db_cursor.execute(query)
            result_rows = self.db_cursor.fetchall()
            for row in result_rows:
                data = Data()
                data.source_with_stress = row[0]
                data.position = row[2]
                data.text = row[3]
                data.line_number = row[4]
                self.data_dict[row[0]] = data

        except Exception as e:
            print('Exception: {}'.format(e))

    def sort_by_word(self):
        if not self.ready:
            print('Error: Handler not initialized.')
            return

        for key in sorted(self.data_dict.keys()):
            value = self.data_dict[key]
            print('{0}| {1}| {2}| {3}'.format(key, value.text, value.line_number, value.position))

    def get_words_by_stress_pos(self):
        if not self.ready:
            print('Error: Handler not initialized.')
            return
#        tg.source, lit.source, tg.first_word_position, txt.name, lit.line_number, tg.reverse_stressed_syllable
        try:
            for num_of_syllables in range (3,6):
                for stress_pos in range (0,num_of_syllables):
                    query = self.syll_count_query_with_stress.format(num_of_syllables, stress_pos)
                    self.db_cursor.execute(query)
                    result_rows = self.db_cursor.fetchall()
                    self.data_dict = {}
                    for row in result_rows:
                        data = Data()
                        data.source_with_stress = row[0]  # tg.source
                        data.position = row[2]            # position in line
                        data.text = row[3]                # source
                        data.line_number = row[4]
                        data.reverse_line_number = row[5]

                        self.data_dict[row[0]] = data

                    self.sort_by_word()

                    kiki = 0

        except Exception as e:
            print('Exception: {}'.format(e))

if __name__== "__main__":

    db_path = '../ZalData/ZalData_04_18_2020_Pasternak.db3'
    h = Handler(db_path)
    h.get_words_by_stress_pos()
#    h.get_words_by_num_of_syllables(3)
#    h.sort_by_word()

    koko = 0
   