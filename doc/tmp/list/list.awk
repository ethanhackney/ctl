#!/usr/bin/awk -f

BEGIN {
  # make sure we got what we needed
  if (!FUNC) {
    print "list.awk: no function given"
    exit 1
  }
  if (!UDOC) {
    print "list.awk: no user doc file given"
    exit 1
  }
  if (!IDOC) {
    print "list.awk: no internal doc file given"
    exit 1
  }
  # character used for barrier
  BARRIER = "="
  # did we find the function?
  found = 0
}

# skip until template function found
!found && $0 !~ "^#define " FUNC {
  next
}

# gotcha
!found {
  found = 1
}

# end of def?
/[^\\]$/ {
  # exit when we are finished with def
  exit 0
}

# comment?
/\*\*/ {
  in_comment = 1
  next
}

# end of comment?
in_comment && /\*\// {
  in_comment = 0
  next
}

# still in comment?
in_comment {
  nc++
  comment[nc] = parse($0)
  next
}

# function signature?
/^(PUBLIC|PRIVATE)/ {
  pub = match($0, /^PUBLIC/)
  out = pub ? UDOC : IDOC
  sig = pub ? "public" : "private"
  in_sig = 1
  next
}

# at end of sig?
in_sig && /\{/ {
  # print sig in between barriers
  barrier(length(sig), out)
  print sig >>out
  barrier(length(sig), out)

  # first comment holds description of function
  info = comment[1]
  gsub(/:/, "", info)
  print "  info:" >>out
  print " " info >>out

  # print rest of comments
  for (c = 2; c <= nc; c++)
    print comment[c] >>out

  # blank line between functions
  print "" >>out

  # start over
  in_sig = 0
  nc = 0
  next
}

# still in sig?
in_sig {
  sig = sig parse($0)
}

# parse string
function parse(s) {
  s = trim(s)
  s = interp(s)
  return s
}

# trim the fat
function trim(s) {
  gsub(/^ *\*?/, " ", s)
  gsub(/ *\\$/, "", s)
  return s
}

# convert to interpolation syntax used in docs
function interp(s) {
  gsub(/_name/, "${name}", s)
  gsub(/_type/, "${type}", s)
  gsub(/ ## /, "", s)
  return s
}

# print string n times
function barrier(n, out) {
  for (i = 1; i <= n; i++)
    printf(BARRIER, s) >>out
  printf("\n") >>out
}

END {
  if (!found) {
    print "list.awk: could not find \"#define " FUNC "\""
    exit 1
  }
}
