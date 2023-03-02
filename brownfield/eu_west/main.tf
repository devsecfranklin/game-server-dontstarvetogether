
resource "azurerm_storage_account" "tfstate" {
  name                     = "franklintfstateeuwest"
  resource_group_name      = "franklin-lab"
  location                 = var.location
  account_tier             = "Standard"
  account_replication_type = "LRS"

  tags = {
    environment = "staging"
  }
}

// import the container like so:
//terraform import azurerm_storage_container.tfstate https://franklintfstate.blob.core.windows.net/tfstateeuwest
resource "azurerm_storage_container" "tfstate_eu_west" {
  name                  = "tfstateeuwest"
  storage_account_name  = azurerm_storage_account.tfstate.name
  container_access_type = "private"
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
