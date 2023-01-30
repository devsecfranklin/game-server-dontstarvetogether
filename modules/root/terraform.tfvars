# An example showing how to fill out TFVARS to deploy:
# - a common reference architecture
# - using Availbility Zones for infrastructure redundancy
# - using only a public Load Balancer for balancing inbound traffic.
# - using NAT Gateway for outgoing traffic - two NAT GWs are deployed each in a Zone matching VMseries' appliance Zone (zone resiliency architecure).

# --- GENERAL --- #
location              = "North Europe"
resource_group_name   = "common-refarch"
name_prefix           = "fosix-"
create_resource_group = true
tags = {
  "CreatedBy"   = "Palo Alto Networks"
  "CreatedWith" = "Terraform"
}
enable_zones = true



# --- VNET PART --- #
vnets = {
  "transit-vnet" = {
    create_virtual_network = true
    address_space          = ["10.0.0.0/16"]
    network_security_groups = {
      "management" = {
        rules = {
          vmseries_mgmt_allow_inbound = {
            priority                   = 100
            direction                  = "Inbound"
            access                     = "Allow"
            protocol                   = "Tcp"
            source_address_prefixes    = ["134.238.135.137", "134.238.135.14", "199.167.52.5", "113.146.139.33"]
            source_port_range          = "*"
            destination_address_prefix = "10.0.0.0/24"
            destination_port_ranges    = ["22", "443"]
          }
        }
      }
      "private" = {}
      "public"  = {}
    }
    route_tables = {
      # add blackhole tables based on subnets    
    }
    create_subnets = true
    subnets = {
      "management" = {
        address_prefixes       = ["10.0.0.0/24"]
        network_security_group = "management"
        # route_table            = ""
      }
      "private" = {
        address_prefixes = ["10.0.1.0/24"]
        # network_security_group = ""
        # route_table = ""
      }
      "public_1" = {
        address_prefixes       = ["10.0.2.0/25"]
        network_security_group = "public"
        # route_table            = ""
      }
    }
  }
}


# --- LOAD BALANCING PART --- #
load_balancers = {
  "lb-public" = {
    vnet_name                   = "transit-vnet"
    network_security_group_name = "public"
    network_security_allow_source_ips = [
      #  "x.x.x.x", # Put your own public IP address here  <-- TODO to be adjusted by the customer
      "134.238.135.137", # Tests
      "134.238.135.14",
    ]
    avzones = ["1", "2", "3"]

    frontend_ips = {
      "palo-lb-app1-pip" = {
        create_public_ip = true
        rules = {
          "balanceHttp" = {
            protocol = "Tcp"
            port     = 80
          }
          "balanceHttps" = {
            protocol = "Tcp"
            port     = 443
          }
        }
      }
    }
  }
  "lb-private" = {
    frontend_ips = {
      "ha-ports" = {
        vnet_name          = "transit-vnet"
        subnet_name        = "private"
        private_ip_address = "10.0.1.100"
        rules = {
          HA_PORTS = {
            port     = 0
            protocol = "All"
          }
        }
      }
    }
  }
}



# --- VMSERIES PART --- #
vmseries_version  = "10.2.0"
vmseries_vm_size  = "Standard_DS3_v2"
vmseries_sku      = "byol"
vmseries_password = "123QWEasd"
vmseries = {
  "vmseries-1" = {
    bootstrap_options = "type=dhcp-client"
    vnet_name         = "transit-vnet"
    avzone            = 1
    interfaces = {
      mgmt = {
        subnet_name = "management"
        create_pip  = true
        # private_ip_address = "10.0.0.10"
      }
      trust = {
        subnet_name          = "private"
        backend_pool_lb_name = "lb-private"
        # private_ip_address   = "10.0.1.10"
      }
      public = {
        subnet_name          = "public_1"
        backend_pool_lb_name = "lb-public"
        create_pip           = true
        # private_ip_address   = "10.0.2.10"
      }
    }
  }
  "vmseries-2" = {
    bootstrap_options = "type=dhcp-client"
    vnet_name         = "transit-vnet"
    avzone            = 2
    interfaces = {
      mgmt = {
        subnet_name = "management"
        create_pip  = true
        # private_ip_address = "10.0.0.11"
      }
      trust = {
        subnet_name          = "private"
        backend_pool_lb_name = "lb-private"
        # private_ip_address   = "10.0.1.11"
      }
      public = {
        subnet_name          = "public_2"
        backend_pool_lb_name = "lb-public"
        create_pip           = true
        # private_ip_address   = "10.0.2.11"
      }
    }
  }
}
