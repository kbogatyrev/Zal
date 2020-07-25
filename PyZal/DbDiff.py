import sqlite3


class Headword:
    def __init__(self):
        self.hw_source = ''
        self.hw_id = -1
        self.hw_comment = ''
        self.hw_variant_comment = ''
        self.hw_plural_of = ''
        self.hw_usage = ''
        self.hw_variant = ''
        self.hw_see_ref = ''
        self.hw_back_ref = ''
        self.hw_source_entry_id = -1

    def __eq__(self, other):
        if (self.hw_source == other.hw_source and
        self.hw_id == other.hw_id and
        self.hw_comment == other.hw_comment and
        self.hw_variant_comment == other.hw_variant_comment and
        self.hw_plural_of == other.hw_plural_of and
        self.hw_usage == other.hw_usage and
        self.hw_variant == other.hw_variant and
        self.hw_see_ref == other.hw_see_ref and
        self.hw_back_ref == other.hw_back_ref):
            return True
        return False

    def __ne__(self, other):
        return not self.__eq__(other)

class Descriptor:
    def __init__(self):
        d_id = -1
        d_graphic_stem = ''
        d_is_variant = ''
        d_main_symbol = ''
        d_part_of_speech = ''
        d_is_plural_of = ''
        d_is_intransitive = ''
        d_is_reflexive = ''
        d_main_symbol_plural_of = ''
        d_alt_main_symbol = ''
        d_inflection_type = ''
        d_comment = ''
        d_alt_main_symbol_comment = ''
        d_alt_inflection_comment = ''
        d_verb_stem_alternation = ''
        d_part_past_pass_zhd = ''
        d_section = ''
        d_no_comparative = ''
        d_assumed_forms = ''
        d_yo_alternation = ''
        d_o_alternation = ''
        d_second_genitive = ''
        d_is_impersonal = ''
        d_is_iterative = ''
        d_has_aspect_pair = ''
        d_difficult_and_missing_forms = ''
        d_has_irregular_forms = ''
        d_irregular_forms_lead_comment = ''
        d_has_deficiencies = ''
        d_restricted_contexts = ''
        d_contexts = ''
        d_cognate = ''
        d_trailing_comment = ''

    def __eq__(self, other):
        if (self.d_graphic_stem == other.d_graphic_stem and
        self.d_is_variant == other.d_is_variant and
        self.d_main_symbol == other.d_main_symbol and
        self.d_part_of_speech == other.d_part_of_speech and
        self.d_is_plural_of == other.d_is_plural_of and
        self.d_is_intransitive == other.d_is_intransitive and
        self.d_is_reflexive == other.d_is_reflexive and
        self.d_main_symbol_plural_of == other.d_main_symbol_plural_of and
        self.d_alt_main_symbol == other.d_alt_main_symbol and
        self.d_inflection_type == other.d_inflection_type and
        self.d_comment == other.d_comment and
        self.d_alt_main_symbol_comment == other.d_alt_main_symbol_comment and
        self.d_alt_inflection_comment == other.d_alt_inflection_comment and
        self.d_verb_stem_alternation == other.d_verb_stem_alternation and
        self.d_part_past_pass_zhd == other.d_part_past_pass_zhd and
        self.d_section == other.d_section and
        self.d_no_comparative == other.d_no_comparative and
        self.d_assumed_forms == other.d_assumed_forms and
        self.d_yo_alternation == other.d_yo_alternation and
        self.d_o_alternation == other.d_o_alternation and
        self.d_second_genitive == other.d_second_genitive and
        self.d_is_impersonal == other.d_is_impersonal and
        self.d_is_iterative == other.d_is_iterative and
        self.d_has_aspect_pair == other.d_has_aspect_pair and
        self.d_difficult_and_missing_forms == other.d_difficult_and_missing_forms and
        self.d_irregular_forms_lead_comment == other.d_irregular_forms_lead_comment and
        self.d_has_deficiencies == other.d_has_deficiencies and
        self.d_restricted_contexts == other.d_restricted_contexts and
        self.d_contexts == other.d_contexts and
        self.d_cognate == other.d_cognate and
        self.d_trailing_comment == other.d_trailing_comment):
            return True
        return False

