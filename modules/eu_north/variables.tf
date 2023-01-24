# General
variable "tags" {
  description = "Map of tags to assign to the created resources."
  type        = map(string)
  default     = {}
}

variable "location" {
  description = "The Azure region to use."
  type        = string
}

variable "name_prefix" {
  description = "A prefix for all the names of the created Azure objects. It can end with a dash `-` character, if your naming convention prefers such separator."
  default     = ""
  type        = string
}

variable "resource_group_name" {
  description = "Name of the Resource Group to create. If not provided, it will be auto-generated."
  default     = ""
  type        = string
}

# Network
variable "vnet_resource_group_name" {
  description = "Name of an existing Resource Group hosting security VNET."
  default     = ""
  type        = string
}

variable "virtual_network_name" {
  description = "Name of the Virtual Network to create."
  type        = string
}

variable "vnet_address_space" {
  description = "CDIR for VNET."
  type        = list(string)
}

variable "subnets" {
  description = "Map of Subnets to create. Refer to the `vnet` module documentation for more information."
}

variable "network_security_groups" {
  description = "Map of Network Security Groups to create. Refer to the `vnet` module documentation for more information."
}

variable "route_tables" {
  description = "Map of Route Tables to create. Refer to the `vnet` module documentation for more information."
}


# Load Balancing
variable "allow_inbound_data_ips" {
  description = <<-EOF
    List of IP CIDR ranges (like `["23.23.23.23"]`) that are allowed to access public data interfaces of VM-Series.
    If the list is empty, the contents of `allow_inbound_mgmt_ips` are substituted instead.
  EOF
  default     = []
  type        = list(string)
}

variable "public_lb_name" {
  description = "Name of the inbound load balancer (the public-facing one)."
  type        = string
}

variable "public_inbound_rules" {
  description = "Map of objects describing frontend IP configurations and rules for the inbound load balancer."
}

variable "private_lb_name" {
  description = "Name of the outbound load balancer (the private one)."
  type        = string
}

variable "private_lb_ip" {
  description = "The private IP address to assign to the private/outbound load balancer. This IP **must** fall in the `private_subnet` network."
  type        = string
}

variable "bootstrap_options" {
  description = "Bootstrap options to pass to VM-Series instances."
  default     = ""
  type        = string
}

variable "vmseries_sku" {
  description = "VM-Series SKU - list available with `az vm image list -o table --all --publisher paloaltonetworks`"
  type        = string
}

variable "vmseries_version" {
  description = "VM-Series PAN-OS version - list available with `az vm image list -o table --all --publisher paloaltonetworks`"
  type        = string
}

variable "vmseries_vm_size" {
  description = "Azure VM size (type) to be created. Consult the *VM-Series Deployment Guide* as only a few selected sizes are supported."
  type        = string
}

variable "enable_zones" {
  description = "If false, all the VM-Series, load balancers and public IP addresses default to not to use Availability Zones (the `No-Zone` setting). It is intended for the regions that do not yet support Availability Zones."
  default     = true
  type        = bool
}

variable "username" {
  description = "Initial administrative username to use for all systems."
  default     = "panadmin"
  type        = string
}

variable "vmseries_password" {
  description = "Initial administrative password to use for all systems. Set to null for an auto-generated password."
  default     = null
  type        = string
}

variable "vmseries" {
  description = <<-EOF
  Map of virtual machines to create to run VM-Series. Keys are the individual names, values
  are the objects containing the attributes unique to that individual virtual machine:

  - `avzone`: the Azure Availability Zone identifier ("1", "2", "3"). Default is "1" in order to avoid non-HA deployments.
  - `trust_private_ip`: the static private IP to assign to the trust-side data interface (nic2). If unspecified, uses a dynamic IP.

  The hostname of each of the VM-Series will consist of a `name_prefix` concatenated with its map key.

  Basic:
  ```
  {
    "fw00" = { avzone = 1 }
    "fw01" = { avzone = 2 }
  }
  ```

  Full example:
  ```
  {
    "fw00" = {
      trust_private_ip = "192.168.0.10"
      avzone           = "1"
    }
    "fw01" = { 
      trust_private_ip = "192.168.0.11"
      avzone           = "2"
    }
  }
  ```
  EOF
}
