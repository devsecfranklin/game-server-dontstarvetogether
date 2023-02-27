prefix   = "mna-use2-core-"
location = "East US 2"
# subscription_id     = "d47f1af8-9795-4e86-bbce-da72cfd0f8ec" // Palo Home Tenant ID
resource_group_name = "rg-ngfw"
vnet_name           = "vnet-ntw"
vnet_address_space  = ["10.72.0.0/20"]
subnets = {
  "sub-ngfw"       = ["10.72.1.0/27"]
  "sub-priv"       = ["10.72.0.64/27"]
  "sub-pa-untrust" = ["10.72.4.0/23"]
}