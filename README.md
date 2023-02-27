# pso-azure-Markel-849728

```sh
don't forget to set subscription `az account set --subscription mint-core-eu`
```

Verify that they do NOT want the prefix on the FW names. If they do, manually update
the name of each FW pair in all 4 regions.

To specify an existing public IP for the LB front end, I need to update like so:

```sh
create_public_ip = true # TODO <- set this to false when reusing an existing PIP
# public_ip_resource_group = "" # TODO <- in case an existing PIP is used, provide it's name and RG
# public_ip_name           = ""
```

The VM series names are not using the same prefixes as the rest of the code.

[LINK to TRD](https://docs.google.com/document/d/1ef37bCJ9LrCxbQLmfJFYlWDxAlxgWv60/edit)

## Folder structure

folder | usage
--- | ---
[modules](./modules/) | reusable code, split into regions, **not to be called directly**
[brownfield](./brownfield/) | a mock of customer environments, to be used only in lab
[deployments](./deployments/) | the actual deployment code, split into regions

## Usage

<ins>This code is flexible in terms of a target subscription. A subscription can be set directly in variables. Refer to `TODO` markers in `terraform.tfvars`.</ins>

Follow steps below to deploy VMseries infrastructure to a region (`US Central` region used as an example):

1. *(LAB ONLY)* deploy the mock of customer's environment:

        cd brownfield/us_central
        terraform init
        terraform apply
        cd -

1. deploy VMSeries infrastructure:

        cd deployments/us_central
        terraform init
        terraform apply
        
1. to read sensitive outputs, like VMSeries initial password, issue a following command:

        terraform output password

To destroy the infrastructure, instead of `terraform apply` run `terraform destroy` in the reversed order: 1<sup>st</sup> the VMSeries infrastructure, then (if deployed) the mock environment.
