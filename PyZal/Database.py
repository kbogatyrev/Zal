import argparse
from ctypes import *

#
#  DB maintenance using MainLib
#
class Database:
    def __init__(self, lib_path, db_path):
        try:
            self.zal_lib = cdll.LoadLibrary(lib_path)
            if self.zal_lib is None:
                return False

            ret = self.zal_lib.Init(db_path)
            if not ret:
                return False

        except Exception as e:
            print(e)

    def add_lexeme_hashes(self):
        self.zal_lib.AddLexemeHashes()

    def generate_all_forms(self):
        self.zal_lib.GenerateAllForms()

if __name__ == '__main__':

    lib_path = '../x64/Release/MainLibCTypes.dll'
    db_path = 'C:\git-repos\Zal-Windows\ZalData\ZalData_TEST.db3'
    db = Database(lib_path, db_path)
    db.add_lexeme_hashes()
    db.generate_all_forms()
