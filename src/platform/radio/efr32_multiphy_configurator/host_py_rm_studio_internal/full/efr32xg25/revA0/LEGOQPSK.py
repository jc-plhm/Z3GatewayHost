
# -*- coding: utf-8 -*-

__all__ = [ 'RM_Peripheral_LEGOQPSK' ]

from . static import Base_RM_Peripheral
from . LEGOQPSK_register import *

class RM_Peripheral_LEGOQPSK(Base_RM_Peripheral):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Peripheral_LEGOQPSK, self).__init__(rmio, label,
            0xB500FC00, 'LEGOQPSK',
            u"")
        self.LEGOQPSKCFG1 = RM_Register_LEGOQPSK_LEGOQPSKCFG1(self.zz_rmio, self.zz_label)
        self.zz_rdict['LEGOQPSKCFG1'] = self.LEGOQPSKCFG1
        self.LEGOQPSKCFG2 = RM_Register_LEGOQPSK_LEGOQPSKCFG2(self.zz_rmio, self.zz_label)
        self.zz_rdict['LEGOQPSKCFG2'] = self.LEGOQPSKCFG2
        self.__dict__['zz_frozen'] = True