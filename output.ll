; ModuleID = './t.c'
source_filename = "./t.c"

@a = common global i32 0

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %b = alloca i1, align 1
  br i1 false, label %endshortcircuit, label %checkrhs

checkrhs:                                         ; preds = %entry
  %x1 = load i32, ptr %x, align 4
  %int_add = add i32 %x1, 1
  store i32 %int_add, ptr %x, align 4
  %int_to_bool_one = icmp ne i32 %int_add, 0
  br label %endshortcircuit

endshortcircuit:                                  ; preds = %checkrhs, %entry
  %mergeandshortcircuit = phi i1 [ true, %entry ], [ %int_to_bool_one, %checkrhs ]
  store i1 %mergeandshortcircuit, ptr %b, align 1
  %b2 = load i1, ptr %b, align 1
  %bool_int_cast = zext i1 %b2 to i32
  ret i32 %bool_int_cast
}
