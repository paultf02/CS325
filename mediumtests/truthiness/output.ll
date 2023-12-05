; ModuleID = './truthiness.c'
source_filename = "./truthiness.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @foo() {
entry:
  %x = alloca float, align 4
  store float 5.000000e+00, ptr %x, align 4
  %x1 = load float, ptr %x, align 4
  %float_to_bool_one = fcmp une float %x1, 0.000000e+00
  %ifcond = icmp ne i1 %float_to_bool_one, false
  br i1 %ifcond, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 5
  br label %endif

endif:                                            ; preds = %then, %entry
  ret i32 0
}
