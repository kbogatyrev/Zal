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
    garm_hash = ''
    pos = ''

class Handler:

    def __init__(self, db_path):

        self.db_connect = None
        self.db_cursor = None
        self.words_query = 'SELECT tg.source, tg.gram_hash, tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, tg.id, lit.source, tg.first_word_position, lit.number_of_words, txt.name, lit.line_number \
FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id INNER JOIN text AS txt ON txt.id = lit.text_id ORDER BY tg.source ASC'
#        self.syll_count_query = 'SELECT tg.source, tg.gram_hash, lit.source, tg.first_word_position, txt.name, lit.line_number FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id \
#INNER JOIN text AS txt ON txt.id = lit.text_id WHERE num_of_syllables = {} ORDER BY lit.source ASC'
#        self.syll_count_query_with_stress = 'SELECT tg.source, tg.gram_hash, lit.source, tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, tg.first_word_position, txt.name, lit.line_number, lit.number_of_words FROM tact_group AS tg INNER JOIN \
#lines_in_text AS lit ON tg.line_id = lit.id INNER JOIN text AS txt ON txt.id = lit.text_id WHERE num_of_syllables = {0} AND stressed_syllable = {1}'

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
                data.line_source = row[6]
                data.position = row[7]
                data.text_name = row[9]
                data.line_number = row[10]
                data.is_last_word = '*' if row[7] + 1 == row[8] else ''

                data.pos = data.gram_hash.split('_')[0]

                self.data_dict[row[5]] = data

            out_file = open(output_path, "w", encoding='utf16')
            out_data = None
            pos_tags = []
            for key in sorted(self.data_dict.keys()):
                cur_data = self.data_dict[key]
                if out_data == None:
                    out_data = cur_data
                else:
                    if (cur_data.source_with_stress == out_data.source_with_stress and
                        cur_data.num_of_syllables == out_data.num_of_syllables and
                        cur_data.stressed_syllable == out_data.stressed_syllable and
                        cur_data.reverse_stressed_syllable == out_data.reverse_stressed_syllable and
                        cur_data.line_source == out_data.line_source and
                        cur_data.position == out_data.position and
                        cur_data.text_name == out_data.text_name and
                        cur_data.line_number == out_data.line_number and
                        cur_data.is_last_word == out_data.is_last_word):
                            if not(cur_data.pos in pos_tags):
                                if len(cur_data.pos) > 0:
                                    pos_tags.append(cur_data.pos)
                    else:
                        pos_tags.sort()
                        pos_string = ''
                        for tag in pos_tags:
                            if len(pos_string) > 0:
                                pos_string += '/'
                            pos_string += tag

                        out_file.write ('{0} | {1} | {2} | {3} | {4} | {5} | {6} | {7} | {8} \n'.format(out_data.source_with_stress, pos_string, out_data.num_of_syllables, out_data.stressed_syllable, \
out_data.reverse_stressed_syllable, out_data.is_last_word, out_data.text_name, out_data.line_number, out_data.line_source))
                        out_data = cur_data
                        pos_tags = [out_data.pos]

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
    db_path = '../ZalData/ZalData_06_07_2020_Pasternak.db3'
    output_path = '../ZalData/Dasha_06_09_2020.csv'
    h = Handler(db_path)
#    h.get_words_by_stress_pos()
#    h.get_words_by_num_of_syllables(3)
    h.get_words()

    k = 0
   