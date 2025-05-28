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

