module "vmseries_deployment" {
  source = "../../modules/eu_north"

  name_prefix = var.name_prefix

  location              = var.location
  resource_group_name   = var.resource_group_name
  create_resource_group = var.create_resource_group
  enable_zones          = var.enable_zones

  vnets = var.vnets

  load_balancers = var.load_balancers

  vmseries_version  = var.vmseries_version
  vmseries_vm_size  = var.vmseries_vm_size
  vmseries_sku      = var.vmseries_sku
  vmseries_username = var.vmseries_username
  vmseries_password = var.vmseries_password
  vmseries          = var.vmseries

  tags = var.tags
}