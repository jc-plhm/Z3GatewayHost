
# -*- coding: utf-8 -*-

__all__ = [ 'RM_Peripheral_SUNFSK' ]

from . static import Base_RM_Peripheral
from . SUNFSK_register import *

class RM_Peripheral_SUNFSK(Base_RM_Peripheral):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Peripheral_SUNFSK, self).__init__(rmio, label,
            0xB500FC00, 'SUNFSK',
            u"")
        self.SUNFSKCFG1 = RM_Register_SUNFSK_SUNFSKCFG1(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG1'] = self.SUNFSKCFG1
        self.SUNFSKCFG2 = RM_Register_SUNFSK_SUNFSKCFG2(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG2'] = self.SUNFSKCFG2
        self.SUNFSKCFG3 = RM_Register_SUNFSK_SUNFSKCFG3(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG3'] = self.SUNFSKCFG3
        self.SUNFSKCFG4 = RM_Register_SUNFSK_SUNFSKCFG4(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG4'] = self.SUNFSKCFG4
        self.SUNFSKCFG5 = RM_Register_SUNFSK_SUNFSKCFG5(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG5'] = self.SUNFSKCFG5
        self.SUNFSKCFG6 = RM_Register_SUNFSK_SUNFSKCFG6(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG6'] = self.SUNFSKCFG6
        self.SUNFSKCFG7 = RM_Register_SUNFSK_SUNFSKCFG7(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG7'] = self.SUNFSKCFG7
        self.SUNFSKCFG8 = RM_Register_SUNFSK_SUNFSKCFG8(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG8'] = self.SUNFSKCFG8
        self.SUNFSKCFG9 = RM_Register_SUNFSK_SUNFSKCFG9(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG9'] = self.SUNFSKCFG9
        self.SUNFSKCFG10 = RM_Register_SUNFSK_SUNFSKCFG10(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG10'] = self.SUNFSKCFG10
        self.SUNFSKCFG11 = RM_Register_SUNFSK_SUNFSKCFG11(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG11'] = self.SUNFSKCFG11
        self.SUNFSKCFG12 = RM_Register_SUNFSK_SUNFSKCFG12(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG12'] = self.SUNFSKCFG12
        self.SUNFSKCFG13 = RM_Register_SUNFSK_SUNFSKCFG13(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG13'] = self.SUNFSKCFG13
        self.SUNFSKCFG14 = RM_Register_SUNFSK_SUNFSKCFG14(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG14'] = self.SUNFSKCFG14
        self.SUNFSKCFG15 = RM_Register_SUNFSK_SUNFSKCFG15(self.zz_rmio, self.zz_label)
        self.zz_rdict['SUNFSKCFG15'] = self.SUNFSKCFG15
        self.__dict__['zz_frozen'] = True