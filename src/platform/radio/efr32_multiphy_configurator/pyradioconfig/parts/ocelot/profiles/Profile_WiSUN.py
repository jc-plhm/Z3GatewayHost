from pyradioconfig.parts.jumbo.profiles.Profile_WiSUN import Profile_WiSUN_Jumbo
from pyradioconfig.parts.common.profiles.ocelot_regs import build_modem_regs_ocelot
from pyradioconfig.parts.common.profiles.profile_common import buildCrcOutputs, buildFecOutputs, buildFrameOutputs, \
    buildWhiteOutputs
from pyradioconfig.parts.common.utils.units_multiplier import UnitsMultiplier

class Profile_WiSUN_Ocelot(Profile_WiSUN_Jumbo):

    def __init__(self):
        self._profileName = "WiSUN"
        self._readable_name = "Wi-SUN FSK Profile"
        self._category = ""
        self._description = "Profile used for Wi-SUN FSK PHYs"
        self._default = False
        self._activation_logic = ""
        self._family = "ocelot"

    def build_optional_profile_inputs(self, model, profile):
        #Start with the same optional Profile Inputs as Series 1
        super().build_optional_profile_inputs(model, profile)

        #Add additional inputs
        self.make_optional_input(profile, model.vars.fec_tx_enable, 'Channel_Coding',
                                     readable_name="Enable FEC on TX",
                                     default=model.vars.fec_tx_enable.var_enum.DISABLED)
    def build_advanced_profile_inputs(self, model, profile):
        self.make_linked_io(profile, model.vars.antdivmode, 'Advanced', readable_name="Antenna diversity mode")
        self.make_linked_io(profile, model.vars.antdivrepeatdis, 'Advanced', readable_name="Diversity Select-Best repeat")
        self.make_linked_io(profile, model.vars.skip2ant, 'Advanced', 'Skip 2nd antenna check with phase demod antenna diversity')
        pass

    def build_hidden_profile_inputs(self, model, profile):
        # Hidden inputs to allow for fixed frame length testing
        self.make_hidden_input(profile, model.vars.frame_length_type, 'frame_general',
                                   readable_name="Frame Length Algorithm")
        self.make_hidden_input(profile, model.vars.fixed_length_size, category='frame_fixed_length',
                                   readable_name="Fixed Payload Size", value_limit_min=0, value_limit_max=0x7fffffff)

        # Hidden inputs to allow for keeping absolute tolerance the same when testing at 915M
        self.make_hidden_input(profile, model.vars.rx_xtal_error_ppm, category="general",
                                   readable_name="Set desired xtal tolerance on RX side", value_limit_min=0,
                                   value_limit_max=100)
        self.make_hidden_input(profile, model.vars.tx_xtal_error_ppm, category="general",
                                   readable_name="Set desired xtal tolerance on TX side", value_limit_min=0,
                                   value_limit_max=100)
        self.make_hidden_input(profile, model.vars.freq_offset_hz, 'Advanced',
                                   readable_name="Frequency Offset Compensation (AFC) Limit", value_limit_min=0,
                                   value_limit_max=500000, units_multiplier=UnitsMultiplier.KILO)

    def build_deprecated_profile_inputs(self, model, profile):
        # These inputs were exposed on or after Ocelot Alpha 1 release, so they may be present in radioconf XML
        self.make_deprecated_input(profile, model.vars.max_tx_power_dbm)

    def build_register_profile_outputs(self, model, profile):
        # Overriding this method to include modem regs for Ocelot
        family = self._family
        build_modem_regs_ocelot(model, profile, family)
        buildFrameOutputs(model, profile, family)
        buildCrcOutputs(model, profile, family)
        buildWhiteOutputs(model, profile)
        buildFecOutputs(model, profile)

    def _fixed_wisun_vars(self, model):
        #Start with the inherited vars
        super()._fixed_wisun_vars(model)

        #Add deviation tolerance and xtal error for Ocelot
        model.vars.deviation_tol_ppm.value_forced = 300000
        model.vars.rx_xtal_error_ppm.value_forced = 20  # FAN EU Mode #1b, WiSUN 20140727-PHY-Profile Table 7 (assume RX=TX value)
        model.vars.tx_xtal_error_ppm.value_forced = 20  # FAN EU Mode #1b, WiSUN 20140727-PHY-Profile Table 7 (assume RX=TX value)

    def _fixed_wisun_thresholds(self, model):
        #No hard-coded thresholds on Ocelot
        pass

    def _fixed_wisun_agc(self, model):
        #No hard-coded AGC settings on Ocelot
        pass

    def _fixed_wisun_fec(self, model):
        # We have an optional Profile Input for fec_tx_enable on Ocelot
        pass

    def _lookup_from_wisun_mode(self, model):
        # This function calculates some variables/registers based on the wisun_mode

        # Read the mode and fec_en from the profile inputs (not yet written to model vars)
        wisun_mode = model.profile.inputs.wisun_mode.var_value
        fec_tx_enable = (model.profile.inputs.fec_tx_enable.var_value == model.vars.fec_tx_enable.var_enum.ENABLED)

        # We need to modify the net bitrate (data rate) based on whether FEC is enabled or not
        fec_factor = 2 if fec_tx_enable else 1

        #Set basic parameters based on the mode
        if wisun_mode == model.vars.wisun_mode.var_enum.Mode1a:
            model.vars.bitrate.value_forced = 50000//fec_factor
            model.vars.deviation.value_forced = 12500

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode1b:
            model.vars.bitrate.value_forced = 50000//fec_factor
            model.vars.deviation.value_forced = 25000

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode2a:
            model.vars.bitrate.value_forced = 100000//fec_factor
            model.vars.deviation.value_forced = 25000

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode2b:
            model.vars.bitrate.value_forced = 100000//fec_factor
            model.vars.deviation.value_forced = 50000

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode3:
            model.vars.bitrate.value_forced = 150000//fec_factor
            model.vars.deviation.value_forced = 37500

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode4a:
            model.vars.bitrate.value_forced = 200000//fec_factor
            model.vars.deviation.value_forced = 50000

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode4b:
            model.vars.bitrate.value_forced = 200000//fec_factor
            model.vars.deviation.value_forced = 100000

        elif wisun_mode == model.vars.wisun_mode.var_enum.Mode5:
            model.vars.bitrate.value_forced = 300000//fec_factor
            model.vars.deviation.value_forced = 75000