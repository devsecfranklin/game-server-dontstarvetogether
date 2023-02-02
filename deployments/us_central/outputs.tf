output "username" {
  description = "Initial administrative username to use for VM-Series."
  value       = module.vmseries_deployment.username
}

output "password" {
  description = "Initial administrative password to use for VM-Series."
  value       = module.vmseries_deployment.password
  sensitive   = true
}

output "mgmt_ip_addresses" {
  description = "IP Addresses for VM-Series management (https or ssh)."
  value       = module.vmseries_deployment.vmseries_mgmt_ip
}

output "public_lb_pips" {
  description = "IP Addresses of the inbound load balancer."
  value       = module.vmseries_deployment.frontend_ips
}
