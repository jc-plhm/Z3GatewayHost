from pyradioconfig.parts.common.profiles.lynx_regs import build_modem_regs_lynx


def build_modem_regs_leopard(model,profile, family):
    build_modem_regs_lynx(model, profile, family)