; ModuleID = './lazyeval.c'
source_filename = "./lazyeval.c"

@mutable_var = common global i32 0

define i32 @mutating_function() {
entry:
  %mutable_var = load i32, ptr @mutable_var, align 4
  %int_add = add i32 %mutable_var, 1
  store i32 %int_add, ptr @mutable_var, align 4
  ret i32 1
}

define i32 @lazyeval_and(i32 %control) {
entry:
  %control1 = alloca i32, align 4
  store i32 %control, ptr %control1, align 4
  store i32 0, ptr @mutable_var, align 4
  %control2 = load i32, ptr %control1, align 4
  %int_eq = icmp eq i32 %control2, 1
  br i1 %int_eq, label %checkrhs, label %endshortcircuit

then:                                             ; preds = %endshortcircuit
  %mutable_var = load i32, ptr @mutable_var, align 4
  ret i32 %mutable_var
  br label %endif

checkrhs:                                         ; preds = %entry
  %calltmp = call i32 @mutating_function()
  %int_to_bool_one = icmp ne i32 %calltmp, 0
  br label %endshortcircuit

endshortcircuit:                                  ; preds = %checkrhs, %entry
  %mergeandshortcircuit = phi i1 [ false, %entry ], [ %int_to_bool_one, %checkrhs ]
  %ifcond = icmp ne i1 %mergeandshortcircuit, false
  br i1 %ifcond, label %then, label %else

else:                                             ; preds = %endshortcircuit
  %mutable_var3 = load i32, ptr @mutable_var, align 4
  ret i32 %mutable_var3
  br label %endif

endif:                                            ; preds = %else, %then
  ret i32 0
}

define i32 @lazyeval_or(i32 %control) {
entry:
  %control1 = alloca i32, align 4
  store i32 %control, ptr %control1, align 4
  store i32 0, ptr @mutable_var, align 4
  %control2 = load i32, ptr %control1, align 4
  %int_eq = icmp eq i32 %control2, 1
  br i1 %int_eq, label %endshortcircuit, label %checkrhs

then:                                             ; preds = %endshortcircuit
  %mutable_var = load i32, ptr @mutable_var, align 4
  ret i32 %mutable_var
  br label %endif

checkrhs:                                         ; preds = %entry
  %calltmp = call i32 @mutating_function()
  %int_to_bool_one = icmp ne i32 %calltmp, 0
  br label %endshortcircuit

endshortcircuit:                                  ; preds = %checkrhs, %entry
  %mergeandshortcircuit = phi i1 [ true, %entry ], [ %int_to_bool_one, %checkrhs ]
  %ifcond = icmp ne i1 %mergeandshortcircuit, false
  br i1 %ifcond, label %then, label %else

else:                                             ; preds = %endshortcircuit
  %mutable_var3 = load i32, ptr @mutable_var, align 4
  ret i32 %mutable_var3
  br label %endif

endif:                                            ; preds = %else, %then
  ret i32 0
}
