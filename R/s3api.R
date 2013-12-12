s3api_get_object <- function(bucket, key, outfile = NULL, check_md5 = FALSE, retry = FALSE) {
  if (is.null(outfile)) outfile <- key
  cmd <- sprintf("aws s3api get-object --bucket %s --key %s %s", bucket, key, outfile)
  retval <- system_collapse(cmd)
  if (check_md5) {
    if (sprintf('"%s"', tools::md5sum(outfile)) != retval$ETag) {
      file.remove(outfile)
      if (retry) {
        return(s3api_get_object(bucket, key, outfile, check_md5, FALSE))
      }
      stop("ETag and md5sum mismatch!")
    }
  }
  retval
}

s3api_put_object <- function(bucket, key, path, class = c("STANDARD", "REDUCED_REDUNDANCY"), check_md5 = TRUE) {
  if (check_md5) {
    md5 <- tools::md5sum(path)
    md5base64 <-  sprintf('--content-md5 "%s"',.Call("hexToRaw", md5, PACKAGE=.packageName))
  } else {
    md5base64 <- ""
  }
  class <- switch(class, "STANDARD" = "", "REDUCED_REDUNDANCY" = "--storage-class REDUCED_REDUNDANCY")
  cmd <- sprintf("aws s3api put-object --bucket %s --key %s --body %s %s %s", bucket, key, path, md5base64, class)
  retval <- system_collapse(cmd)
  if (check_md5) stopifnot(retval$ETag == sprintf('"%s"', md5))
  invisible(retval)
}

