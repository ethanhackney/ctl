#!/usr/bin/awk -f

BEGIN {
  FS = ","
}

length($0) > 0 {
  cmpfn = $3 # name of comparison function
  type  = $1 # type of array
  name  = $2 # name of array struct
  dtor  = $4 # destructor

  # save our name for later
  names[++nnames,"name"] = name

  # destructor?
  names[nnames,"dtor"] = NF == 4 ? dtor : "NULL"

  printf("ARR_TEST(%s, %s, %s)\n", type, name, cmpfn)
}

END {
  print ""
  print "void"
  print "arr_test(void)"
  print "{"
  printf("\tprintf(\"CTL_ARR_DEF tests running...\\n\");\n")
  for (i = 1; i <= nnames; i++) {
    printf("\t%s_test(%s);\n", names[i,"name"], names[i,"dtor"])
    if (!SILENT)
      printf("\tprintf(\"PASSED: %s_test\\n\");\n", names[i,"name"])
  }
  printf("\tprintf(\"PASS\\n\");\n")
  print "}"
}
