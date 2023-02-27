terraform {
  required_version = ">= 1.00, < 2.0"
  required_providers {
    azurerm = {
      source  = "hashicorp/azurerm"
      version = "~> 3.7"
    }
    random = {
      source  = "hashicorp/random"
      version = "~> 3.1"
    }
  }
}

variable "prefix" {}
variable "location" {}
variable "resource_group_name" {}
variable "vnet_name" {}
variable "vnet_address_space" {}
variable "subnets" {}

# VNET RG
resource "azurerm_resource_group" "this" {
  name     = "${var.prefix}${var.resource_group_name}"
  location = var.location
}

# VNET
resource "azurerm_virtual_network" "this" {
  name                = "${var.prefix}${var.vnet_name}"
  location            = var.location
  resource_group_name = azurerm_resource_group.this.name
  address_space       = var.vnet_address_space
}

resource "azurerm_subnet" "this" {
  for_each = var.subnets

  name                 = "${var.prefix}${each.key}"
  resource_group_name  = azurerm_resource_group.this.name
  virtual_network_name = azurerm_virtual_network.this.name
  address_prefixes     = each.value
}

# LB PIP RG
resource "azurerm_resource_group" "lbpip" {
  name     = "${var.prefix}lbpip-rg"
  location = var.location
}

# LB PIP
resource "azurerm_public_ip" "this" {
  name                = "${var.prefix}lbpip"
  resource_group_name = azurerm_resource_group.lbpip.name
  location            = var.location
  sku                 = "Standard"
  allocation_method   = "Static"
}