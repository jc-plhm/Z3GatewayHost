
# -*- coding: utf-8 -*-

from . static import Base_RM_Field


class RM_Field_SUNOFDM_SUNOFDMCFG_MODULATION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOFDM_SUNOFDMCFG_MODULATION, self).__init__(register,
            'MODULATION', 'SUNOFDM.SUNOFDMCFG.MODULATION', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOFDM_SUNOFDMCFG_OFDMOPTION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOFDM_SUNOFDMCFG_OFDMOPTION, self).__init__(register,
            'OFDMOPTION', 'SUNOFDM.SUNOFDMCFG.OFDMOPTION', 'read-write',
            u"",
            8, 2)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOFDM_SUNOFDMCFG_INTERLEAVING(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOFDM_SUNOFDMCFG_INTERLEAVING, self).__init__(register,
            'INTERLEAVING', 'SUNOFDM.SUNOFDMCFG.INTERLEAVING', 'read-write',
            u"",
            10, 1)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOFDM_SUNOFDMCFG_MACFCSTYPE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOFDM_SUNOFDMCFG_MACFCSTYPE, self).__init__(register,
            'MACFCSTYPE', 'SUNOFDM.SUNOFDMCFG.MACFCSTYPE', 'read-write',
            u"",
            11, 1)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNOFDM_SUNOFDMCFG_XTAL_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNOFDM_SUNOFDMCFG_XTAL_FREQ, self).__init__(register,
            'XTAL_FREQ', 'SUNOFDM.SUNOFDMCFG.XTAL_FREQ', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


