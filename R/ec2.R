get_dry_run <- function(dry_run) {
  ifelse(dry_run, "--dry-run", "--no-dry-run")
}

ec2_describe_instances <- function( instance_ids = character(), 
                                    filters = character(),
                                    dry_run = FALSE) {
  dry_run <- get_dry_run(dry_run)
  instance_ids <- get_or_empty(instance_ids, "--instance-ids")
  filters <- get_or_empty(filters, "--filters")
  cmd <- sprintf("aws ec2 describe-instances %s %s %s", dry_run, instance_ids, filters)
  system_collapse(cmd)
}

ec2_describe_spot_instance_requests <- function( spot_instance_request_ids = character(0),
                                            dry_run = FALSE) {
  dry_run <- get_dry_run(dry_run)
  spot_instance_request_ids <- get_or_empty(spot_instance_request_ids, "--spot-instance-request-ids")
  cmd <- sprintf("aws ec2 describe-spot-instance-requests %s %s", dry_run, spot_instance_request_ids)
  system_collapse(cmd)
}

ec2_request_spot_instances <- function(spot_price, instance_count, launch_specification = list()) {
  write(toJSON(launch_specification), file=(json.path <- tempfile()))
  cmd <- sprintf("aws ec2 request-spot-instances --spot-price %f --instance-count %d --launch-specification %s", spot_price, instance_count, sprintf("file://%s", json.path))
  system_collapse(cmd)
}

ec2_cancel_spot_instance_requests <- function(spot_instance_request_ids, dry_run = FALSE) {
  dry_run <- get_dry_run(dry_run)
  spot_instance_request_ids <- get_or_empty(spot_instance_request_ids, "--spot-instance-request-ids")
  cmd <- sprintf("aws ec2 cancel-spot-instance-requests %s %s", dry_run, spot_instance_request_ids)
  retval <- system_collapse(cmd)
  stopifnot(sapply(retval$CancelledSpotInstanceRequests, function(a) a$State) == "cancelled")
  invisible(retval)
}

ec2_terminate_instances <- function(instance_ids, dry_run = FALSE) {
  dry_run <- get_dry_run(dry_run)
  instance_ids <- get_or_empty(instance_ids, "--instance-ids")
  cmd <- sprintf("aws ec2 terminate-instances %s %s", dry_run, instance_ids)
  retval <- system_collapse(cmd)
  stopifnot(sapply(retval$TerminatingInstances, function(a) a$CurrentState$Name) == "shutting-down")
}
