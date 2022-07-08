from pyradioconfig.parts.ocelot.calculators.calc_agc import CALC_AGC_ocelot

class Calc_AGC_Sol(CALC_AGC_ocelot):

    def calc_agc_misc(self, model):
        super().calc_agc_misc(model)

        #Enable peak detector freeze
        self._reg_write(model.vars.AGC_CTRL4_FRZPKDEN, 1)

    def calc_settlingindctrl_reg(self, model):

        #For now, always disable
        settlingindctrl_en = 0
        delayperiod = 0

        #Write the registers
        self._reg_write(model.vars.AGC_SETTLINGINDCTRL_EN, settlingindctrl_en)
        self._reg_write(model.vars.AGC_SETTLINGINDCTRL_POSTHD, 1)
        self._reg_write(model.vars.AGC_SETTLINGINDCTRL_NEGTHD, 1)
        self._reg_write(model.vars.AGC_SETTLINGINDPER_SETTLEDPERIOD, 200) # fast loop
        self._reg_write(model.vars.AGC_SETTLINGINDPER_DELAYPERIOD, delayperiod) # fast loop
