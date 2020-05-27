import logging
import re
import sys
import sqlite3

class Data:
    source_with_stress = ''
    position = -1
    text = ''
    line_number = -1
    words_in_line = -1
    line_source = ''

class Handler:

    def __init__(self, db_path):

        self.db_connect = None
        self.db_cursor = None
        self.words_query = 'SELECT tg.source, tg.gram_hash, tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, lit.source, tg.first_word_position, lit.number_of_words, txt.name, lit.line_number \
FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id INNER JOIN text AS txt ON txt.id = lit.text_id ORDER BY tg.source ASC'
        self.syll_count_query = 'SELECT tg.source, tg.gram_hash, lit.source, tg.first_word_position, txt.name, lit.line_number FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id \
INNER JOIN text AS txt ON txt.id = lit.text_id WHERE num_of_syllables = {} ORDER BY lit.source ASC'
        self.syll_count_query_with_stress = 'SELECT tg.source, tg.gram_hash, lit.source, tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, tg.first_word_position, txt.name, lit.line_number, lit.number_of_words FROM tact_group AS tg INNER JOIN \
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

    def get_words(self):
        if not self.ready:
            print('Error: Handler not initialized.')
            return
        try:
            query = self.words_query
            self.db_cursor.execute(query)
            result_rows = self.db_cursor.fetchall()
            self.data_dict = {}

            for row in result_rows:
                data = Data()
                data.source_with_stress = row[0]
                data.gram_hash = row[1]
                data.num_of_syllables = row[2]
                data.stressed_syllable = row[3] + 1
                data.reverse_stressed_syllable = row[4] + 1
                data.line_source = row[5]
                data.position = row[6]
                data.text_name = row[8]
                data.line_number = row[9]
                data.is_last_word = '*' if row[6] + 1 == row[7] else ''
                self.data_dict[row[0]+row[1]] = data

            out_file = open(output_path, "w", encoding='utf16')
            for key in sorted(self.data_dict.keys()):
                data = self.data_dict[key]
                out_file.write ('{0} | {1} | {2} | {3} | {4} | {5} | {6} | {7} | {8} \n'.format(data.source_with_stress, data.gram_hash, data.num_of_syllables, data.stressed_syllable, \
data.reverse_stressed_syllable, data.is_last_word, data.text_name, data.line_number, data.line_source))

        except Exception as e:
            print('Exception: {}'.format(e))


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
                data.words_in_line = row[5]

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
            for num_of_syllables in range (3, 6):
                for stress_pos in range (0, num_of_syllables):
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
                        data.words_in_line = row[5]

                        if data.words_in_line - 1 == data.position:
                            self.data_dict[row[0]] = data

                    self.sort_by_word()

        except Exception as e:
            print('Exception: {}'.format(e))

if __name__== "__main__":
    db_path = '../ZalData/ZalData_05_14_2020_Pasternak.db3'
    output_path = '../ZalData/Dasha_05_27_2020.csv'
    h = Handler(db_path)
#    h.get_words_by_stress_pos()
#    h.get_words_by_num_of_syllables(3)
    h.get_words()

    k = 0
   