prefix              = "mint-neu-core-"
location            = "North Europe"
subscription_id     = "d47f1af8-9795-4e86-bbce-da72cfd0f8ec"
resource_group_name = "rg-ngfw"
vnet_name           = "vnet-ntw"
vnet_address_space  = ["10.174.0.0/20"]
subnets = {
  "sub-ngfw"       = ["10.174.1.0/27"]
  "sub-priv"       = ["10.174.0.64/27"]
  "sub-pa-untrust" = ["10.174.4.0/23"]
}