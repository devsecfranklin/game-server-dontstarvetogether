# US East 2

```sh
terraform init
terraform validate
terraform plan -out=franklin.plan -var-file=franklin-lab.tfvars
terraform destroy -var-file=franklin-lab.tfvars
```
