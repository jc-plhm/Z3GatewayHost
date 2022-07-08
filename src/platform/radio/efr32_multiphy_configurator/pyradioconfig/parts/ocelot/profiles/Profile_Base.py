from pyradioconfig.parts.common.profiles.Profile_Base import Profile_Base
from pyradioconfig.parts.common.profiles.ocelot_regs import *
from pyradioconfig.parts.common.profiles.profile_common import *
from pyradioconfig.parts.ocelot.profiles.profile_modem import *
from pyradioconfig.parts.ocelot.profiles.frame_profile_inputs_common import frame_profile_inputs_common_ocelot

class Profile_Base_Ocelot(Profile_Base):

    """
    Init internal variables
    """
    def __init__(self):
        super().__init__()
        self._description = "Profile used for most PHYs on EFR32xG23 parts"
        self._family = "ocelot"

    # def buildProfileModel(self, model):
    #     # All profile I/O for Ocelot should be defined here (no splitting up between files)
    #     # If this needs to be inherited, split up into multiple sub-methods and then override those as needed (e.g. to remove profile I/O or remap)
    #     # Do not add if/then statements based on part family
    #
    #     # TODO: Clean this up so that all of the profile I/O is bubbled up to the top level
    #
    #     return super(Profile_Base_Ocelot, self).buildProfileModel(model)

    def buildProfileModel(self, model):

        family = self._family

        # Build profile
        profile = self._makeProfile(model)

        # Add inputs
        self.build_required_profile_inputs(model, profile)
        self.build_optional_profile_inputs(model, profile)
        self.build_advanced_profile_inputs(model, profile)
        self.build_hidden_profile_inputs(model, profile)
        self.build_deprecated_profile_inputs(model, profile)

        # Frame configuration Inputs
        self.build_frame_configuration_inputs(model, profile)

        # Modem Advanced Inputs
        buildModemAdvancedInputs(model, profile, family)

        # Informational Output
        buildModemInfoOutputs(model, profile)

        # RAIL Outputs
        buildRailOutputs(model, profile)

        # Output fields
        buildFrameOutputs(model, profile, family=family)
        buildCrcOutputs(model, profile, family)
        buildWhiteOutputs(model, profile)
        buildFecOutputs(model, profile)

        self.buildRegisterOutputs(model, profile, family)

        build_ircal_sw_vars(model, profile)

        return profile

    def build_required_profile_inputs(self, model, profile):
        IProfile.make_required_input(profile, model.vars.base_frequency_hz, "operational_frequency",
                                     readable_name="Base Channel Frequency", value_limit_min=long(100000000),
                                     value_limit_max=long(2480000000), units_multiplier=UnitsMultiplier.MEGA)
        IProfile.make_required_input(profile, model.vars.channel_spacing_hz, "operational_frequency",
                                     readable_name="Channel Spacing", value_limit_min=0, value_limit_max=10000000,
                                     units_multiplier=UnitsMultiplier.KILO)

        IProfile.make_required_input(profile, model.vars.xtal_frequency_hz, "crystal",
                                     readable_name="Crystal Frequency", value_limit_min=38000000,
                                     value_limit_max=40000000, units_multiplier=UnitsMultiplier.MEGA)
        IProfile.make_required_input(profile, model.vars.rx_xtal_error_ppm, "crystal",
                                     readable_name="RX Crystal Accuracy", value_limit_min=0, value_limit_max=200)
        IProfile.make_required_input(profile, model.vars.tx_xtal_error_ppm, "crystal",
                                     readable_name="TX Crystal Accuracy", value_limit_min=0, value_limit_max=200)

        IProfile.make_required_input(profile, model.vars.syncword_0, "syncword", readable_name="Sync Word 0",
                                     value_limit_min=long(0), value_limit_max=long(0xffffffff))
        IProfile.make_required_input(profile, model.vars.syncword_1, "syncword", readable_name="Sync Word 1",
                                     value_limit_min=long(0), value_limit_max=long(0xffffffff))
        IProfile.make_required_input(profile, model.vars.syncword_length, "syncword", readable_name="Sync Word Length",
                                     value_limit_min=0, value_limit_max=32)

        IProfile.make_required_input(profile, model.vars.preamble_pattern_len, "preamble",
                                     readable_name="Preamble Pattern Length", value_limit_min=0, value_limit_max=4)
        IProfile.make_required_input(profile, model.vars.preamble_length, "preamble",
                                     readable_name="Preamble Length Total", value_limit_min=0, value_limit_max=2097151)
        IProfile.make_required_input(profile, model.vars.preamble_pattern, "preamble",
                                     readable_name="Preamble Base Pattern", value_limit_min=0, value_limit_max=15)

        IProfile.make_required_input(profile, model.vars.modulation_type, "modem", readable_name="Modulation Type")
        IProfile.make_required_input(profile, model.vars.deviation, "modem", readable_name="Deviation",
                                     value_limit_min=0, value_limit_max=1000000, units_multiplier=UnitsMultiplier.KILO)
        IProfile.make_required_input(profile, model.vars.bitrate, "modem", readable_name="Bitrate", value_limit_min=100,
                                     value_limit_max=2000000, units_multiplier=UnitsMultiplier.KILO)
        IProfile.make_required_input(profile, model.vars.baudrate_tol_ppm, "modem", readable_name="Baudrate Tolerance",
                                     value_limit_min=0, value_limit_max=200000)
        IProfile.make_required_input(profile, model.vars.shaping_filter, "modem", readable_name="Shaping Filter")
        IProfile.make_required_input(profile, model.vars.fsk_symbol_map, "modem", readable_name="FSK symbol map")
        IProfile.make_required_input(profile, model.vars.shaping_filter_param, "modem",
                                     readable_name="Shaping Filter Parameter (BT or R)", value_limit_min=0.3,
                                     value_limit_max=2.0, fractional_digits=2)
        IProfile.make_required_input(profile, model.vars.diff_encoding_mode, "symbol_coding",
                                     readable_name="Differential Encoding Mode")
        IProfile.make_required_input(profile, model.vars.dsss_chipping_code, "symbol_coding",
                                     readable_name="DSSS Chipping Code Base", value_limit_min=long(0),
                                     value_limit_max=long(0xffffffff))
        IProfile.make_required_input(profile, model.vars.dsss_len, "symbol_coding",
                                     readable_name="DSSS Chipping Code Length", value_limit_min=0, value_limit_max=32)
        IProfile.make_required_input(profile, model.vars.dsss_spreading_factor, "symbol_coding",
                                     readable_name="DSSS Spreading Factor", value_limit_min=0, value_limit_max=100)

    def build_optional_profile_inputs(self, model, profile):
        IProfile.make_optional_input(profile, model.vars.syncword_tx_skip, "syncword",
                                     readable_name="Sync Word TX Skip", default=False)
        IProfile.make_optional_input(profile, model.vars.asynchronous_rx_enable, "modem",
                                     readable_name="Enable Asynchronous direct mode", default=False)
        IProfile.make_optional_input(profile, model.vars.symbol_encoding, "symbol_coding",
                                     readable_name="Symbol Encoding", default=model.vars.symbol_encoding.var_enum.NRZ)
        IProfile.make_optional_input(profile, model.vars.test_ber, "testing",
                                     readable_name="Reconfigure for BER testing", default=False)
        IProfile.make_optional_input(profile, model.vars.deviation_tol_ppm, 'modem', default=0,
                                     readable_name="Maximum deviation offset expected in ppm", value_limit_min=0,
                                     value_limit_max=500000)

    def build_advanced_profile_inputs(self, model, profile):
        IProfile.make_linked_io(profile, model.vars.fec_tx_enable, 'Channel_Coding', readable_name="Enable FEC on TX")
        IProfile.make_linked_io(profile, model.vars.dynamic_fec_enable, 'Channel_Coding', readable_name="Enable dynamic FEC based on syncword")
        IProfile.make_linked_io(profile, model.vars.skip2ant, 'Advanced', 'Skip 2nd antenna check with phase demod antenna diversity')
        IProfile.make_linked_io(profile, model.vars.preamble_detection_length, "preamble",
                                'Preamble Detection Length', value_limit_min=0, value_limit_max=2097151)
        IProfile.make_linked_io(profile, model.vars.lo_injection_side, "Advanced",
                                readable_name="Injection side")
        IProfile.make_linked_io(profile, model.vars.fast_detect_enable, 'Advanced',
                                   readable_name="Fast preamble detect")

    def build_hidden_profile_inputs(self, model, profile):
        IProfile.make_hidden_input(profile, model.vars.src1_range_available_minimum, "modem",
                                   readable_name="SRC range minimum", value_limit_min=125, value_limit_max=155)
        IProfile.make_hidden_input(profile, model.vars.input_decimation_filter_allow_dec3, "modem",
                                   readable_name="1=Allow input decimation filter decimate by 3 in cost function",
                                   value_limit_min=0, value_limit_max=1)
        IProfile.make_hidden_input(profile, model.vars.input_decimation_filter_allow_dec8, "modem",
                                   readable_name="1=Allow input decimation filter decimate by 8 in cost function",
                                   value_limit_min=0, value_limit_max=1)
        IProfile.make_hidden_input(profile, model.vars.demod_select, 'Advanced', readable_name="Demod Selection")
        IProfile.make_hidden_input(profile, model.vars.adc_clock_mode, "modem",
                                   readable_name="ADC Clock Mode (XO vs VCO)")
        IProfile.make_hidden_input(profile, model.vars.adc_rate_mode, 'Advanced', readable_name="ADC Rate Mode")
        IProfile.make_hidden_input(profile, model.vars.bcr_demod_en, 'Advanced',
                                   readable_name="Force BCR demod calculation", value_limit_min=0, value_limit_max=1)
        IProfile.make_hidden_input(profile, model.vars.bcr_conc_baudrate, "concurrent",
                                   readable_name="baudrate for BCR in concurrent mode", value_limit_min=0,
                                   value_limit_max=1000000)
        IProfile.make_hidden_input(profile, model.vars.bcr_conc_manchester, "concurrent",
                                   readable_name="Manchester code enable bit for BCR in concurrent mode",
                                   value_limit_min=0, value_limit_max=1)
        IProfile.make_hidden_input(profile, model.vars.bcr_conc_deviation, "concurrent",
                                   readable_name="deviation for BCR in concurrent mode", value_limit_min=0,
                                   value_limit_max=1000000)
        IProfile.make_hidden_input(profile, model.vars.synth_settling_mode, 'modem',
                                   readable_name="Synth Settling Mode")

    def build_frame_configuration_inputs(self, model, profile):
        frame_profile_inputs_common_ocelot().build_frame_inputs(model, profile)
        buildCrcInputs(model, profile)
        buildWhiteInputs(model, profile)
        buildFecInputs(model, profile)

    def build_deprecated_profile_inputs(self, model, profile):
        #These inputs were exposed on or after Ocelot Alpha 1 release, so they may be present in radioconf XML
        self.make_deprecated_input(profile, model.vars.max_tx_power_dbm)
        self.make_deprecated_input(profile, model.vars.tx_preamble_length)
        self.make_deprecated_input(profile, model.vars.manchester_mapping)
        self.make_deprecated_input(profile, model.vars.frame_coding)

    def buildRegisterOutputs(self, model, profile, family):
        build_modem_regs_ocelot(model, profile, family)

    def profile_calculate(self, model):

        manchester_mapping_input = model.profile.inputs.manchester_mapping.var_value
        frame_coding_input = model.profile.inputs.frame_coding.var_value
        symbol_encoding_input = model.profile.inputs.symbol_encoding.var_value

        #Handle deprecated vars
        if manchester_mapping_input == model.vars.manchester_mapping.var_enum.Inverted and \
            manchester_mapping_input == model.vars.symbol_encoding.var_enum.Manchester:
            #If an old CFG specified manchester + inverted, then use the new var to do the same thing
            symbol_encoding_input = model.vars.symbol_encoding.var_enum.Inv_Manchester
        elif frame_coding_input != model.vars.frame_coding.var_enum.NONE:
            #If an old CFG specified frame coding, use the new var to do the same thing
            if frame_coding_input == model.vars.frame_coding.var_enum.UART_NO_VAL:
                symbol_encoding_input = model.vars.symbol_encoding.var_enum.UART_NO_VAL
            elif frame_coding_input == model.vars.frame_coding.var_enum.UART_VAL:
                symbol_encoding_input = model.vars.symbol_encoding.var_enum.UART_NO_VAL
            elif frame_coding_input == model.vars.symbol_encoding.var_enum.MBUS_3OF6:
                symbol_encoding_input = model.vars.symbol_encoding.var_enum.MBUS_3OF6

        model.profile.inputs.symbol_encoding.var_value = symbol_encoding_input

