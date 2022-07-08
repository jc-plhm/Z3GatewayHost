
# -*- coding: utf-8 -*-

from . static import Base_RM_Field


class RM_Field_LEGOQPSK_LEGOQPSKCFG1_MODULATION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_LEGOQPSK_LEGOQPSKCFG1_MODULATION, self).__init__(register,
            'MODULATION', 'LEGOQPSK.LEGOQPSKCFG1.MODULATION', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_LEGOQPSK_LEGOQPSKCFG1_CHIPRATE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_LEGOQPSK_LEGOQPSKCFG1_CHIPRATE, self).__init__(register,
            'CHIPRATE', 'LEGOQPSK.LEGOQPSKCFG1.CHIPRATE', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_LEGOQPSK_LEGOQPSKCFG1_BANDFREQMHZ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_LEGOQPSK_LEGOQPSKCFG1_BANDFREQMHZ, self).__init__(register,
            'BANDFREQMHZ', 'LEGOQPSK.LEGOQPSKCFG1.BANDFREQMHZ', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_LEGOQPSK_LEGOQPSKCFG2_XTAL_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_LEGOQPSK_LEGOQPSKCFG2_XTAL_FREQ, self).__init__(register,
            'XTAL_FREQ', 'LEGOQPSK.LEGOQPSKCFG2.XTAL_FREQ', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


