module "vmseries_deployment" {
  source = "../../modules/root"

  resource_group_name = var.resource_group_name
  tags                = var.tags
  location            = var.location
  name_prefix         = var.name_prefix

  vnet_resource_group_name = var.vnet_resource_group_name
  virtual_network_name     = var.virtual_network_name
  vnet_address_space       = var.vnet_address_space
  subnets                  = var.subnets
  network_security_groups  = var.network_security_groups
  route_tables             = var.route_tables
  allow_inbound_data_ips   = var.allow_inbound_data_ips

  public_lb_name       = var.public_lb_name
  public_inbound_rules = var.public_inbound_rules
  private_lb_name      = var.private_lb_name
  private_lb_ip        = var.private_lb_ip

  bootstrap_options = var.bootstrap_options
  vmseries_sku      = var.vmseries_sku
  vmseries_version  = var.vmseries_version
  vmseries_vm_size  = var.vmseries_vm_size
  enable_zones      = var.enable_zones
  username          = var.username
  vmseries_password = var.vmseries_password
  vmseries          = var.vmseries
}