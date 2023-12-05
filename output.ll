; ModuleID = './t.c'
source_filename = "./t.c"

@a = common global i32 0

define i32 @main() {
entry:
  %a = load i32, ptr @a, align 4
  %int_add = add i32 %a, 1
  store i32 %int_add, ptr @a, align 4
  %a1 = load i32, ptr @a, align 4
  ret i32 %a1
}
