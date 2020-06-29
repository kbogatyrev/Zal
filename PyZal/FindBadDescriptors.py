import logging
import re
import sys
import sqlite3

if __name__== "__main__":

        db_path = '../ZalData/ZalData_06_17_2020_Merge.db3'
        try:
            db_connect = sqlite3.connect(db_path)
            db_cursor = db_connect.cursor()
        except Exception as e:
            print(e)

        main_symbols = [ "м", "мо", "ж", "жо", "с", "со", "мо-жо", "мн.", "мн. неод.", "мн. одуш.", "мн. _от_", "п", "мс", "мс-п", "числ.", "числ.-п", "св", "нсв", "св-нсв", "н", "предл.", "союз", "предик.", "вводн.", "сравн.", "част.", "межд."]

        query = 'SELECT id, graphic_stem, main_symbol, part_of_speech, inflection_type FROM descriptor'

        db_cursor.execute(query)
        result_rows = db_cursor.fetchall()

        for row in result_rows:
            if not (row[2] in main_symbols) or row[3] < 0:
                print (row)