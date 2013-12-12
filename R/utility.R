aws.env <- new.env()
aws.env$bin <- "aws"

set_aws <- function(bin) {
  aws.env$bin <- bin
}

get_aws <- function(bin) {
  aws.env$bin
}

get_or_empty <- function(str, prefix="") {
  ifelse(length(str) > 0, sprintf("%s %s", prefix, paste(str, collapse = " ")), "")
}

system_collapse <- function(cmd) {
  tryCatch(
    fromJSON(paste(system(cmd, intern=TRUE), collapse="\n")),
    warning = function(w) stop(conditionMessage(w))
  )
}

