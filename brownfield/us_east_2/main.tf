terraform {
  required_version = ">= 1.00, < 2.0"
  required_providers {
    azurerm = {
      source  = "hashicorp/azurerm"
      version = "~> 3"
    }
    random = {
      source  = "hashicorp/random"
      version = "~> 3.1"
    }
  }
}

provider "azurerm" {
  features {}
}

variable "prefix" {
  default = "-use2"
}

variable "location" {
  default = "East US 2"
}

# VNET RG
resource "azurerm_resource_group" "vnet" {
  name     = "fosix${var.prefix}-vnet-rg"
  location = var.location
}

# VNET
resource "azurerm_virtual_network" "this" {
  name                = "fosix${var.prefix}-vmseries-vnet"
  location            = var.location
  resource_group_name = azurerm_resource_group.vnet.name
  address_space       = ["10.0.0.0/24"]
}

# LB PIP RG
resource "azurerm_resource_group" "lbpip" {
  name     = "fosix${var.prefix}-lbpip-rg"
  location = var.location
}

# LB PIP
resource "azurerm_public_ip" "this" {
  name                = "fosix${var.prefix}-lbpip"
  resource_group_name = azurerm_resource_group.lbpip.name
  location            = var.location
  sku                 = "Standard"
  allocation_method   = "Static"
}