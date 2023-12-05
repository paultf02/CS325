; ModuleID = './while.c'
source_filename = "./while.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @foo(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  br label %whilecondition

whilecondition:                                   ; preds = %endif, %entry
  %x2 = load i32, ptr %x1, align 4
  %int_slt = icmp slt i32 %x2, 10
  %whilecond = icmp ne i1 %int_slt, false
  br i1 %whilecond, label %whilebody, label %endwhile

whilebody:                                        ; preds = %whilecondition
  %x3 = load i32, ptr %x1, align 4
  %int_slt4 = icmp slt i32 %x3, 5
  %ifcond = icmp ne i1 %int_slt4, false
  br i1 %ifcond, label %then, label %else

endwhile:                                         ; preds = %whilecondition
  %x8 = load i32, ptr %x1, align 4
  ret i32 %x8

then:                                             ; preds = %whilebody
  %x5 = load i32, ptr %x1, align 4
  %int_add = add i32 %x5, 2
  store i32 %int_add, ptr %x1, align 4
  br label %endif

else:                                             ; preds = %whilebody
  %x6 = load i32, ptr %x1, align 4
  %int_add7 = add i32 %x6, 1
  store i32 %int_add7, ptr %x1, align 4
  br label %endif

endif:                                            ; preds = %else, %then
  br label %whilecondition
}
