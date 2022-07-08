from pyradioconfig.parts.ocelot.phys.Phys_Studio_Base_Standard_SUNFSK import PHYS_Studio_Base_Standard_SUNFSK_Ocelot
from pyradioconfig.calculator_model_framework.decorators.phy_decorators import do_not_inherit_phys

@do_not_inherit_phys
class PHYS_Studio_Base_Standard_SUNFSK_Sol(PHYS_Studio_Base_Standard_SUNFSK_Ocelot):

    # Owner: Casey Weltzin
    # Jira Link: https://jira.silabs.com/browse/PGSOLVALTEST-41
    def PHY_IEEE802154_SUN_FSK_920MHz_4FSK_400kbps(self, model, phy_name=None):
        phy = super().PHY_IEEE802154_SUN_FSK_920MHz_4FSK_400kbps(model, phy_name=phy_name)
        return phy

    # Owner: Casey Weltzin
    # Jira Link: https://jira.silabs.com/browse/PGSOLVALTEST-55
    def PHY_IEEE802154_SUN_FSK_896MHz_2FSK_40kbps(self, model, phy_name=None):
        phy = super().PHY_IEEE802154_SUN_FSK_896MHz_2FSK_40kbps(model, phy_name=phy_name)
        return phy

    # Owner: Casey Weltzin
    # Jira Link: https://jira.silabs.com/browse/PGSOLVALTEST-56
    def PHY_IEEE802154_SUN_FSK_450MHz_4FSK_9p6kbps(self, model, phy_name=None):
        phy = super().PHY_IEEE802154_SUN_FSK_450MHz_4FSK_9p6kbps(model, phy_name=phy_name)
        return phy

    # Owner: Casey Weltzin
    # Jira Link: https://jira.silabs.com/browse/PGSOLVALTEST-57
    def PHY_IEEE802154_SUN_FSK_450MHz_2FSK_4p8kbps(self, model, phy_name=None):
        phy = super().PHY_IEEE802154_SUN_FSK_450MHz_2FSK_4p8kbps(model, phy_name=phy_name)
        return phy