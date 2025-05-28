terraform {
  required_version = ">= 1.00, < 2.0"
  required_providers {
    azurerm = {
      source  = "hashicorp/azurerm"
      version = "~> 4.0"
    }
    random = {
      source  = "hashicorp/random"
      version = "~> 3.1"
    }
  }
}

provider "azurerm" {
  features {}
  subscription_id = var.subscription_id
}

variable "prefix" {}
variable "location" {}
variable "subscription_id" {
  default = null
}
variable "resource_group_name" {}
variable "vnet_name" {}
variable "vnet_address_space" {}
variable "subnets" {}

module "brownfield" {
  source = "../../modules/brownfield"

  prefix              = var.prefix
  location            = var.location
  resource_group_name = var.resource_group_name
  vnet_name           = var.vnet_name
  vnet_address_space  = var.vnet_address_space
  subnets             = var.subnets
}