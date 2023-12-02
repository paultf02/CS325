; ModuleID = './t.c'
source_filename = "./t.c"

define i32 @comp1() {
entry:
  br i1 true, label %then, label %else

then:                                             ; preds = %entry
  br label %end

else:                                             ; preds = %entry
  br label %end

end:                                              ; preds = %else, %then
  ret i32 0
}
