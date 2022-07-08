
# -*- coding: utf-8 -*-

from . static import Base_RM_Register
from . SUNOFDM_field import *


class RM_Register_SUNOFDM_SUNOFDMCFG(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNOFDM_SUNOFDMCFG, self).__init__(rmio, label,
            0xb500fc00, 0x000,
            'SUNOFDMCFG', 'SUNOFDM.SUNOFDMCFG', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.MODULATION = RM_Field_SUNOFDM_SUNOFDMCFG_MODULATION(self)
        self.zz_fdict['MODULATION'] = self.MODULATION
        self.OFDMOPTION = RM_Field_SUNOFDM_SUNOFDMCFG_OFDMOPTION(self)
        self.zz_fdict['OFDMOPTION'] = self.OFDMOPTION
        self.INTERLEAVING = RM_Field_SUNOFDM_SUNOFDMCFG_INTERLEAVING(self)
        self.zz_fdict['INTERLEAVING'] = self.INTERLEAVING
        self.MACFCSTYPE = RM_Field_SUNOFDM_SUNOFDMCFG_MACFCSTYPE(self)
        self.zz_fdict['MACFCSTYPE'] = self.MACFCSTYPE
        self.XTAL_FREQ = RM_Field_SUNOFDM_SUNOFDMCFG_XTAL_FREQ(self)
        self.zz_fdict['XTAL_FREQ'] = self.XTAL_FREQ
        self.__dict__['zz_frozen'] = True


