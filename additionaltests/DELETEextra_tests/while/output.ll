; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @foo(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  br label %conditionalBlock

conditionalBlock:                                 ; preds = %continueBlock5, %entry
  %x2 = load i32, ptr %x1, align 4
  %0 = icmp slt i32 %x2, 10
  br i1 %0, label %thenBlock, label %continueBlock

thenBlock:                                        ; preds = %conditionalBlock
  %x3 = load i32, ptr %x1, align 4
  %1 = icmp slt i32 %x3, 5
  br i1 %1, label %thenBlock4, label %elseBlock

continueBlock:                                    ; preds = %conditionalBlock
  %x8 = load i32, ptr %x1, align 4
  ret i32 %x8

thenBlock4:                                       ; preds = %thenBlock
  %x6 = load i32, ptr %x1, align 4
  %2 = add i32 %x6, 2
  store i32 %2, ptr %x1, align 4
  br label %continueBlock5

elseBlock:                                        ; preds = %thenBlock
  %x7 = load i32, ptr %x1, align 4
  %3 = add i32 %x7, 1
  store i32 %3, ptr %x1, align 4
  br label %continueBlock5

continueBlock5:                                   ; preds = %elseBlock, %thenBlock4
  br label %conditionalBlock
}
