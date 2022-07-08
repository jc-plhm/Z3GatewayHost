
# -*- coding: utf-8 -*-

from . static import Base_RM_Field


class RM_Field_SUNOQPSK_SUNOQPSKCFG1_MODULATION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOQPSK_SUNOQPSKCFG1_MODULATION, self).__init__(register,
            'MODULATION', 'SUNOQPSK.SUNOQPSKCFG1.MODULATION', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOQPSK_SUNOQPSKCFG1_CHIPRATE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOQPSK_SUNOQPSKCFG1_CHIPRATE, self).__init__(register,
            'CHIPRATE', 'SUNOQPSK.SUNOQPSKCFG1.CHIPRATE', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOQPSK_SUNOQPSKCFG1_BANDFREQMHZ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOQPSK_SUNOQPSKCFG1_BANDFREQMHZ, self).__init__(register,
            'BANDFREQMHZ', 'SUNOQPSK.SUNOQPSKCFG1.BANDFREQMHZ', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOQPSK_SUNOQPSKCFG2_MACFCSTYPE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOQPSK_SUNOQPSKCFG2_MACFCSTYPE, self).__init__(register,
            'MACFCSTYPE', 'SUNOQPSK.SUNOQPSKCFG2.MACFCSTYPE', 'read-write',
            u"",
            0, 1)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOQPSK_SUNOQPSKCFG2_XTAL_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOQPSK_SUNOQPSKCFG2_XTAL_FREQ, self).__init__(register,
            'XTAL_FREQ', 'SUNOQPSK.SUNOQPSKCFG2.XTAL_FREQ', 'read-write',
            u"",
            8, 16)
        self.__dict__['zz_frozen'] = True


