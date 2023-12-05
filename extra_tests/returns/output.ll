; ModuleID = './returns.c'
source_filename = "./returns.c"

define i32 @returns(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  br label %whilecondition

whilecondition:                                   ; preds = %whilebody, %entry
  %x2 = load i32, ptr %x1, align 4
  %int_eq = icmp eq i32 %x2, 1
  %whilecond = icmp ne i1 %int_eq, false
  br i1 %whilecond, label %whilebody, label %endwhile

whilebody:                                        ; preds = %whilecondition
  ret i32 0
  br label %whilecondition

endwhile:                                         ; preds = %whilecondition
  %x3 = load i32, ptr %x1, align 4
  %int_sgt = icmp sgt i32 %x3, 1
  %ifcond = icmp ne i1 %int_sgt, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %endwhile
  ret i32 1
  br label %endif

else:                                             ; preds = %endwhile
  ret i32 2
  br label %endif

endif:                                            ; preds = %else, %then
  ret i32 3
}
