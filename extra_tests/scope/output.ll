; ModuleID = './scope.c'
source_filename = "./scope.c"

declare i32 @print_int(i32)

define i32 @scope() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 4, ptr %a, align 4
  %a1 = alloca i32, align 4
  store i32 5, ptr %a1, align 4
  %a2 = load i32, ptr %a1, align 4
  store i32 %a2, ptr %b, align 4
  %b3 = load i32, ptr %b, align 4
  %calltmp = call i32 @print_int(i32 %b3)
  %a4 = load i32, ptr %a, align 4
  %b5 = load i32, ptr %b, align 4
  %int_add = add i32 %a4, %b5
  store i32 %int_add, ptr %b, align 4
  %b6 = load i32, ptr %b, align 4
  %calltmp7 = call i32 @print_int(i32 %b6)
  %b8 = load i32, ptr %b, align 4
  ret i32 %b8
}
