resource "azurerm_storage_account" "tfstate" {
  name                     = "franklintfstate"
  resource_group_name      = azurerm_resource_group.franklin_lab.name
  location                 = var.location
  account_tier             = "Standard"
  account_replication_type = "LRS"

  tags = {
    environment = "staging"
  }
}

module "brownfield" {
  source = "../../modules/brownfield"

  prefix              = var.prefix
  location            = var.location
  resource_group_name = var.resource_group_name
  vnet_name           = var.vnet_name
  vnet_address_space  = var.vnet_address_space
  subnets             = var.subnets
}
