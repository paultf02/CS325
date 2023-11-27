; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @returns(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  br label %conditionalBlock

conditionalBlock:                                 ; preds = %thenBlock, %entry
  %x2 = load i32, ptr %x1, align 4
  %0 = icmp eq i32 %x2, 1
  br i1 %0, label %thenBlock, label %continueBlock

thenBlock:                                        ; preds = %conditionalBlock
  ret i32 0
  br label %conditionalBlock

continueBlock:                                    ; preds = %conditionalBlock
  %x3 = load i32, ptr %x1, align 4
  %1 = icmp sgt i32 %x3, 1
  br i1 %1, label %thenBlock4, label %elseBlock

thenBlock4:                                       ; preds = %continueBlock
  ret i32 1
  br label %continueBlock5

elseBlock:                                        ; preds = %continueBlock
  ret i32 2
  br label %continueBlock5

continueBlock5:                                   ; preds = %elseBlock, %thenBlock4
  ret i32 3
}
