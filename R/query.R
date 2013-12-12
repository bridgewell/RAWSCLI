get_spot_request_id <- function(request) {
  sapply(request$SpotInstanceRequests, function(a) a$SpotInstanceRequestId)
}

get_spot_request_status <- function(spot_request) {
  cat(spot_request$SpotInstanceRequests[[1]]$Status$Message);cat("\n")
  sapply(spot_request$SpotInstanceRequests, function(a) a$Status$Code)
}

get_instance_id <- function(spot_request) {
  sapply(spot_request$SpotInstanceRequests, function(a) a$InstanceId)
}

get_instance_status <- function(instance_request) {
  cat(instance_request$Reservations[[1]]$Instances[[1]]$state$Name);cat("\n")
  sapply(instance_request$Reservations, function(a) a$Instances[[1]]$State$Name)
}

get_instance_public_dns_name <- function(instances) {
  sapply(instances$Reservations, function(a) a$Instances[[1]]$PublicDnsName)
}

get_instance_public_ip <- function(instances) {
  sapply(instances$Reservations, function(a) a$Instances[[1]]$PublicIp)
}

get_instance_private_ip <- function(instances) {
  sapply(instances$Reservations, function(a) a$Instances[[1]]$PrivateIp)
}



