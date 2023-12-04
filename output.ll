; ModuleID = './tests/factorial/factorial.c'
source_filename = "./tests/factorial/factorial.c"

define i32 @factorial(i32 %n) {
entry:
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  %i = alloca i32, align 4
  %factorial = alloca i32, align 4
  store i32 1, ptr %factorial, align 4
  store i32 1, ptr %i, align 4
  %i2 = load i32, ptr %i, align 4
  %n3 = load i32, ptr %n1, align 4
  %int_sle = icmp sle i32 %i2, %n3
  %whilecond = icmp ne i1 %int_sle, false
  br i1 %whilecond, label %whilebody, label %endwhile

whilebody:                                        ; preds = %entry
  %factorial4 = load i32, ptr %factorial, align 4
  %i5 = load i32, ptr %i, align 4
  %int_mul = mul i32 %factorial4, %i5
  store i32 %int_mul, ptr %factorial, align 4
  %i6 = load i32, ptr %i, align 4
  %int_add = add i32 %i6, 1
  store i32 %int_add, ptr %i, align 4
  br label %endwhile

endwhile:                                         ; preds = %whilebody, %entry
  %factorial7 = load i32, ptr %factorial, align 4
  ret i32 %factorial7
}