class Inflection:
    def __init__(self):
        i_id = -1
        i_is_primary = ''
        i_inflection_type = ''
        i_accent_type1 = ''
        i_accent_type2 = ''
        i_short_form_restrictions = ''
        i_past_part_restrictions = ''
        i_no_short_form = ''
        i_no_past_part = ''
        i_fleeting_vowel = ''
        i_stem_augment = ''
        i_inflected_parts = ''


    def __eq__(self, other):
        if (self.i_is_primary == other.i_is_primary and
            self.i_inflection_type == other.i_inflection_type and
            self.i_accent_type1 == other.i_accent_type1 and
            self.i_accent_type2 == other.i_accent_type2 and
            self.i_short_form_restrictions == other.i_short_form_restrictions and
            self.i_past_part_restrictions == other.i_past_part_restrictions and
            self.i_no_short_form == other.i_no_short_form and
            self.i_no_past_part == other.i_no_past_part and
            self.i_fleeting_vowel == other.i_fleeting_vowel and
            self.i_stem_augment == other.i_stem_augment and
            self.i_inflected_parts == other.i_inflected_parts):
                return True
        return False


IrregularForms = []


def get_rows(result_rows):

    lstHeadword = []
    lstDescriptor = []
    lstInflection = []

    count = 0
    for row in result_rows:
        count  += 1

        h = Headword()
        d = Descriptor()
        i = Inflection()

        h.hw_source = row[0]
        h.hw_id = row[1]
        h.hw_comment = row[2]
        h.hw_variant_comment = row[3]
        h.hw_plural_of = row[4]
        h.hw_usage = row[5]
        h.hw_variant = row[6]
        h.hw_see_ref = row[7]
        h.hw_back_ref = row[8]
        h.hw_source_entry_id = row[9]
        lstHeadword.append(h)

        d.d_id = row[10]
        d.d_graphic_stem = row[11]
        d.d_is_variant = row[12]
        d.d_main_symbol = row[13]
        d.d_part_of_speech = row[14]
        d.d_is_plural_of = row[15]
        d.d_is_intransitive = row[16]
        d.d_is_reflexive = row[17]
        d.d_main_symbol_plural_of = row[18]
        d.d_alt_main_symbol = row[19]
        d.d_inflection_type = row[20]
        d.d_comment = row[21]
        d.d_alt_main_symbol_comment = row[22]
        d.d_alt_inflection_comment = row[23]
        d.d_verb_stem_alternation = row[24]
        d.d_part_past_pass_zhd = row[25]
        d.d_section = row[26]
        d.d_no_comparative = row[27]
        d.d_assumed_forms = row[28]
        d.d_yo_alternation = row[29]
        d.d_o_alternation = row[30]
        d.d_second_genitive = row[31]
        d.d_is_impersonal = row[32]
        d.d_is_iterative = row[33]
        d.d_has_aspect_pair = row[34]
        d.d_difficult_and_missing_forms = row[35]
        d.d_has_irregular_forms = row[36]
        d.d_irregular_forms_lead_comment = row[37]
        d.d_has_deficiencies = row[38]
        d.d_restricted_contexts = row[39]
        d.d_contexts = row[40]
        d.d_cognate = row[41]
        d.d_trailing_comment = row[42]
        lstDescriptor.append(d)

        i.i_is_primary = row[43]
        i.i_is_primary = row[44]
        i.i_inflection_type = row[45]
        i.i_accent_type1 = row[46]
        i.i_accent_type2 = row[47]
        i.i_short_form_restrictions = row[48]
        i.i_past_part_restrictions = row[49]
        i.i_no_short_form = row[50]
        i.i_no_past_part = row[51]
        i.i_fleeting_vowel = row[52]
        i.i_stem_augment = row[53]
        i.i_inflected_parts = row[54]
        lstInflection.append(i)

#        lst.append((h, d, i))

    if not (len(lstHeadword) == len(lstDescriptor) == len(lstInflection)):
        print ('ERROR: lists have different sizes')

    return lstHeadword, lstDescriptor, lstInflection
#        print (h_L.hw_source)

if __name__=="__main__":

