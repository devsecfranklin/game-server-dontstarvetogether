variable "prefix" {
  type    = string
  default = "mint-neu-core-"
}
variable "location" {
  type    = string
  default = "North Europe"
}
variable "subscription_id" {
  default = null
  type    = string
}
variable "resource_group_name" {
  type    = string
  default = "rg-ngfw"
}

variable "vnet_name" {
  type = string
}

variable "vnet_address_space" {}

variable "subnets" {}