output "username" {
  description = "Initial administrative username to use for VM-Series."
  value       = var.username
}

output "password" {
  description = "Initial administrative password to use for VM-Series."
  value       = local.vmseries_password
  sensitive   = true
}

output "mgmt_ip_addresses" {
  description = "IP Addresses for VM-Series management (https or ssh)."
  value       = { for k, v in module.vmseries : k => v.mgmt_ip_address }
}

output "public_lb_pips" {
  description = "IP Addresses of the inbound load balancer."
  value       = module.public_lb.frontend_ip_configs
}