#                           0              1               2                3                         4
    query = ('SELECT headword.source, headword.id, headword.comment, headword.variant_comment, headword.plural_of, ' +
#               5                6                 7                 8                     9                  10
        'headword.usage, headword.variant, headword.see_ref, headword.back_ref, headword.source_entry_id, descriptor.id, ' +
#                 11                      12                     13                      14
        'descriptor.graphic_stem, descriptor.is_variant, descriptor.main_symbol, descriptor.part_of_speech,' +
#                 15                      16                            17                      18
        'descriptor.is_plural_of, descriptor.is_intransitive, descriptor.is_reflexive, descriptor.main_symbol_plural_of, ' +
#                 19                         20                          21                   22
        'descriptor.alt_main_symbol, descriptor.inflection_type, descriptor.comment, descriptor.alt_main_symbol_comment, ' +
#                  23                              24                                   25
        'descriptor.alt_inflection_comment, descriptor.verb_stem_alternation, descriptor.part_past_pass_zhd, ' +
#                 26                         27                      28                    29
        'descriptor.section, descriptor.no_comparative, descriptor.assumed_forms, descriptor.yo_alternation, ' +
#                 30                         31                        32                          33
        'descriptor.o_alternation, descriptor.second_genitive, descriptor.is_impersonal, descriptor.is_iterative, ' +
#                 34                          35                                         36
        'descriptor.has_aspect_pair, descriptor.difficult_and_missing_forms, descriptor.has_irregular_forms, ' +
#                  37                                   38                              39
        'descriptor.irregular_forms_lead_comment, descriptor.has_deficiencies, descriptor.restricted_contexts, '    +
#                  40                   41                  42                      43                  44
        'descriptor.contexts, descriptor.cognate, descriptor.trailing_comment, inflection.id, inflection.is_primary, ' +
#                    45                         46                        47                      48
        'inflection.inflection_type, inflection.accent_type1, inflection.accent_type2, inflection.short_form_restrictions, ' +
#                   49                                 50                      51                       52
        'inflection.past_part_restrictions, inflection.no_short_form, inflection.no_past_part, inflection.fleeting_vowel, ' +
#                  53                          54
        'inflection.stem_augment, inflection.inflected_parts ' +
#
        'FROM headword INNER JOIN descriptor ON descriptor.word_id = headword.id ' +
        'LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id  ' +
        'INNER JOIN conversion_source_text as cst ON cst.id = source_entry_id ORDER BY cst.guid;')

    db_connection_L = sqlite3.connect('../ZalData/ZalData_07_23_2020_TEST.db3')
    db_cursor_L = db_connection_L.cursor()

    db_connection_R = sqlite3.connect ('../ZalData/ZalData_07_20_2020_BAD_TEST.db3')
    db_cursor_R = db_connection_R.cursor()

    db_cursor_L.execute(query)
    result_rows_L = db_cursor_L.fetchall()
    lstH_L, lstD_L, lstI_L = get_rows(result_rows_L)

    db_cursor_R.execute(query)
    result_rows_R = db_cursor_R.fetchall()
    lstH_R, lstD_R, lstI_R = get_rows(result_rows_L)

    current_R = 0
    total_L = len(result_rows_L)

    for current_L in range(len(lstH_L)):
        found = False
        if lstH_L[current_L]== lstH_R[current_R] and lstD_L[current_L] == lstD_R[current_R] and lstI_L[current_L] == lstI_R[current_R]:
            current_R += 1
            found = True
            continue

        if not found:
            current_R = current_L
            while not found and current_R > 0 and current_L - current_R < 200:
                current_R -= 1
                if lstH_L[current_L] == lstH_R[current_R] and lstD_L[current_L] == lstD_R[current_R] and lstI_L[current_L] == lstI_R[current_R]:
                    found = True

        if not found:
            current_R = current_L
            while not found and current_R < len(result_rows_R) and current_R - current_L < 200:
                current_R += 1
                if lstH_L[current_L] == lstH_R[current_R] and lstD_L[current_L] == lstD_R[current_R] and lstI_L[current_L] == lstI_R[current_R]:
                    found = True

        if not found:
            print ('Error: {0}, entry ID = {1}'.format(lstH_L[current_L].hw_source, str(lstH_L[current_L].hw_source_entry_id)))
            current_R = current_L + 1
        else:
            current_R += 1
    print ('++++++ current_L {0}, current_R {1}'.format(current_L, current_R))
    print ('*** total (L) {}'.format (len(result_rows_L)))
