; ModuleID = './associativity.c'
source_filename = "./associativity.c"

define i32 @associativity() {
entry:
  %n = alloca i32, align 4
  %m = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 4, ptr %n, align 4
  store i32 5, ptr %m, align 4
  %n1 = load i32, ptr %n, align 4
  %m2 = load i32, ptr %m, align 4
  %int_sub = sub i32 %n1, %m2
  %int_sub3 = sub i32 %int_sub, 3
  store i32 %int_sub3, ptr %result, align 4
  %result4 = load i32, ptr %result, align 4
  ret i32 %result4
}
