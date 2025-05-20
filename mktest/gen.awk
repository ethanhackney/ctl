#!/usr/bin/awk -f

BEGIN {
  FS = ","

  print "struct arr_test_struct {"
  printf("\tint a;\n")
  printf("\tint b;\n")
  printf("\tint c;\n")
  print "};"

  print "int arr_test_struct_cmp(const struct arr_test_struct a, const struct arr_test_struct b)"
  print "{"
  printf("\treturn s64_cmp(a.a, b.a);\n")
  print "}"

  print "union arr_test_union {"
  printf("\tint a;\n")
  printf("\tfloat b;\n")
  printf("\tlong c;\n")
  print "};"

  print "int arr_test_union_cmp(const union arr_test_union a, const union arr_test_union b)"
  print "{"
  printf("\treturn s64_cmp(a.a, b.a);\n")
  print "}"
}

NF >= 5 {
  cmpfn = $3 # name of comparison function
  type  = $1 # type of array
  name  = $2 # name of array struct
  nil   = $4 # nil value
  nelem = $5 # number of elements in test data

  # save our name for later
  nnames++
  names[nnames,"name"] = name
  names[nnames,"nil"] = nil
  names[nnames,"nelem"] = nelem
  names[nnames,"dtor"] = NF == 6 ? $6 : "NULL"
  names[nnames,"type"] = type

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
    type = names[i,"type"]
    name = names[i,"name"]
    dtor = names[i,"dtor"]
    nil = names[i,"nil"]
    printf("\t%s_test((%s)%s, %d, %s);\n", name, type, nil, nelem, dtor)
    if (!SILENT)
      printf("\tprintf(\"CTL_ARR_TEST_PASSED: %s_test\\n\");\n", names[i,"name"])
  }
  printf("\tprintf(\"CTL_ARR_TEST_PASSED\\n\");\n")
  print "}"
}
