#!/usr/bin/awk -f

BEGIN {
  if (!FUNC) {
    print "doc.awk: no function given"
    exit 1
  }
  if (!PUB) {
    print "doc.awk: no public file given"
    exit 1
  }
  if (!PRIV) {
    print "doc.awk: no private file given"
    exit 1
  }

  # regex for function macro we want
  def = "^#define " FUNC

  print "AUTO-GENERATED!! DO NOT MODIFY"
}

# definition found
$0 ~ def {
  # flag as in def
  in_def = 1
  next
}

# end of definition
in_def && /[^\\]$/ {
  # exit when we are finished with def
  exit
}

# in def
in_def {
  # remove leading space
  gsub(/^ */, "")

  # remove trailing space and backslash
  gsub(/ *\\$/, "")

  # convert to interpolation syntax
  gsub(/_name/, "${name}")
  gsub(/_type/, "${type}")
  gsub(/ ## /, "")
}

# in def and hit function comment
in_def && /^\/\*\*$/ {
  # flag as in fn comment
  in_fn_comment = 1
}

# in definition and hit a public function
in_fn_comment && /^PUBLIC/ {
  gen(PUB)
}

# in definition and hit a private function
in_fn_comment && /^PRIVATE/ {
  gen(PRIV)
}

# in a function comment
in_fn_comment && /^\*[^//]/ {
  # remove *
  gsub(/\*/, "")
  # add comment to comment list
  comment[++nc] = $0
}

# trim leading and trailing space
function trim() {
  # remove leading space
  gsub(/^ */, " ")
  # remove trailing space and backslash
  gsub(/ *\\$/, "")
}

# print string n times
function repeat(s, n, out) {
  for (i = 1; i <= n; i++)
    printf("%s", s) >>out
  printf("\n") >>out
}

# build up function signature
function sig(s) {
  # visibility
  if (s ~ /^PUBLIC/) {
    s = "public " $0
  } else {
    s = "private " $0
  }

  # while more arguments
  while (getline > 0 && /^[^{]/) {
    trim()
    # add argument to signature
    s = s $0
  }

  # done
  return s
}

# generate doc for function
function gen(out) {
  # parse function signature
  s = sig()

  # print signature in between barrier
  repeat("=", length(s), out)
  print s >>out
  repeat("=", length(s), out)

  # first comment holds description of function
  info = comment[1]
  gsub(/:/, "", info)
  print "info:" >>out
  print " " info >>out

  # print rest of comments
  for (c = 2; c <= nc; c++)
    print comment[c] >>out

  # start over
  in_fn_comment = 0
  nc = 0
}
