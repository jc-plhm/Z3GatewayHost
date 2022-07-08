
# -*- coding: utf-8 -*-

__all__ = [ 'RM_Peripheral_CW' ]

from . static import Base_RM_Peripheral
from . CW_register import *

class RM_Peripheral_CW(Base_RM_Peripheral):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Peripheral_CW, self).__init__(rmio, label,
            0xB500FC00, 'CW',
            u"")
        self.CWCFG1 = RM_Register_CW_CWCFG1(self.zz_rmio, self.zz_label)
        self.zz_rdict['CWCFG1'] = self.CWCFG1
        self.CWCFG2 = RM_Register_CW_CWCFG2(self.zz_rmio, self.zz_label)
        self.zz_rdict['CWCFG2'] = self.CWCFG2
        self.CWCFG3 = RM_Register_CW_CWCFG3(self.zz_rmio, self.zz_label)
        self.zz_rdict['CWCFG3'] = self.CWCFG3
        self.__dict__['zz_frozen'] = True