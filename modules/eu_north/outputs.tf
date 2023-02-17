output "username" {
  description = "Initial administrative username to use for VM-Series."
  value       = var.vmseries_username
}

output "password" {
  description = "Initial administrative password to use for VM-Series."
  value       = local.vmseries_password
  sensitive   = true
}

output "frontend_ips" {
  description = "IP Addresses of the load balancers."
  value       = { for k, v in module.load_balancer : k => v.frontend_ip_configs }
}

output "vmseries_mgmt_ip" {
  description = "IP addresses for the VMSeries management interface."
  value       = { for k, v in module.vmseries : k => v.mgmt_ip_address }
}