; ModuleID = './t.c'
source_filename = "./t.c"

@glob = common global i32 0

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i1, align 1
  store i1 false, ptr %y, align 1
  store i32 1, ptr @glob, align 4
  %glob = load i32, ptr @glob, align 4
  ret i32 %glob
}
