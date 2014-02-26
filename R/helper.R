wait_spot_request <- function(id) {
  while(!all((status <- get_spot_request_status(spot_request <- ec2_describe_spot_instance_requests(id))) == "fulfilled")) {
    if ("capacity-oversubscribed" %in% status) stop(paste(status, collapse=","))
    if ("cancelled" %in% status) stop(paste(status, collapse=","))
    if ("price-too-low" %in% status) stop(paste(status, collapse=","))
    Sys.sleep(10)
  }
  get_instance_id(spot_request)
}

wait_instance_running <- function(id) {
  while(!all((status <- get_instance_status(instance_status <- ec2_describe_instances(id))) == "running")) {
    if ("terminated" %in% status) stop(paste(status, collapse=","))
    Sys.sleep(10)
  }
  instance_status
}

gen_instance_spec <- function(key_name, instance_type, security_group_ids, image_id) {
  list(
    KeyName = key_name,
    InstanceType = instance_type,
    SecurityGroupIds = security_group_ids,
    ImageId = image_id
    )
}

`%[%` <- function(x, key) {
  if (is.list(x)) x[[key]] else x[key]
}
