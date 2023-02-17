# next_hop_in_ip_address

## Sept 15 testing

- Subscription ID added to TF vars in all 4 regions.
- Prefix to use, if any
- Network diagram
- Provide names of the 4 existing VNets you would like the new FW deployed to.
- What outbound route to the Internet will be used?
- Please provide the existing PIP you said you wanted to use for external facing LB.
- Please provide VM series auth key when possible.
- It was mentioned that a Virtual Network Gateway with ExpressRoute circuit to datacenter will
  allow the existing on prem Panorama to connect to management subnets. What is the status of 
  these circuits? 

## Jan 24 testing

- The resource group already exists
- The VNet already exists
- The subnets already exist. 
- The root module may need to be recreated for each region due to tight coupling and reuse of names.
- The static private IP of internal load balancer needs to be updated. 
- There are no existing PIP for public LB.

EU North Lab:
```
mgmt_ip_addresses = {
    azeeunevm001 = "20.238.77.180"
    azeeunevm002 = "20.238.77.140"
}
password = (sensitive value)
public_lb_pips = {
    lb-private = {
        ha-ports = "10.174.0.73"
    }
    lb-public  = {
        palo-lb-app1-pip = "20.223.72.127"
    }
}
username = "panadmin"
```

EU West LAB:
```
mgmt_ip_addresses = {
  "azeeuwevm001" = "20.229.201.119"
  "azeeuwevm002" = "20.229.201.187"
}
password = <sensitive>
public_lb_pips = {
  "lb-private" = {
    "ha-ports" = "10.172.0.73"
  }
  "lb-public" = {
    "palo-lb-app1-pip" = "20.82.56.94"
  }
}
username = "panadmin"
```

US Central Lab:
```
mgmt_ip_addresses = {
  "azeeuwevm001" = "40.77.20.177"
  "azeeuwevm002" = "40.77.20.202"
}
password = <sensitive>
public_lb_pips = {
  "lb-private" = {
    "ha-ports" = "10.74.0.73"
  }
  "lb-public" = {
    "palo-lb-app1-pip" = "20.84.233.131"
  }
}
username = "panadmin"
```

US East2 Lab:
```
mgmt_ip_addresses = {
  "azeeuwevm001" = "172.176.249.52"
  "azeeuwevm002" = "172.176.249.96"
}
password = <sensitive>
public_lb_pips = {
  "lb-private" = {
    "ha-ports" = "10.72.0.73"
  }
  "lb-public" = {
    "palo-lb-app1-pip" = "20.22.106.118"
  }
}
username = "panadmin"
```
