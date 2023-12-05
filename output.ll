; ModuleID = './t.c'
source_filename = "./t.c"

@a = common global i32 0

define i32 @main() {
entry:
  %b = alloca i32, align 4
  store i1 true, ptr %b, align 1
  %b1 = load i32, ptr %b, align 4
  ret i32 %b1
}
