from pyradioconfig.parts.bobcat.calculators.calc_viterbi import Calc_Viterbi_Bobcat

class Calc_Viterbi_Sol(Calc_Viterbi_Bobcat):

    def calc_swcoeffen_reg(self, model):

        afc1shot_en = model.vars.MODEM_AFC_AFCONESHOT.value
        fefilt_selected = model.vars.fefilt_selected.value

        reg = 1 if afc1shot_en else 0

        self._reg_write_by_name_concat(model, fefilt_selected, 'CHFCTRL_SWCOEFFEN', reg)

        self._reg_write(model.vars.MODEM_VTCORRCFG1_KSI3SWENABLE, reg)

    def calc_vtdemoden_reg(self, model):
        demod_sel = model.vars.demod_select.value
        modformat = model.vars.modulation_type.value

        # enable viterbi demod when it is selected
        if demod_sel == model.vars.demod_select.var_enum.TRECS_VITERBI or \
                demod_sel == model.vars.demod_select.var_enum.TRECS_SLICER or \
                (demod_sel == model.vars.demod_select.var_enum.SOFT_DEMOD and
                 modformat == model.vars.modulation_type.var_enum.FSK2):
            reg = 1
        else:
            reg = 0

        self._reg_write(model.vars.MODEM_VITERBIDEMOD_VTDEMODEN, reg)

    def calc_realtimcfe_rtschmode_reg(self, model):
        #This function calculates the RTSCHMODE register field for TRECS

        #Read in model variables
        dualsync = model.vars.syncword_dualsync.value
        demod_select = model.vars.demod_select.value
        mod_type = model.vars.modulation_type.value

        #Calculate the register value based on whether we are using TRECS and whether dual syncword detect is enabled
        if demod_select == model.vars.demod_select.var_enum.TRECS_VITERBI or \
                demod_select == model.vars.demod_select.var_enum.TRECS_SLICER or \
                (demod_select == model.vars.demod_select.var_enum.SOFT_DEMOD and \
                 mod_type == model.vars.modulation_type.var_enum.FSK2):

            #If dual syncword detection is enabled, then stop using CFE and hard slice syncword w TRECS
            if dualsync:
                rtschmode = 1
            else:
                rtschmode = 0
        else:
            rtschmode = 0

        #Write the register
        self._reg_write(model.vars.MODEM_REALTIMCFE_RTSCHMODE, rtschmode)

    # improving calculation for PMOFFSET in Sol compared to Ocelot
    # could use this for Ocelot as well but don't want to change it at this point
    def calc_pmoffset_reg(self, model):

        osr = model.vars.MODEM_TRECSCFG_TRECSOSR.value
        rtschmode = model.vars.MODEM_REALTIMCFE_RTSCHMODE.value
        baudrate =  model.vars.baudrate.value
        pmdetthd = model.vars.MODEM_PHDMODCTRL_PMDETTHD.value

        if (rtschmode == 1) and (baudrate > 90000) or model.vars.fast_detect_enable.value == model.vars.fast_detect_enable.var_enum.DISABLED:
            #Special case for dual syncword detection case where hard slicing on syncword is required
            #because frequency tolerance is more difficult when RTSCHMODE is 1
            pmoffset = osr * 2 + 2
        else:
            # + 2 for processing delay. See expsynclen register description. These are used in the same way.
            pmoffset =  osr * pmdetthd + 2

        self._reg_write(model.vars.MODEM_TRECSCFG_PMOFFSET,  pmoffset)