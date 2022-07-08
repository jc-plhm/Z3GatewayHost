
# -*- coding: utf-8 -*-

__all__ = [ 'RM_Peripheral_SUNOFDM' ]

from . static import Base_RM_Peripheral
from . SUNOFDM_register import *

class RM_Peripheral_SUNOFDM(Base_RM_Peripheral):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Peripheral_SUNOFDM, self).__init__(rmio, label,
            0xB500FC00, 'SUNOFDM',
            u"")
        self.SUNOFDMCFG = RM_Register_SUNOFDM_SUNOFDMCFG(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNOFDMCFG'] = self.SUNOFDMCFG
        self.__dict__['zz_frozen'] = True