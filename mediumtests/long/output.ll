; ModuleID = './long.c'
source_filename = "./long.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @nested_blocks(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  %x2 = load i32, ptr %x1, align 4
  ret i32 %x2
}

define i1 @strange_assoc() {
entry:
  %lhs = alloca float, align 4
  %rhs_1 = alloca float, align 4
  %rhs = alloca float, align 4
  store float 0x4032AAAAA0000000, ptr %lhs, align 4
  store float 1.500000e+00, ptr %rhs_1, align 4
  %rhs_11 = load float, ptr %rhs_1, align 4
  %float_div = fdiv float %rhs_11, 7.000000e+00
  %float_div2 = fdiv float 4.000000e+00, %float_div
  store float %float_div2, ptr %rhs, align 4
  %lhs3 = load float, ptr %lhs, align 4
  %rhs4 = load float, ptr %rhs, align 4
  %float_oeq = fcmp oeq float %lhs3, %rhs4
  ret i1 %float_oeq
}

define i32 @void_param() {
entry:
  ret i32 0
}

define i32 @example_scope() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 5, ptr %x, align 4
  store i32 2, ptr %y, align 4
  br label %whilecondition

whilecondition:                                   ; preds = %endwhile5, %entry
  %y1 = load i32, ptr %y, align 4
  %int_sgt = icmp sgt i32 %y1, 0
  %whilecond = icmp ne i1 %int_sgt, false
  br i1 %whilecond, label %whilebody, label %endwhile

whilebody:                                        ; preds = %whilecondition
  %x2 = alloca i32, align 4
  %cond = alloca i1, align 1
  store i1 true, ptr %cond, align 1
  br label %whilecondition3

endwhile:                                         ; preds = %whilecondition
  %x10 = load i32, ptr %x, align 4
  ret i32 %x10

whilecondition3:                                  ; preds = %whilebody4, %whilebody
  %cond6 = load i1, ptr %cond, align 1
  %whilecond7 = icmp ne i1 %cond6, false
  br i1 %whilecond7, label %whilebody4, label %endwhile5

whilebody4:                                       ; preds = %whilecondition3
  store i32 17, ptr %x2, align 4
  store i1 false, ptr %cond, align 1
  br label %whilecondition3

endwhile5:                                        ; preds = %whilecondition3
  store i32 2, ptr %x2, align 4
  %y8 = load i32, ptr %y, align 4
  %x9 = load i32, ptr %x2, align 4
  %int_sub = sub i32 %y8, %x9
  store i32 %int_sub, ptr %y, align 4
  br label %whilecondition
}

define i32 @expr_stmt() {
entry:
  %x = alloca i32, align 4
  store i32 5, ptr %x, align 4
  %x1 = load i32, ptr %x, align 4
  ret i32 0
}

define i32 @shadowing() {
entry:
  %nested_blocks = alloca i32, align 4
  store i32 5, ptr %nested_blocks, align 4
  %nested_blocks1 = load i32, ptr %nested_blocks, align 4
  %calltmp = call i32 @nested_blocks(i32 %nested_blocks1)
  ret i32 %calltmp
}

define i1 @drive() {
entry:
  %calltmp = call i32 @nested_blocks(i32 5)
  %int_ne = icmp ne i32 %calltmp, 5
  %calltmp1 = call i32 @shadowing()
  %int_ne2 = icmp ne i32 %calltmp1, 5
  %calltmp3 = call i32 @example_scope()
  %int_ne4 = icmp ne i32 %calltmp3, 5
  %calltmp5 = call i1 @strange_assoc()
  %not = icmp ne i1 %calltmp5, true
  %calltmp6 = call i32 @expr_stmt()
  %int_ne7 = icmp ne i32 %calltmp6, 0
  %calltmp8 = call i32 @void_param()
  %int_ne9 = icmp ne i32 %calltmp8, 0
  %or = or i1 %int_ne7, %int_ne9
  %or10 = or i1 %not, %or
  %or11 = or i1 %int_ne4, %or10
  %or12 = or i1 %int_ne2, %or11
  %or13 = or i1 %int_ne, %or12
  ret i1 %or13
}
