; ModuleID = './t.c'
source_filename = "./t.c"

@0 = common global i32 0

define i32 @addition(i32 %n, i32 %m) {
entry:
  %m2 = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  store i32 %m, ptr %m2, align 4
  %result = alloca i32, align 4
  %cat = alloca i32, align 4
  %dog = alloca i32, align 4
  store i32 42, ptr %result, align 4
  store float 0x4023666660000000, ptr %cat, align 4
  %result3 = load i32, ptr %result, align 4
  ret i32 %result3
}

define i32 @main() {
entry:
  %calltmp = call i32 @addition(i32 7, i32 8)
  ret i32 %calltmp
}
