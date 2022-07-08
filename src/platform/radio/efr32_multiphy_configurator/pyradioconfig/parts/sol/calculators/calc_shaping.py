from pyradioconfig.parts.ocelot.calculators.calc_shaping import CALC_Shaping_ocelot

class Calc_Shaping_Sol(CALC_Shaping_ocelot):

    def calc_txfront_coeffs(self, model):

        #Read in model vars
        softmodem_modulator_select = model.vars.softmodem_modulator_select.value

        #Lookup the coeffs
        if softmodem_modulator_select == model.vars.softmodem_modulator_select.var_enum.IQ_MOD:
            coeffs = [-7,-74,-113,-149,-194,-223,-219,-144,82,329,609,983,1374,1734,2001,2076]
        else:
            coeffs = [0]*16

        #Write the registers
        part_family = model.part_family.lower()
        self._reg_write(model.vars.TXFRONT_INT1COEF01_COEFF0,  coeffs[0], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF01_COEFF1, coeffs[1], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF23_COEFF2, coeffs[2], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF23_COEFF3, coeffs[3], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF45_COEFF4, coeffs[4], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF45_COEFF5, coeffs[5], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF67_COEFF6, coeffs[6], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF67_COEFF7, coeffs[7], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF89_COEFF8, coeffs[8], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF89_COEFF9, coeffs[9], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1011_COEFF10, coeffs[10], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1011_COEFF11, coeffs[11], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1213_COEFF12, coeffs[12], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1213_COEFF13, coeffs[13], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1415_COEFF14, coeffs[14], part_family=part_family)
        self._reg_write(model.vars.TXFRONT_INT1COEF1415_COEFF15, coeffs[15], part_family=part_family)