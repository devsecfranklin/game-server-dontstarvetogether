# US Central

```sh
terraform init
terraform import  azurerm_storage_account.tfstate /subscriptions/d47f1af8-9795-4e86-bbce-da72cfd0f8ec/resourceGroups/franklin-lab/providers/Microsoft.Storage/storageAccounts/franklintfstate
terraform validate
terraform plan -out=franklin.plan
terraform apply franklin.plan
```
