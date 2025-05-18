#!/usr/bin/awk -f

/^export/ {
  # remove assignment
  gsub(/=([0-9]+|('|")[^'"]*('|"))/, "")

  name = $2 # name of variable

  # update maxlen if needed (used for aligning output)
  maxlen = length(name) > maxlen ? length(name) : maxlen

  # save comment
  gsub(/.*#/, "")
  comments[name] = $0
}

END {
  # generate doc
  for (name in comments) {
    diff = maxlen - length(name)
    align = "%-" diff "s"
    fmt = "%s: " align "%s\n"
    printf(fmt, name, "", comments[name])
  }
}
