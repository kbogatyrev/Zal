import logging
import re
import sys
import sqlite3
import time

class Data:
    tact_group_id = -1
    source_with_stress = ''
    first_word_position = -1
    text = ''
    line_number = -1
    words_in_line = -1
    line_source = ''
    pos = []

class Handler:

    def __init__(self, db_path):

        self.db_connect = None
        self.db_cursor = None
#                                     0       1               2                     3                   4                    5                        6                      7              8              9          10
        self.words_query = 'SELECT tg.id, tg.source, tg.first_word_position, tg.num_of_words, tg.num_of_syllables, tg.stressed_syllable, tg.reverse_stressed_syllable, lit.source, lit.number_of_words, txt.name, lit.line_number \
                            FROM tact_group AS tg INNER JOIN lines_in_text AS lit ON tg.line_id = lit.id INNER JOIN text AS txt ON txt.id = lit.text_id ORDER BY tg.source ASC' 

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

            print ('Query returned {} rows'.format(len(result_rows)))

            count = 0;
            for row in result_rows:
                data = Data()
                data.tact_group_id = row[0]
                data.source_with_stress = row[1]
                data.first_word_pos = row[2]
                data.num_of_words = row[3]
                data.num_of_syllables = row[4]
                data.stressed_syllable = row[5] + 1
                data.reverse_stressed_syllable = row[6] + 1
                data.line_source = row[7]
                data.text_name = row[9]
                data.line_number = row[10]
                data.is_last_word = '*' if row[2] + 1 == row[8] else ''

                data.pos = self.get_gram_hashes(data)

                self.data_dict[data.tact_group_id] = data

                count += 1
                if count%1000 == 0:
                    print(count)

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
                        cur_data.first_word_pos == out_data.first_word_pos and
                        cur_data.text_name == out_data.text_name and
                        cur_data.line_number == out_data.line_number and
                        cur_data.is_last_word == out_data.is_last_word):
                            if not(cur_data.pos in pos_tags):
                                if len(cur_data.pos) > 0:
                                    pos_tags.extend(cur_data.pos)
                                    pos_tags = list(set(pos_tags))
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
                        pos_tags = list(set(out_data.pos))

        except Exception as e:
            print('Exception: {}'.format(e))

    def get_gram_hashes(self, data):
        word_query = 'SELECT gram_hash FROM tact_group_to_gram_hash AS gh INNER JOIN tact_group AS tg ON gh.tact_group_id = tg.id WHERE tg.id = {};'.format(data.tact_group_id)
        self.db_cursor.execute(word_query)
        result_rows = self.db_cursor.fetchall()
        gram_hashes = []
        for row in result_rows:
            gram_hashes.append(row[0].split('_')[0])
        return list(set(gram_hashes))

    def sort_by_word(self):
        if not self.ready:
            print('Error: Handler not initialized.')
            return

        for key in sorted(self.data_dict.keys()):
            value = self.data_dict[key]
            print('{0}| {1}| {2}| {3}'.format(key, value.text, value.line_number, value.position))

if __name__== "__main__":
    db_path = '../ZalData/ZalData_Master_Pasternak_07_03_2021.db3'
    output_path = '../ZalData/Pasternak_07_03.csv'
    h = Handler(db_path)
    h.get_words()

    k = 0
   