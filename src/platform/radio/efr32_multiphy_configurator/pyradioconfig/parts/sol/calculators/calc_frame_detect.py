from pyradioconfig.parts.ocelot.calculators.calc_frame_detect import CALC_Frame_Detect_Ocelot

class Calc_Frame_Detect_Sol(CALC_Frame_Detect_Ocelot):

    def calc_syncerrors_reg(self, model):
        # This function calulates the SYNCERRORS field

        # Read in model variables
        demod_select = model.vars.demod_select.value
        rtschmode_actual = model.vars.MODEM_REALTIMCFE_RTSCHMODE.value
        mod_type = model.vars.modulation_type.value

        # Allow 1 sync error if using TRECS and RTSCHMODE = 1 (hard slicing instead of CFE)
        if demod_select == model.vars.demod_select.var_enum.TRECS_VITERBI or \
                demod_select == model.vars.demod_select.var_enum.TRECS_SLICER or \
                (demod_select == model.vars.demod_select.var_enum.SOFT_DEMOD and \
                 mod_type == model.vars.modulation_type.var_enum.FSK2):
            if rtschmode_actual == 1:
                syncerrors = 1
            else:
                syncerrors = 0
        else:
            syncerrors = 0

        # Write the register
        self._reg_write(model.vars.MODEM_CTRL1_SYNCERRORS, syncerrors)