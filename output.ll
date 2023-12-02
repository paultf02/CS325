; ModuleID = './tests/addition/addition.c'
source_filename = "./tests/addition/addition.c"

declare i32 @print_int(i32)

define i32 @addition(i32 %n, i32 %m) {
entry:
  %m2 = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  store i32 %m, ptr %m2, align 4
  %result = alloca i32, align 4
  %n3 = load i32, ptr %n1, align 4
  %m4 = load i32, ptr %m2, align 4
  %add = add i32 %n3, %m4
  store i32 %add, ptr %result, align 4
  %result5 = load i32, ptr %result, align 4
  ret i32 %result5
}
