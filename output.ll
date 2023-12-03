; ModuleID = './t.c'
source_filename = "./t.c"

define i32 @comp1() {
entry:
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  store i32 4, ptr %i, align 4
  %i1 = load i32, ptr %i, align 4
  %add = add i32 %i1, 1
  store i32 %add, ptr %j, align 4
  %j2 = load i32, ptr %j, align 4
  %equalitycheck = icmp eq i32 %j2, 5
  store i1 %equalitycheck, ptr %k, align 1
  %i3 = load i32, ptr %i, align 4
  %ifcond = icmp ne i32 %i3, 0
  br i1 %ifcond, label %then, label %endif

then:                                             ; preds = %entry
  store i32 10, ptr %j, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %k4 = load i32, ptr %k, align 4
  ret i32 %k4
}

define i32 @main() {
entry:
  %calltmp = call i32 @comp1()
  ret i32 %calltmp
}
