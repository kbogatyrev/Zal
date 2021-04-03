from ctypes import *

#
#  Data structures (they map C++ interfaces defined in MainLib)
#
class StWordForm(Structure):
    """
   struct StWordForm {

        long long llLexemeId;
        wchar_t szWordForm[MAX_SIZE];
        wchar_t szStem[MAX_SIZE];
        ET_PartOfSpeech ePos;
        ET_Case eCase;
        ET_Number eNumber;
        ET_Subparadigm eSubparadigm;
        ET_Gender eGender;
        ET_Person ePerson;
        ET_Animacy eAnimacy;
        ET_Reflexive eReflexive;
        ET_Aspect eAspect;
        ET_Status eStatus;
        bool bIrregular;      // came from the DB as opposed to being generated by the app
        wchar_t* szLeadComment[MAX_SIZE];
        wchar_t* szTrailingComment[MAX_SIZE];
        bool bIsEdited;
        bool bIsVariant;
        char arrStress[MAX_SIZE] = { 0 };
        wchar_t szGrammHash[MAX_SIZE];
    };
    """

    _fields_ = [("llLexemeId", c_longlong), \
                ("szWordForm", type(create_unicode_buffer(1000))), \
                ("szStem", type(create_unicode_buffer(1000))), \
                ("ePos", c_int), \
                ("eCase", c_int), \
                ("eNumber", c_int), \
                ("eSubparadigm", c_int), \
                ("eGender", c_int), \
                ("ePerson", c_int), \
                ("eAnimacy", c_int), \
                ("eReflexive", c_int), \
                ("eAspect", c_int), \
                ("eStatus", c_int), \
                ("bIrregular", c_bool), \
                ("szLeadComment", type(create_unicode_buffer(1000))), \
                ("szTrailingComment", type(create_unicode_buffer(1000))), \
                ("bIsEdited", c_bool), \
                ("bIsVariant", c_bool), \
                ("szStress", type(create_unicode_buffer(1000))), \
                ("szGrammHash", type(create_unicode_buffer(1000)))]

#
#  Parser
#
class Parser:
    def __init__(self, lib_path, db_path):

        try:
            self.zal_lib = cdll.LoadLibrary(lib_path)
            if self.zal_lib is None:
                return False

            ret = self.zal_lib.Init(db_path)
            if not ret:
                return False

            self.parses = []

        except Exception as e:
            print(e)

    # Get a word without accents, send to Zal parser

    def parse_word_form(self, input_word):

        if self.zal_lib is None:
            print('Parser library was not initialized. Exiting.')
            return False

        ret = self.zal_lib.bParseWord(input_word)
        if not ret:
            return False

        for iIdx in range(self.zal_lib.iNumOfParses()-1):
            parse = StWordForm()

            ret = self.zal_lib.GetParsedWordForm(iIdx, byref(parse))

            print('wordform = {}'.format(parse.szWordForm))
            print(parse.szStress)
            print('descriptor = {}'.format(parse.szGrammHash))
            print('\r\n')

            test = 0

    #  parse_word_form

if __name__== "__main__":

    lib_path = '../x64/Debug/MainLibCTypes.dll'
    db_path = '../ZalData/ZalData_test.db3'

    parser = Parser(lib_path, db_path)
    parser.parse_word_form('шедшего')

