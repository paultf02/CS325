; ModuleID = './t.c'
source_filename = "./t.c"

@0 = common global i32 0

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @addition(i32 %n, i32 %m) {
entry:
  %m2 = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  store i32 %m, ptr %m2, align 4
  %result = alloca i32, align 4
  %cat = alloca i32, align 4
  %dog = alloca i32, align 4
}
