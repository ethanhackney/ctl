#!/usr/bin/awk -f

BEGIN {
  FS = ","

  print "struct arr_test_struct {"
  printf("\tint a;\n")
  printf("\tint b;\n")
  printf("\tint c;\n")
  print "};\n"

  print "int"
  print "arr_test_struct_cmp(const struct arr_test_struct a, const struct arr_test_struct b)"
  print "{"
  printf("\treturn s64_cmp(a.a, b.a);\n")
  print "}\n"

  print "union arr_test_union {"
  printf("\tint a;\n")
  printf("\tfloat b;\n")
  printf("\tlong c;\n")
  print "};\n"

  print "int"
  print "arr_test_union_cmp(const union arr_test_union a, const union arr_test_union b)"
  print "{"
  printf("\treturn s64_cmp(a.a, b.a);\n")
  print "}\n"
}

NF >= 5 {
  key = $1
  val = $2
  name = $3
  cmpfn = $4
  size = $5

  # save our name for later
  nnames++
  names[nnames,"name"] = name
  names[nnames,"size"] = size
  names[nnames,"dtor"] = NF == 6 ? $6 : "NULL"
  names[nnames,"key"] = key
  names[nnames,"val"] = val

  printf("HASH_SLIST_TEST(%s, %s, %s, %s)\n", key, val, name, cmpfn)
}

END {
  print ""
  print "void"
  print "hash_slist_test(void)"
  print "{"
  printf("\tsrand((unsigned int)time(NULL));\n")
  if (!SILENT)
    printf("\tprintf(\"CTL_HASH_SLIST_DEF tests running...\\n\");\n\n")
  for (i = 1; i <= nnames; i++) {
    size = names[i,"size"]
    key = names[i,"key"]
    val = names[i,"val"]
    name = names[i,"name"]
    dtor = names[i,"dtor"]
    printf("\t%s_test(%d, %s);\n", name, size, dtor)
    if (!SILENT)
      printf("\tprintf(\"CTL_HASH_SLIST_TEST_PASSED: %s_test\\n\");\n", names[i,"name"])
    print ""
  }
  print "}"
}
