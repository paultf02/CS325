; ModuleID = './example_scope.c'
source_filename = "./example_scope.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @foo() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 5, ptr %x, align 4
  store i32 2, ptr %y, align 4
  br label %whilecondition

whilecondition:                                   ; preds = %whilebody, %entry
  %y1 = load i32, ptr %y, align 4
  %int_sgt = icmp sgt i32 %y1, 0
  %whilecond = icmp ne i1 %int_sgt, false
  br i1 %whilecond, label %whilebody, label %endwhile

whilebody:                                        ; preds = %whilecondition
  %x2 = alloca i32, align 4
  store i32 2, ptr %x2, align 4
  %y3 = load i32, ptr %y, align 4
  %x4 = load i32, ptr %x2, align 4
  %int_sub = sub i32 %y3, %x4
  store i32 %int_sub, ptr %y, align 4
  br label %whilecondition

endwhile:                                         ; preds = %whilecondition
  %x5 = load i32, ptr %x, align 4
  %calltmp = call i32 @print_int(i32 %x5)
  %x6 = load i32, ptr %x, align 4
  ret i32 %x6
}
