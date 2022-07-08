
# -*- coding: utf-8 -*-

from . static import Base_RM_Register
from . CW_field import *


class RM_Register_CW_CWCFG1(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_CW_CWCFG1, self).__init__(rmio, label,
            0xb500fc00, 0x000,
            'CWCFG1', 'CW.CWCFG1', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.MODULATION = RM_Field_CW_CWCFG1_MODULATION(self)
        self.zz_fdict['MODULATION'] = self.MODULATION
        self.DUAL_PATH = RM_Field_CW_CWCFG1_DUAL_PATH(self)
        self.zz_fdict['DUAL_PATH'] = self.DUAL_PATH
        self.SIG_TYPE = RM_Field_CW_CWCFG1_SIG_TYPE(self)
        self.zz_fdict['SIG_TYPE'] = self.SIG_TYPE
        self.__dict__['zz_frozen'] = True


class RM_Register_CW_CWCFG2(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_CW_CWCFG2, self).__init__(rmio, label,
            0xb500fc00, 0x004,
            'CWCFG2', 'CW.CWCFG2', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.FREQ = RM_Field_CW_CWCFG2_FREQ(self)
        self.zz_fdict['FREQ'] = self.FREQ
        self.AMP = RM_Field_CW_CWCFG2_AMP(self)
        self.zz_fdict['AMP'] = self.AMP
        self.__dict__['zz_frozen'] = True


class RM_Register_CW_CWCFG3(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_CW_CWCFG3, self).__init__(rmio, label,
            0xb500fc00, 0x008,
            'CWCFG3', 'CW.CWCFG3', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.XTAL_FREQ = RM_Field_CW_CWCFG3_XTAL_FREQ(self)
        self.zz_fdict['XTAL_FREQ'] = self.XTAL_FREQ
        self.__dict__['zz_frozen'] = True


