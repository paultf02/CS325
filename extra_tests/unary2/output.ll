; ModuleID = './unary2.c'
source_filename = "./unary2.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @unary2() {
entry:
  %f = alloca float, align 4
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  %b = alloca i1, align 1
  %b2 = alloca i1, align 1
  %z = alloca i32, align 4
  %z2 = alloca i32, align 4
  %X = alloca i1, align 1
  store i1 true, ptr %X, align 1
  store i32 0, ptr %i2, align 4
  store float 0.000000e+00, ptr %f, align 4
  store i32 1, ptr %i, align 4
  %f1 = load float, ptr %f, align 4
  %float_to_bool_one = fcmp une float %f1, 0.000000e+00
  %not = icmp ne i1 %float_to_bool_one, true
  %bool_int_cast = zext i1 %not to i32
  %0 = sub i32 0, %bool_int_cast
  %i3 = load i32, ptr %i, align 4
  %int_add = add i32 %0, %i3
  %int_to_bool_one = icmp ne i32 %int_add, 0
  store i32 %int_add, ptr %b, align 4
  %f4 = load float, ptr %f, align 4
  %float_to_bool_one5 = fcmp une float %f4, 0.000000e+00
  %not6 = icmp ne i1 %float_to_bool_one5, true
  %bool_int_cast7 = zext i1 %not6 to i32
  %1 = sub i32 0, %bool_int_cast7
  store i32 %1, ptr %z2, align 4
  br i1 true, label %endshortcircuit, label %checkrhs

checkrhs:                                         ; preds = %entry
  %b8 = load i1, ptr %b, align 1
  br label %endshortcircuit

endshortcircuit:                                  ; preds = %checkrhs, %entry
  %mergeandshortcircuit = phi i1 [ true, %entry ], [ %b8, %checkrhs ]
  store i1 %mergeandshortcircuit, ptr %b2, align 1
  %b29 = load i1, ptr %b2, align 1
  %bool_int_cast10 = zext i1 %b29 to i32
  store i1 %b29, ptr %i2, align 1
  %i211 = load i32, ptr %i2, align 4
  ret i32 %i211
}
