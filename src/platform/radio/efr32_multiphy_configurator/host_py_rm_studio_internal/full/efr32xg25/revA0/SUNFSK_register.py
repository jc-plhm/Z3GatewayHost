
# -*- coding: utf-8 -*-

from . static import Base_RM_Register
from . SUNFSK_field import *


class RM_Register_SUNFSK_SUNFSKCFG1(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG1, self).__init__(rmio, label,
            0xb500fc00, 0x000,
            'SUNFSKCFG1', 'SUNFSK.SUNFSKCFG1', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.MODULATION = RM_Field_SUNFSK_SUNFSKCFG1_MODULATION(self)
        self.zz_fdict['MODULATION'] = self.MODULATION
        self.FECSEL = RM_Field_SUNFSK_SUNFSKCFG1_FECSEL(self)
        self.zz_fdict['FECSEL'] = self.FECSEL
        self.PHYSUNFSKSFD = RM_Field_SUNFSK_SUNFSKCFG1_PHYSUNFSKSFD(self)
        self.zz_fdict['PHYSUNFSKSFD'] = self.PHYSUNFSKSFD
        self.MODSCHEME = RM_Field_SUNFSK_SUNFSKCFG1_MODSCHEME(self)
        self.zz_fdict['MODSCHEME'] = self.MODSCHEME
        self.BT = RM_Field_SUNFSK_SUNFSKCFG1_BT(self)
        self.zz_fdict['BT'] = self.BT
        self.MODINDEX = RM_Field_SUNFSK_SUNFSKCFG1_MODINDEX(self)
        self.zz_fdict['MODINDEX'] = self.MODINDEX
        self.FSKPREAMBLELENGTH = RM_Field_SUNFSK_SUNFSKCFG1_FSKPREAMBLELENGTH(self)
        self.zz_fdict['FSKPREAMBLELENGTH'] = self.FSKPREAMBLELENGTH
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG2(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG2, self).__init__(rmio, label,
            0xb500fc00, 0x004,
            'SUNFSKCFG2', 'SUNFSK.SUNFSKCFG2', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.VEQEN = RM_Field_SUNFSK_SUNFSKCFG2_VEQEN(self)
        self.zz_fdict['VEQEN'] = self.VEQEN
        self.KSI1 = RM_Field_SUNFSK_SUNFSKCFG2_KSI1(self)
        self.zz_fdict['KSI1'] = self.KSI1
        self.KSI2 = RM_Field_SUNFSK_SUNFSKCFG2_KSI2(self)
        self.zz_fdict['KSI2'] = self.KSI2
        self.KSI3 = RM_Field_SUNFSK_SUNFSKCFG2_KSI3(self)
        self.zz_fdict['KSI3'] = self.KSI3
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG3(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG3, self).__init__(rmio, label,
            0xb500fc00, 0x008,
            'SUNFSKCFG3', 'SUNFSK.SUNFSKCFG3', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.KSI3W = RM_Field_SUNFSK_SUNFSKCFG3_KSI3W(self)
        self.zz_fdict['KSI3W'] = self.KSI3W
        self.PHASESCALE = RM_Field_SUNFSK_SUNFSKCFG3_PHASESCALE(self)
        self.zz_fdict['PHASESCALE'] = self.PHASESCALE
        self.OSR = RM_Field_SUNFSK_SUNFSKCFG3_OSR(self)
        self.zz_fdict['OSR'] = self.OSR
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG4(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG4, self).__init__(rmio, label,
            0xb500fc00, 0x00C,
            'SUNFSKCFG4', 'SUNFSK.SUNFSKCFG4', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.PREAMBLECOSTTHD = RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECOSTTHD(self)
        self.zz_fdict['PREAMBLECOSTTHD'] = self.PREAMBLECOSTTHD
        self.PREAMBLECNTWIN = RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTWIN(self)
        self.zz_fdict['PREAMBLECNTWIN'] = self.PREAMBLECNTWIN
        self.PREAMBLECNTTHD = RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTTHD(self)
        self.zz_fdict['PREAMBLECNTTHD'] = self.PREAMBLECNTTHD
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG5(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG5, self).__init__(rmio, label,
            0xb500fc00, 0x010,
            'SUNFSKCFG5', 'SUNFSK.SUNFSKCFG5', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.PREAMBLELEN = RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLELEN(self)
        self.zz_fdict['PREAMBLELEN'] = self.PREAMBLELEN
        self.PREAMBLEERRORS = RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLEERRORS(self)
        self.zz_fdict['PREAMBLEERRORS'] = self.PREAMBLEERRORS
        self.PREAMBLETIMEOUT = RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLETIMEOUT(self)
        self.zz_fdict['PREAMBLETIMEOUT'] = self.PREAMBLETIMEOUT
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG6(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG6, self).__init__(rmio, label,
            0xb500fc00, 0x014,
            'SUNFSKCFG6', 'SUNFSK.SUNFSKCFG6', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.PREAMBLEPATT = RM_Field_SUNFSK_SUNFSKCFG6_PREAMBLEPATT(self)
        self.zz_fdict['PREAMBLEPATT'] = self.PREAMBLEPATT
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG7(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG7, self).__init__(rmio, label,
            0xb500fc00, 0x018,
            'SUNFSKCFG7', 'SUNFSK.SUNFSKCFG7', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.PREAMBLEFREQLIM = RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLEFREQLIM(self)
        self.zz_fdict['PREAMBLEFREQLIM'] = self.PREAMBLEFREQLIM
        self.PREAMBLECOSTMAX = RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLECOSTMAX(self)
        self.zz_fdict['PREAMBLECOSTMAX'] = self.PREAMBLECOSTMAX
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG8(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG8, self).__init__(rmio, label,
            0xb500fc00, 0x01C,
            'SUNFSKCFG8', 'SUNFSK.SUNFSKCFG8', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.SFD1 = RM_Field_SUNFSK_SUNFSKCFG8_SFD1(self)
        self.zz_fdict['SFD1'] = self.SFD1
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG9(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG9, self).__init__(rmio, label,
            0xb500fc00, 0x020,
            'SUNFSKCFG9', 'SUNFSK.SUNFSKCFG9', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.SFD2 = RM_Field_SUNFSK_SUNFSKCFG9_SFD2(self)
        self.zz_fdict['SFD2'] = self.SFD2
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG10(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG10, self).__init__(rmio, label,
            0xb500fc00, 0x024,
            'SUNFSKCFG10', 'SUNFSK.SUNFSKCFG10', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.SFDLEN = RM_Field_SUNFSK_SUNFSKCFG10_SFDLEN(self)
        self.zz_fdict['SFDLEN'] = self.SFDLEN
        self.SFDCOSTTHD = RM_Field_SUNFSK_SUNFSKCFG10_SFDCOSTTHD(self)
        self.zz_fdict['SFDCOSTTHD'] = self.SFDCOSTTHD
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG11(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG11, self).__init__(rmio, label,
            0xb500fc00, 0x028,
            'SUNFSKCFG11', 'SUNFSK.SUNFSKCFG11', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.SFDTIMEOUT = RM_Field_SUNFSK_SUNFSKCFG11_SFDTIMEOUT(self)
        self.zz_fdict['SFDTIMEOUT'] = self.SFDTIMEOUT
        self.SFDERRORS = RM_Field_SUNFSK_SUNFSKCFG11_SFDERRORS(self)
        self.zz_fdict['SFDERRORS'] = self.SFDERRORS
        self.TIMTRACKGEAR = RM_Field_SUNFSK_SUNFSKCFG11_TIMTRACKGEAR(self)
        self.zz_fdict['TIMTRACKGEAR'] = self.TIMTRACKGEAR
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG12(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG12, self).__init__(rmio, label,
            0xb500fc00, 0x02C,
            'SUNFSKCFG12', 'SUNFSK.SUNFSKCFG12', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.TIMTRACKSYMB = RM_Field_SUNFSK_SUNFSKCFG12_TIMTRACKSYMB(self)
        self.zz_fdict['TIMTRACKSYMB'] = self.TIMTRACKSYMB
        self.TIMMINTRANS = RM_Field_SUNFSK_SUNFSKCFG12_TIMMINTRANS(self)
        self.zz_fdict['TIMMINTRANS'] = self.TIMMINTRANS
        self.REALTIMESEARCH = RM_Field_SUNFSK_SUNFSKCFG12_REALTIMESEARCH(self)
        self.zz_fdict['REALTIMESEARCH'] = self.REALTIMESEARCH
        self.NBSYMBBATCH = RM_Field_SUNFSK_SUNFSKCFG12_NBSYMBBATCH(self)
        self.zz_fdict['NBSYMBBATCH'] = self.NBSYMBBATCH
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG13(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG13, self).__init__(rmio, label,
            0xb500fc00, 0x030,
            'SUNFSKCFG13', 'SUNFSK.SUNFSKCFG13', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.CHFILTSWEN = RM_Field_SUNFSK_SUNFSKCFG13_CHFILTSWEN(self)
        self.zz_fdict['CHFILTSWEN'] = self.CHFILTSWEN
        self.AFCMODE = RM_Field_SUNFSK_SUNFSKCFG13_AFCMODE(self)
        self.zz_fdict['AFCMODE'] = self.AFCMODE
        self.AFCSCALE = RM_Field_SUNFSK_SUNFSKCFG13_AFCSCALE(self)
        self.zz_fdict['AFCSCALE'] = self.AFCSCALE
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG14(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG14, self).__init__(rmio, label,
            0xb500fc00, 0x034,
            'SUNFSKCFG14', 'SUNFSK.SUNFSKCFG14', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.AFCADJLIM = RM_Field_SUNFSK_SUNFSKCFG14_AFCADJLIM(self)
        self.zz_fdict['AFCADJLIM'] = self.AFCADJLIM
        self.__dict__['zz_frozen'] = True


class RM_Register_SUNFSK_SUNFSKCFG15(Base_RM_Register):
    def __init__(self, rmio, label):
        self.__dict__['zz_frozen'] = False
        super(RM_Register_SUNFSK_SUNFSKCFG15, self).__init__(rmio, label,
            0xb500fc00, 0x038,
            'SUNFSKCFG15', 'SUNFSK.SUNFSKCFG15', 'read-write',
            u"",
            0x00000000, 0x00000000,
            0x00001000, 0x00002000,
            0x00003000)

        self.AFCADJPERIOD = RM_Field_SUNFSK_SUNFSKCFG15_AFCADJPERIOD(self)
        self.zz_fdict['AFCADJPERIOD'] = self.AFCADJPERIOD
        self.AFCCOSTTHD = RM_Field_SUNFSK_SUNFSKCFG15_AFCCOSTTHD(self)
        self.zz_fdict['AFCCOSTTHD'] = self.AFCCOSTTHD
        self.XTAL_FREQ = RM_Field_SUNFSK_SUNFSKCFG15_XTAL_FREQ(self)
        self.zz_fdict['XTAL_FREQ'] = self.XTAL_FREQ
        self.__dict__['zz_frozen'] = True


