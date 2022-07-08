
# -*- coding: utf-8 -*-

from . static import Base_RM_Field


class RM_Field_CW_CWCFG1_MODULATION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG1_MODULATION, self).__init__(register,
            'MODULATION', 'CW.CWCFG1.MODULATION', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_CW_CWCFG1_DUAL_PATH(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG1_DUAL_PATH, self).__init__(register,
            'DUAL_PATH', 'CW.CWCFG1.DUAL_PATH', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_CW_CWCFG1_SIG_TYPE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG1_SIG_TYPE, self).__init__(register,
            'SIG_TYPE', 'CW.CWCFG1.SIG_TYPE', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_CW_CWCFG2_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG2_FREQ, self).__init__(register,
            'FREQ', 'CW.CWCFG2.FREQ', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_CW_CWCFG2_AMP(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG2_AMP, self).__init__(register,
            'AMP', 'CW.CWCFG2.AMP', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_CW_CWCFG3_XTAL_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_CW_CWCFG3_XTAL_FREQ, self).__init__(register,
            'XTAL_FREQ', 'CW.CWCFG3.XTAL_FREQ', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


