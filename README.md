# pso-azure-Markel-849728

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
