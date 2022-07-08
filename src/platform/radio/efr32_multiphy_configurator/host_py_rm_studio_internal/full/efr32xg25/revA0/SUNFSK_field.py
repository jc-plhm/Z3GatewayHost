
# -*- coding: utf-8 -*-

from . static import Base_RM_Field


class RM_Field_SUNFSK_SUNFSKCFG1_MODULATION(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_MODULATION, self).__init__(register,
            'MODULATION', 'SUNFSK.SUNFSKCFG1.MODULATION', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_FECSEL(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_FECSEL, self).__init__(register,
            'FECSEL', 'SUNFSK.SUNFSKCFG1.FECSEL', 'read-write',
            u"",
            8, 2)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_PHYSUNFSKSFD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_PHYSUNFSKSFD, self).__init__(register,
            'PHYSUNFSKSFD', 'SUNFSK.SUNFSKCFG1.PHYSUNFSKSFD', 'read-write',
            u"",
            10, 1)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_MODSCHEME(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_MODSCHEME, self).__init__(register,
            'MODSCHEME', 'SUNFSK.SUNFSKCFG1.MODSCHEME', 'read-write',
            u"",
            12, 2)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_BT(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_BT, self).__init__(register,
            'BT', 'SUNFSK.SUNFSKCFG1.BT', 'read-write',
            u"",
            14, 2)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_MODINDEX(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_MODINDEX, self).__init__(register,
            'MODINDEX', 'SUNFSK.SUNFSKCFG1.MODINDEX', 'read-write',
            u"",
            16, 6)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG1_FSKPREAMBLELENGTH(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG1_FSKPREAMBLELENGTH, self).__init__(register,
            'FSKPREAMBLELENGTH', 'SUNFSK.SUNFSKCFG1.FSKPREAMBLELENGTH', 'read-write',
            u"",
            24, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG2_VEQEN(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG2_VEQEN, self).__init__(register,
            'VEQEN', 'SUNFSK.SUNFSKCFG2.VEQEN', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG2_KSI1(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG2_KSI1, self).__init__(register,
            'KSI1', 'SUNFSK.SUNFSKCFG2.KSI1', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG2_KSI2(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG2_KSI2, self).__init__(register,
            'KSI2', 'SUNFSK.SUNFSKCFG2.KSI2', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG2_KSI3(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG2_KSI3, self).__init__(register,
            'KSI3', 'SUNFSK.SUNFSKCFG2.KSI3', 'read-write',
            u"",
            24, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG3_KSI3W(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG3_KSI3W, self).__init__(register,
            'KSI3W', 'SUNFSK.SUNFSKCFG3.KSI3W', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG3_PHASESCALE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG3_PHASESCALE, self).__init__(register,
            'PHASESCALE', 'SUNFSK.SUNFSKCFG3.PHASESCALE', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG3_OSR(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG3_OSR, self).__init__(register,
            'OSR', 'SUNFSK.SUNFSKCFG3.OSR', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECOSTTHD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECOSTTHD, self).__init__(register,
            'PREAMBLECOSTTHD', 'SUNFSK.SUNFSKCFG4.PREAMBLECOSTTHD', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTWIN(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTWIN, self).__init__(register,
            'PREAMBLECNTWIN', 'SUNFSK.SUNFSKCFG4.PREAMBLECNTWIN', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTTHD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG4_PREAMBLECNTTHD, self).__init__(register,
            'PREAMBLECNTTHD', 'SUNFSK.SUNFSKCFG4.PREAMBLECNTTHD', 'read-write',
            u"",
            24, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLELEN(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLELEN, self).__init__(register,
            'PREAMBLELEN', 'SUNFSK.SUNFSKCFG5.PREAMBLELEN', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLEERRORS(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLEERRORS, self).__init__(register,
            'PREAMBLEERRORS', 'SUNFSK.SUNFSKCFG5.PREAMBLEERRORS', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLETIMEOUT(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG5_PREAMBLETIMEOUT, self).__init__(register,
            'PREAMBLETIMEOUT', 'SUNFSK.SUNFSKCFG5.PREAMBLETIMEOUT', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG6_PREAMBLEPATT(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG6_PREAMBLEPATT, self).__init__(register,
            'PREAMBLEPATT', 'SUNFSK.SUNFSKCFG6.PREAMBLEPATT', 'read-write',
            u"",
            0, 32)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLEFREQLIM(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLEFREQLIM, self).__init__(register,
            'PREAMBLEFREQLIM', 'SUNFSK.SUNFSKCFG7.PREAMBLEFREQLIM', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLECOSTMAX(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG7_PREAMBLECOSTMAX, self).__init__(register,
            'PREAMBLECOSTMAX', 'SUNFSK.SUNFSKCFG7.PREAMBLECOSTMAX', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG8_SFD1(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG8_SFD1, self).__init__(register,
            'SFD1', 'SUNFSK.SUNFSKCFG8.SFD1', 'read-write',
            u"",
            0, 32)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG9_SFD2(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG9_SFD2, self).__init__(register,
            'SFD2', 'SUNFSK.SUNFSKCFG9.SFD2', 'read-write',
            u"",
            0, 32)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG10_SFDLEN(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG10_SFDLEN, self).__init__(register,
            'SFDLEN', 'SUNFSK.SUNFSKCFG10.SFDLEN', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG10_SFDCOSTTHD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG10_SFDCOSTTHD, self).__init__(register,
            'SFDCOSTTHD', 'SUNFSK.SUNFSKCFG10.SFDCOSTTHD', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG11_SFDTIMEOUT(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG11_SFDTIMEOUT, self).__init__(register,
            'SFDTIMEOUT', 'SUNFSK.SUNFSKCFG11.SFDTIMEOUT', 'read-write',
            u"",
            0, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG11_SFDERRORS(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG11_SFDERRORS, self).__init__(register,
            'SFDERRORS', 'SUNFSK.SUNFSKCFG11.SFDERRORS', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG11_TIMTRACKGEAR(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG11_TIMTRACKGEAR, self).__init__(register,
            'TIMTRACKGEAR', 'SUNFSK.SUNFSKCFG11.TIMTRACKGEAR', 'read-write',
            u"",
            24, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG12_TIMTRACKSYMB(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG12_TIMTRACKSYMB, self).__init__(register,
            'TIMTRACKSYMB', 'SUNFSK.SUNFSKCFG12.TIMTRACKSYMB', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG12_TIMMINTRANS(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG12_TIMMINTRANS, self).__init__(register,
            'TIMMINTRANS', 'SUNFSK.SUNFSKCFG12.TIMMINTRANS', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG12_REALTIMESEARCH(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG12_REALTIMESEARCH, self).__init__(register,
            'REALTIMESEARCH', 'SUNFSK.SUNFSKCFG12.REALTIMESEARCH', 'read-write',
            u"",
            16, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG12_NBSYMBBATCH(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG12_NBSYMBBATCH, self).__init__(register,
            'NBSYMBBATCH', 'SUNFSK.SUNFSKCFG12.NBSYMBBATCH', 'read-write',
            u"",
            24, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG13_CHFILTSWEN(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG13_CHFILTSWEN, self).__init__(register,
            'CHFILTSWEN', 'SUNFSK.SUNFSKCFG13.CHFILTSWEN', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG13_AFCMODE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG13_AFCMODE, self).__init__(register,
            'AFCMODE', 'SUNFSK.SUNFSKCFG13.AFCMODE', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG13_AFCSCALE(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG13_AFCSCALE, self).__init__(register,
            'AFCSCALE', 'SUNFSK.SUNFSKCFG13.AFCSCALE', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG14_AFCADJLIM(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG14_AFCADJLIM, self).__init__(register,
            'AFCADJLIM', 'SUNFSK.SUNFSKCFG14.AFCADJLIM', 'read-write',
            u"",
            0, 32)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG15_AFCADJPERIOD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG15_AFCADJPERIOD, self).__init__(register,
            'AFCADJPERIOD', 'SUNFSK.SUNFSKCFG15.AFCADJPERIOD', 'read-write',
            u"",
            0, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG15_AFCCOSTTHD(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG15_AFCCOSTTHD, self).__init__(register,
            'AFCCOSTTHD', 'SUNFSK.SUNFSKCFG15.AFCCOSTTHD', 'read-write',
            u"",
            8, 8)
        self.__dict__['zz_frozen'] = True


class RM_Field_SUNFSK_SUNFSKCFG15_XTAL_FREQ(Base_RM_Field):
    def __init__(self, register):
        self.__dict__['zz_frozen'] = False
        super(RM_Field_SUNFSK_SUNFSKCFG15_XTAL_FREQ, self).__init__(register,
            'XTAL_FREQ', 'SUNFSK.SUNFSKCFG15.XTAL_FREQ', 'read-write',
            u"",
            16, 16)
        self.__dict__['zz_frozen'] = True


