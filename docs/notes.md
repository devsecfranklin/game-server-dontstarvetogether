# Notes

Create TAR file like so:
 `tar cvf pso-azure-Markel-849728-v0.2.tar $(git ls-files | grep -v docs | grep -v brownfield)`

Send file to customer via TAC case

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

## EU North Lab

```json
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
```

## EU West LAB

```json
mgmt_ip_addresses = {
    azeeuwevm001 = "20.224.149.128"
    azeeuwevm002 = "108.142.214.74"
}
password = (sensitive value)
public_lb_pips = {
    lb-private = {
        ha-ports = "10.172.0.73"
    }
    lb-public  = {
        palo-lb-app1-pip = "20.31.199.50"
    }
}
```

## US Central Lab

```json
mgmt_ip_addresses = {
    azeeuwevm001 = "20.9.81.243"
    azeeuwevm002 = "20.9.82.199"
}
password = (sensitive value)
public_lb_pips = {
    lb-private = {
        ha-ports = "10.74.0.73"
    }
    lb-public  = {
        palo-lb-app1-pip = "13.86.32.168"
    }
}
```

## US East2 Lab

```json
mgmt_ip_addresses = {
    azeeuwevm001 = "20.122.116.80"
    azeeuwevm002 = "20.122.116.76"
}
password = (sensitive value)
public_lb_pips = {
    lb-private = {
        ha-ports = "10.72.0.73"
    }
    lb-public  = {
        palo-lb-app1-pip = "20.96.94.140"
    }
}
```
