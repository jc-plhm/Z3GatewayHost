from pyradioconfig.calculator_model_framework.interfaces.iprofile import ModelOutput, ModelOutputType

class sw_profile_outputs_common_sol(object):

    def build_ircal_outputs(self, model, profile):
        # Output Software Variables
        profile.outputs.append(ModelOutput(model.vars.ircal_auxndiv, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL auxndiv'))
        profile.outputs.append(ModelOutput(model.vars.ircal_auxlodiv, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL auxlodiv'))
        profile.outputs.append(ModelOutput(model.vars.ircal_rampval, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL rampval'))
        profile.outputs.append(ModelOutput(model.vars.ircal_rxamppll, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL rxamppll'))
        profile.outputs.append(ModelOutput(model.vars.ircal_rxamppa, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL rxamppa'))
        profile.outputs.append(ModelOutput(model.vars.ircal_manufconfigvalid, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL manufconfigvalid'))
        profile.outputs.append(ModelOutput(model.vars.ircal_pllconfigvalid, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL pllconfigvalid'))
        profile.outputs.append(ModelOutput(model.vars.ircal_paconfigvalid, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL paconfigvalid'))
        profile.outputs.append(ModelOutput(model.vars.ircal_useswrssiaveraging, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL useswrssiaveraging'))
        profile.outputs.append(ModelOutput(model.vars.ircal_numrssitoavg, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL numrssitoavg'))
        profile.outputs.append(ModelOutput(model.vars.ircal_throwawaybeforerssi, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL throwawaybeforerssi'))
        profile.outputs.append(ModelOutput(model.vars.ircal_delayusbeforerssi, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL delayusbeforerssi'))
        profile.outputs.append(ModelOutput(model.vars.ircal_delayusbetweenswrssi, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL delayusbetweenswrssi'))
        profile.outputs.append(ModelOutput(model.vars.ircal_bestconfig, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL bestconfig'))

        # All but one (agcrssiperiod) of these were created for backwards compatibility with RAIL 1.x - remove in RAIL 2.x
        profile.outputs.append(ModelOutput(model.vars.ircal_agcrssiperiod, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL agcrssiperiod'))
        profile.outputs.append(ModelOutput(model.vars.ircal_useswrssiaveraging2, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL useswrssiaveraging new'))
        profile.outputs.append(ModelOutput(model.vars.ircal_numrssitoavg2, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL numrssitoavg new'))
        profile.outputs.append(ModelOutput(model.vars.ircal_throwawaybeforerssi2, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL throwawaybeforerssi new'))
        profile.outputs.append(ModelOutput(model.vars.ircal_delayusbeforerssi2, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL delayusbeforerssi new'))
        profile.outputs.append(ModelOutput(model.vars.ircal_delayusbetweenswrssi2, '', ModelOutputType.SW_VAR,
                                           readable_name='IRCAL delayusbetweenswrssi new'))

        profile.outputs.append(ModelOutput(model.vars.ircal_power_level, '', ModelOutputType.RAIL_CONFIG,
                                           readable_name='IR cal power level (amplitude)'))