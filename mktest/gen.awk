#!/usr/bin/awk -f

BEGIN {
  FS = ","
}

length($0) > 0 {
  cmpfn = $3 # name of comparison function
  type  = $1 # type of array
  name  = $2 # name of array struct
  nil   = $4 # nil value
  nelem = $5 # number of elements in test data

  # save our name for later
  ++nnames
  names[nnames,"name"] = name
  names[nnames,"nil"] = nil
  names[nnames,"nelem"] = nelem
  names[nnames,"dtor"] = NF == 6 ? $6 : "NULL"

  printf("ARR_TEST(%s, %s, %s)\n", type, name, cmpfn)
}

END {
  print ""
  print "void"
  print "arr_test(void)"
  print "{"
  printf("\tsrand(time(NULL));\n")
  if (!SILENT)
    printf("\tprintf(\"CTL_ARR_DEF tests running...\\n\");\n")
  for (i = 1; i <= nnames; i++) {
    nelem = names[i,"nelem"]
    name = names[i,"name"]
    dtor = names[i,"dtor"]
    nil = names[i,"nil"]
    printf("\t%s_test(%s, %d, %s);\n", name, nil, nelem, dtor)
    if (!SILENT)
      printf("\tprintf(\"CTL_ARR_TEST_PASSED: %s_test\\n\");\n", names[i,"name"])
  }
  printf("\tprintf(\"CTL_ARR_TEST_PASSED\\n\");\n")
  print "}"
}
