
# -*- coding: utf-8 -*-

__all__ = [ 'RM_Peripheral_SUNOQPSK' ]

from . static import Base_RM_Peripheral
from . SUNOQPSK_register import *

class RM_Peripheral_SUNOQPSK(Base_RM_Peripheral):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Peripheral_SUNOQPSK, self).__init__(rmio, label,
            0xB500FC00, 'SUNOQPSK',
            u"")
        self.SUNOQPSKCFG1 = RM_Register_SUNOQPSK_SUNOQPSKCFG1(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNOQPSKCFG1'] = self.SUNOQPSKCFG1
        self.SUNOQPSKCFG2 = RM_Register_SUNOQPSK_SUNOQPSKCFG2(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNOQPSKCFG2'] = self.SUNOQPSKCFG2
        self.__dict__['zz_frozen'] = True