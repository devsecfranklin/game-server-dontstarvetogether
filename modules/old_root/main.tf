# Create the Resource Group.
resource "azurerm_resource_group" "this" {
  name     = "${var.name_prefix}${var.resource_group_name}"
  location = var.location

  tags = var.tags
}

# Generate a random password.
resource "random_password" "this" {
  count = var.vmseries_password == null ? 1 : 0

  length           = 16
  min_lower        = 16 - 4
  min_numeric      = 1
  min_special      = 1
  min_upper        = 1
  override_special = "_%@"
}

locals {
  vmseries_password = coalesce(var.vmseries_password, try(random_password.this[0].result, null))
}

# Create the network required for the topology.
module "vnet" {
  source = "github.com/PaloAltoNetworks/terraform-azurerm-vmseries-modules//modules/vnet?ref=v0.3.0"

  create_virtual_network  = false
  resource_group_name     = var.vnet_resource_group_name
  virtual_network_name    = var.virtual_network_name
  address_space           = var.vnet_address_space
  location                = var.location
  network_security_groups = var.network_security_groups
  route_tables            = var.route_tables
  subnets                 = var.subnets

  tags = var.tags
}

# The Inbound Load Balancer for handling the traffic from the Internet.
module "public_lb" {
  source = "github.com/PaloAltoNetworks/terraform-azurerm-vmseries-modules//modules/loadbalancer?ref=v0.3.0"

  name                = "${var.name_prefix}${var.public_lb_name}"
  location            = var.location
  resource_group_name = azurerm_resource_group.this.name
  enable_zones        = var.enable_zones

  frontend_ips = var.public_inbound_rules

  network_security_resource_group_name = var.vnet_resource_group_name
  network_security_group_name          = "public"
  network_security_allow_source_ips    = var.allow_inbound_data_ips

  tags = var.tags
}

# The Outbound Load Balancer for handling the traffic from the private networks.
module "private_lb" {
  source = "github.com/PaloAltoNetworks/terraform-azurerm-vmseries-modules//modules/loadbalancer?ref=v0.3.0"

  name                = "${var.name_prefix}${var.private_lb_name}"
  location            = var.location
  resource_group_name = azurerm_resource_group.this.name
  enable_zones        = var.enable_zones

  frontend_ips = {
    "ha-ports" = {
      subnet_id                     = module.vnet.subnet_ids["private"]
      private_ip_address_allocation = "Static"
      private_ip_address            = var.private_lb_ip
      availability_zone             = var.enable_zones ? null : "No-Zone" # For the regions without AZ support.
      rules = {
        HA_PORTS = {
          port     = 0
          protocol = "All"
        }
      }
    }
  }

  tags = var.tags
}

module "vmseries" {
  source = "github.com/PaloAltoNetworks/terraform-azurerm-vmseries-modules//modules/vmseries?ref=v0.3.0"

  for_each = var.vmseries

  location                  = var.location
  resource_group_name       = azurerm_resource_group.this.name
  name                      = "${var.name_prefix}${each.key}"
  avzone                    = try(each.value.avzone, 1)
  username                  = var.username
  password                  = local.vmseries_password
  img_version               = var.vmseries_version
  img_sku                   = var.vmseries_sku
  vm_size                   = var.vmseries_vm_size
  tags                      = var.tags
  enable_zones              = var.enable_zones
  bootstrap_options         = var.bootstrap_options
  metrics_retention_in_days = 0
  interfaces = [
    {
      name                = "${var.name_prefix}${each.key}-mgmt"
      subnet_id           = lookup(module.vnet.subnet_ids, "mgmt", null)
      create_public_ip    = true
      enable_backend_pool = false
    },
    {
      name                = "${var.name_prefix}${each.key}-public"
      subnet_id           = lookup(module.vnet.subnet_ids, "public", null)
      lb_backend_pool_id  = module.public_lb.backend_pool_id
      enable_backend_pool = true
      create_public_ip    = true
      # public_ip_address_id = azurerm_public_ip.public[each.key].id
    },
    {
      name                = "${var.name_prefix}${each.key}-private"
      subnet_id           = lookup(module.vnet.subnet_ids, "private", null)
      lb_backend_pool_id  = module.private_lb.backend_pool_id
      enable_backend_pool = true

      # Optional static private IP
      private_ip_address = try(each.value.trust_private_ip, null)
    },
  ]
}
