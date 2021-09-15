import logging
import sqlite3

def num_of_syllables(text):
    num = 0
    for chr in text:
        if chr in 'аеёиоуыэюя':
            num += 1
    return num

def text_pos_to_syll_pos(text, pos):
    syll_count = 0
    for current_text_pos in range(0, pos+1):
        if text[current_text_pos] in 'аеёиоуыэюя':
            syll_count += 1
    return syll_count - 1

def syll_pos_to_text_pos(text, syll_pos):
    cur_syll = 0
    for current_text_pos in range(0, len(text)):
        if text[current_text_pos] in 'аеёиоуыэюя':
            if cur_syll == syll_pos:
                break
            cur_syll += 1
    return current_text_pos

if __name__== "__main__":

    db_path = 'C:\git-repos\Zal-Windows\ZalData\ZalData_Master.db3'
    db_connect = sqlite3.connect(db_path)
    db_hw_cursor = db_connect.cursor()

    hw_query = '''SELECT hw.source, d.id FROM headword AS hw INNER JOIN spryazh_sm_headwords AS sshw ON hw.id = sshw.headword_id INNER JOIN descriptor AS d ON 
                  sshw.ref_descriptor_id = d.id;'''
    db_hw_cursor.execute(hw_query)

    hw_rows = db_hw_cursor.fetchall()

    db_wf_cursor = db_connect.cursor()

    for hw_row in hw_rows:

        spryazh_sm_headword = hw_row[0]
        descriptor_id = hw_row[1]

        hw_query2 = f'SELECT hw2.source FROM headword AS hw2 INNER JOIN descriptor AS d ON d.word_id = hw2.id WHERE d.id = {descriptor_id}'
        db_hw_cursor.execute(hw_query2)
        ref_headword = db_hw_cursor.fetchone()[0]
        r_spryazh_sm_headword = spryazh_sm_headword[::-1]
        r_ref_headword = ref_headword[::-1]
        common_length = 0
        min_length = min(len(spryazh_sm_headword), len(ref_headword))
        while common_length < min_length:
            if r_spryazh_sm_headword[common_length] != r_ref_headword[common_length]:
                break
            common_length += 1
        
        if common_length > min_length:
            print('*** ERROR: unable to extract prefix')
            continue
        
        ref_prefix = ref_headword[0:len(ref_headword)-common_length]
        spryazh_sm_prefix = spryazh_sm_headword[0:len(spryazh_sm_headword)-common_length]

        stress_pos_diff = num_of_syllables(spryazh_sm_headword) - num_of_syllables(ref_headword)

        print (stress_pos_diff)

#        print(spryazh_sm_headword, spryazh_sm_prefix, ref_headword, ref_prefix)

        db_write_cursor = db_connect.cursor()

        wf_query = f'SELECT id, wordform, is_alternative, lead_comment, trailing_comment FROM irregular_forms WHERE descriptor_id = {descriptor_id}'
        db_wf_cursor.execute(wf_query)
        wf_rows = db_wf_cursor.fetchall()

        for wf_row in wf_rows:
            form_id = wf_row[0]
            wordform = wf_row[1]
            new_wordform = spryazh_sm_prefix + wordform[len(ref_prefix):]
            print(wordform, '-->', new_wordform)
            is_alternative = wf_row[2]
            lead_comment = wf_row[3]
            trailing_comment = wf_row[4]

            db_write_cursor.execute('INSERT INTO irregular_forms_spryazh_sm (descriptor_id, gram_hash, wordform, is_alternative, lead_comment, trailing_comment, is_edited) VALUES (?,?,?,?,?,?,?)')

            stress_query = f'SELECT * FROM irregular_stress WHERE form_id = {form_id}'
            db_wf_cursor.execute(stress_query)
            stress_rows = db_wf_cursor.fetchall()
            for stress_row in stress_rows:
                id = stress_row[0]
#               form_id = stress_row[1]
                stress_pos = stress_row[2]
                stress_position_syll = text_pos_to_syll_pos(wordform, stress_pos) + stress_pos_diff
                new_stress_pos = syll_pos_to_text_pos(new_wordform, stress_position_syll)
                is_primary = stress_row[3]
                is_edited = stress_row[4]

                print ('****   ' + str(stress_pos), str(new_stress_pos))
#            print(at)

    print ('Done.')