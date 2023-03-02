variable "prefix" {}

variable "location" {
  type    = string
  default = "Central US"
}

variable "subscription_id" {
  default = null
  type    = string
}

variable "resource_group_name" {
  type = string
}

variable "vnet_name" {
  type = string
}

variable "vnet_address_space" {}

variable "subnets" {}