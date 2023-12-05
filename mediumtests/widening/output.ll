; ModuleID = './widening.c'
source_filename = "./widening.c"

declare i32 @print_int(i32)

declare float @print_float(float)

define i32 @true_plus_true() {
entry:
  ret i32 2
}

define i1 @widening_casts() {
entry:
  %bin_1 = alloca float, align 4
  %bin_2 = alloca float, align 4
  %bin_3 = alloca float, align 4
  %bin_31 = alloca float, align 4
  %bin_4 = alloca float, align 4
  %bin_5 = alloca i32, align 4
  %bin_6 = alloca i32, align 4
  %bin_7 = alloca float, align 4
  %bin_8 = alloca i32, align 4
  %bin_9 = alloca i32, align 4
  %bin_1i = alloca float, align 4
  %bin_2i = alloca float, align 4
  %bin_3i = alloca float, align 4
  %bin_31i = alloca float, align 4
  %bin_4i = alloca float, align 4
  %bin_5i = alloca i32, align 4
  %bin_6i = alloca i32, align 4
  %bin_7i = alloca float, align 4
  %bin_8i = alloca i32, align 4
  %bin_9i = alloca i1, align 1
  %assign_if = alloca float, align 4
  %assign_bf = alloca float, align 4
  %assign_bi = alloca i32, align 4
  %f4 = alloca float, align 4
  %f1 = alloca float, align 4
  %i3 = alloca i32, align 4
  %i1 = alloca i32, align 4
  %bf = alloca i1, align 1
  %bt = alloca i1, align 1
  store float 4.000000e+00, ptr %f4, align 4
  store float 1.000000e+00, ptr %f1, align 4
  store i32 3, ptr %i3, align 4
  store i32 1, ptr %i1, align 4
  store i1 false, ptr %bf, align 1
  store i1 true, ptr %bt, align 1
  store float 5.000000e+00, ptr %bin_1, align 4
  store float 5.000000e+00, ptr %bin_2, align 4
  store float 4.000000e+00, ptr %bin_3, align 4
  store float 4.000000e+00, ptr %bin_31, align 4
  store float 4.000000e+00, ptr %bin_4, align 4
  store i32 4, ptr %bin_5, align 4
  store i32 4, ptr %bin_6, align 4
  store float 1.000000e+00, ptr %bin_7, align 4
  store i32 2, ptr %bin_8, align 4
  store i32 0, ptr %bin_9, align 4
  store float 1.000000e+00, ptr %assign_if, align 4
  store float 0.000000e+00, ptr %assign_bf, align 4
  store i32 1, ptr %assign_bi, align 4
  %f41 = load float, ptr %f4, align 4
  %f12 = load float, ptr %f1, align 4
  %float_add = fadd float %f41, %f12
  store float %float_add, ptr %bin_1i, align 4
  %f43 = load float, ptr %f4, align 4
  %i14 = load i32, ptr %i1, align 4
  %int_float_cast = sitofp i32 %i14 to float
  %float_add5 = fadd float %f43, %int_float_cast
  store float %float_add5, ptr %bin_2i, align 4
  %f46 = load float, ptr %f4, align 4
  %bt7 = load i1, ptr %bt, align 1
  %bool_int_cast = zext i1 %bt7 to float
  %float_add8 = fadd float %f46, %bool_int_cast
  store float %float_add8, ptr %bin_3i, align 4
  %f49 = load float, ptr %f4, align 4
  %bf10 = load i1, ptr %bf, align 1
  %bool_int_cast11 = zext i1 %bf10 to float
  %float_add12 = fadd float %f49, %bool_int_cast11
  store float %float_add12, ptr %bin_31i, align 4
  %i313 = load i32, ptr %i3, align 4
  %f114 = load float, ptr %f1, align 4
  %int_float_cast15 = sitofp i32 %i313 to float
  %float_add16 = fadd float %int_float_cast15, %f114
  store float %float_add16, ptr %bin_4i, align 4
  %i317 = load i32, ptr %i3, align 4
  %i118 = load i32, ptr %i1, align 4
  %int_add = add i32 %i317, %i118
  store i32 %int_add, ptr %bin_5i, align 4
  %i319 = load i32, ptr %i3, align 4
  %bt20 = load i1, ptr %bt, align 1
  %bool_int_cast21 = zext i1 %bt20 to i32
  %bool_int_cast22 = zext i1 %bt20 to i32
  %int_add23 = add i32 %i319, %bool_int_cast22
  store i32 %int_add23, ptr %bin_6i, align 4
  %bt24 = load i1, ptr %bt, align 1
  %f125 = load float, ptr %f1, align 4
  %bool_int_cast26 = zext i1 %bt24 to float
  %float_add27 = fadd float %bool_int_cast26, %f125
  store float %float_add27, ptr %bin_7i, align 4
  %bt28 = load i1, ptr %bt, align 1
  %i129 = load i32, ptr %i1, align 4
  %bool_int_cast30 = zext i1 %bt28 to i32
  %bool_int_cast31 = zext i1 %bt28 to i32
  %int_add32 = add i32 %bool_int_cast31, %i129
  store i32 %int_add32, ptr %bin_8i, align 4
  %bt33 = load i1, ptr %bt, align 1
  %bt34 = load i1, ptr %bt, align 1
  %bool_int_cast35 = zext i1 %bt33 to i32
  %bool_int_cast36 = zext i1 %bt34 to i32
  %int_sub = sub i32 %bool_int_cast35, %bool_int_cast36
  %int_to_bool_one = icmp ne i32 %int_sub, 0
  store i1 %int_to_bool_one, ptr %bin_9i, align 1
  %bin_137 = load float, ptr %bin_1, align 4
  %calltmp = call float @print_float(float %bin_137)
  %bin_238 = load float, ptr %bin_2, align 4
  %calltmp39 = call float @print_float(float %bin_238)
  %bin_340 = load float, ptr %bin_3, align 4
  %calltmp41 = call float @print_float(float %bin_340)
  %bin_3142 = load float, ptr %bin_31, align 4
  %calltmp43 = call float @print_float(float %bin_3142)
  %bin_444 = load float, ptr %bin_4, align 4
  %calltmp45 = call float @print_float(float %bin_444)
  %bin_546 = load i32, ptr %bin_5, align 4
  %calltmp47 = call i32 @print_int(i32 %bin_546)
  %bin_648 = load i32, ptr %bin_6, align 4
  %calltmp49 = call i32 @print_int(i32 %bin_648)
  %bin_750 = load float, ptr %bin_7, align 4
  %calltmp51 = call float @print_float(float %bin_750)
  %bin_852 = load i32, ptr %bin_8, align 4
  %calltmp53 = call i32 @print_int(i32 %bin_852)
  %bin_954 = load i32, ptr %bin_9, align 4
  %calltmp55 = call i32 @print_int(i32 %bin_954)
  %bin_198 = load float, ptr %bin_1, align 4
  %float_oeq = fcmp oeq float %bin_198, 5.000000e+00
  br i1 %float_oeq, label %checkrhs96, label %endshortcircuit97

checkrhs:                                         ; preds = %endshortcircuit57
  %assign_bi159 = load i32, ptr %assign_bi, align 4
  %int_eq160 = icmp eq i32 %assign_bi159, 1
  br label %endshortcircuit

endshortcircuit:                                  ; preds = %checkrhs, %endshortcircuit57
  %mergeandshortcircuit161 = phi i1 [ false, %entry ], [ %int_eq160, %checkrhs ]
  ret i1 %mergeandshortcircuit161

checkrhs56:                                       ; preds = %endshortcircuit59
  %assign_bf156 = load float, ptr %assign_bf, align 4
  %float_oeq157 = fcmp oeq float %assign_bf156, 1.000000e+00
  br label %endshortcircuit57

endshortcircuit57:                                ; preds = %checkrhs56, %endshortcircuit59
  %mergeandshortcircuit158 = phi i1 [ false, %entry ], [ %float_oeq157, %checkrhs56 ]
  br i1 %mergeandshortcircuit158, label %checkrhs, label %endshortcircuit

checkrhs58:                                       ; preds = %endshortcircuit61
  %assign_if153 = load float, ptr %assign_if, align 4
  %float_oeq154 = fcmp oeq float %assign_if153, 1.000000e+00
  br label %endshortcircuit59

endshortcircuit59:                                ; preds = %checkrhs58, %endshortcircuit61
  %mergeandshortcircuit155 = phi i1 [ false, %entry ], [ %float_oeq154, %checkrhs58 ]
  br i1 %mergeandshortcircuit155, label %checkrhs56, label %endshortcircuit57

checkrhs60:                                       ; preds = %endshortcircuit63
  %bin_9i151 = load i1, ptr %bin_9i, align 1
  %bool_eq = icmp eq i1 %bin_9i151, false
  br label %endshortcircuit61

endshortcircuit61:                                ; preds = %checkrhs60, %endshortcircuit63
  %mergeandshortcircuit152 = phi i1 [ false, %entry ], [ %bool_eq, %checkrhs60 ]
  br i1 %mergeandshortcircuit152, label %checkrhs58, label %endshortcircuit59

checkrhs62:                                       ; preds = %endshortcircuit65
  %bin_8i148 = load i32, ptr %bin_8i, align 4
  %int_eq149 = icmp eq i32 %bin_8i148, 2
  br label %endshortcircuit63

endshortcircuit63:                                ; preds = %checkrhs62, %endshortcircuit65
  %mergeandshortcircuit150 = phi i1 [ false, %entry ], [ %int_eq149, %checkrhs62 ]
  br i1 %mergeandshortcircuit150, label %checkrhs60, label %endshortcircuit61

checkrhs64:                                       ; preds = %endshortcircuit67
  %bin_7i145 = load float, ptr %bin_7i, align 4
  %float_oeq146 = fcmp oeq float %bin_7i145, 2.000000e+00
  br label %endshortcircuit65

endshortcircuit65:                                ; preds = %checkrhs64, %endshortcircuit67
  %mergeandshortcircuit147 = phi i1 [ false, %entry ], [ %float_oeq146, %checkrhs64 ]
  br i1 %mergeandshortcircuit147, label %checkrhs62, label %endshortcircuit63

checkrhs66:                                       ; preds = %endshortcircuit69
  %bin_6i142 = load i32, ptr %bin_6i, align 4
  %int_eq143 = icmp eq i32 %bin_6i142, 4
  br label %endshortcircuit67

endshortcircuit67:                                ; preds = %checkrhs66, %endshortcircuit69
  %mergeandshortcircuit144 = phi i1 [ false, %entry ], [ %int_eq143, %checkrhs66 ]
  br i1 %mergeandshortcircuit144, label %checkrhs64, label %endshortcircuit65

checkrhs68:                                       ; preds = %endshortcircuit71
  %bin_5i139 = load i32, ptr %bin_5i, align 4
  %int_eq140 = icmp eq i32 %bin_5i139, 4
  br label %endshortcircuit69

endshortcircuit69:                                ; preds = %checkrhs68, %endshortcircuit71
  %mergeandshortcircuit141 = phi i1 [ false, %entry ], [ %int_eq140, %checkrhs68 ]
  br i1 %mergeandshortcircuit141, label %checkrhs66, label %endshortcircuit67

checkrhs70:                                       ; preds = %endshortcircuit73
  %bin_4i136 = load float, ptr %bin_4i, align 4
  %float_oeq137 = fcmp oeq float %bin_4i136, 4.000000e+00
  br label %endshortcircuit71

endshortcircuit71:                                ; preds = %checkrhs70, %endshortcircuit73
  %mergeandshortcircuit138 = phi i1 [ false, %entry ], [ %float_oeq137, %checkrhs70 ]
  br i1 %mergeandshortcircuit138, label %checkrhs68, label %endshortcircuit69

checkrhs72:                                       ; preds = %endshortcircuit75
  %bin_31i133 = load float, ptr %bin_31i, align 4
  %float_oeq134 = fcmp oeq float %bin_31i133, 4.000000e+00
  br label %endshortcircuit73

endshortcircuit73:                                ; preds = %checkrhs72, %endshortcircuit75
  %mergeandshortcircuit135 = phi i1 [ false, %entry ], [ %float_oeq134, %checkrhs72 ]
  br i1 %mergeandshortcircuit135, label %checkrhs70, label %endshortcircuit71

checkrhs74:                                       ; preds = %endshortcircuit77
  %bin_3i130 = load float, ptr %bin_3i, align 4
  %float_oeq131 = fcmp oeq float %bin_3i130, 5.000000e+00
  br label %endshortcircuit75

endshortcircuit75:                                ; preds = %checkrhs74, %endshortcircuit77
  %mergeandshortcircuit132 = phi i1 [ false, %entry ], [ %float_oeq131, %checkrhs74 ]
  br i1 %mergeandshortcircuit132, label %checkrhs72, label %endshortcircuit73

checkrhs76:                                       ; preds = %endshortcircuit79
  %bin_2i127 = load float, ptr %bin_2i, align 4
  %float_oeq128 = fcmp oeq float %bin_2i127, 5.000000e+00
  br label %endshortcircuit77

endshortcircuit77:                                ; preds = %checkrhs76, %endshortcircuit79
  %mergeandshortcircuit129 = phi i1 [ false, %entry ], [ %float_oeq128, %checkrhs76 ]
  br i1 %mergeandshortcircuit129, label %checkrhs74, label %endshortcircuit75

checkrhs78:                                       ; preds = %endshortcircuit81
  %bin_1i124 = load float, ptr %bin_1i, align 4
  %float_oeq125 = fcmp oeq float %bin_1i124, 5.000000e+00
  br label %endshortcircuit79

endshortcircuit79:                                ; preds = %checkrhs78, %endshortcircuit81
  %mergeandshortcircuit126 = phi i1 [ false, %entry ], [ %float_oeq125, %checkrhs78 ]
  br i1 %mergeandshortcircuit126, label %checkrhs76, label %endshortcircuit77

checkrhs80:                                       ; preds = %endshortcircuit83
  %bin_9121 = load i32, ptr %bin_9, align 4
  %int_eq122 = icmp eq i32 %bin_9121, 0
  br label %endshortcircuit81

endshortcircuit81:                                ; preds = %checkrhs80, %endshortcircuit83
  %mergeandshortcircuit123 = phi i1 [ false, %entry ], [ %int_eq122, %checkrhs80 ]
  br i1 %mergeandshortcircuit123, label %checkrhs78, label %endshortcircuit79

checkrhs82:                                       ; preds = %endshortcircuit85
  %bin_8118 = load i32, ptr %bin_8, align 4
  %int_eq119 = icmp eq i32 %bin_8118, 2
  br label %endshortcircuit83

endshortcircuit83:                                ; preds = %checkrhs82, %endshortcircuit85
  %mergeandshortcircuit120 = phi i1 [ false, %entry ], [ %int_eq119, %checkrhs82 ]
  br i1 %mergeandshortcircuit120, label %checkrhs80, label %endshortcircuit81

checkrhs84:                                       ; preds = %endshortcircuit87
  %bin_7115 = load float, ptr %bin_7, align 4
  %float_oeq116 = fcmp oeq float %bin_7115, 2.000000e+00
  br label %endshortcircuit85

endshortcircuit85:                                ; preds = %checkrhs84, %endshortcircuit87
  %mergeandshortcircuit117 = phi i1 [ false, %entry ], [ %float_oeq116, %checkrhs84 ]
  br i1 %mergeandshortcircuit117, label %checkrhs82, label %endshortcircuit83

checkrhs86:                                       ; preds = %endshortcircuit89
  %bin_6112 = load i32, ptr %bin_6, align 4
  %int_eq113 = icmp eq i32 %bin_6112, 4
  br label %endshortcircuit87

endshortcircuit87:                                ; preds = %checkrhs86, %endshortcircuit89
  %mergeandshortcircuit114 = phi i1 [ false, %entry ], [ %int_eq113, %checkrhs86 ]
  br i1 %mergeandshortcircuit114, label %checkrhs84, label %endshortcircuit85

checkrhs88:                                       ; preds = %endshortcircuit91
  %bin_5110 = load i32, ptr %bin_5, align 4
  %int_eq = icmp eq i32 %bin_5110, 4
  br label %endshortcircuit89

endshortcircuit89:                                ; preds = %checkrhs88, %endshortcircuit91
  %mergeandshortcircuit111 = phi i1 [ false, %entry ], [ %int_eq, %checkrhs88 ]
  br i1 %mergeandshortcircuit111, label %checkrhs86, label %endshortcircuit87

checkrhs90:                                       ; preds = %endshortcircuit93
  %bin_4107 = load float, ptr %bin_4, align 4
  %float_oeq108 = fcmp oeq float %bin_4107, 4.000000e+00
  br label %endshortcircuit91

endshortcircuit91:                                ; preds = %checkrhs90, %endshortcircuit93
  %mergeandshortcircuit109 = phi i1 [ false, %entry ], [ %float_oeq108, %checkrhs90 ]
  br i1 %mergeandshortcircuit109, label %checkrhs88, label %endshortcircuit89

checkrhs92:                                       ; preds = %endshortcircuit95
  %bin_31104 = load float, ptr %bin_31, align 4
  %float_oeq105 = fcmp oeq float %bin_31104, 4.000000e+00
  br label %endshortcircuit93

endshortcircuit93:                                ; preds = %checkrhs92, %endshortcircuit95
  %mergeandshortcircuit106 = phi i1 [ false, %entry ], [ %float_oeq105, %checkrhs92 ]
  br i1 %mergeandshortcircuit106, label %checkrhs90, label %endshortcircuit91

checkrhs94:                                       ; preds = %endshortcircuit97
  %bin_3101 = load float, ptr %bin_3, align 4
  %float_oeq102 = fcmp oeq float %bin_3101, 5.000000e+00
  br label %endshortcircuit95

endshortcircuit95:                                ; preds = %checkrhs94, %endshortcircuit97
  %mergeandshortcircuit103 = phi i1 [ false, %entry ], [ %float_oeq102, %checkrhs94 ]
  br i1 %mergeandshortcircuit103, label %checkrhs92, label %endshortcircuit93

checkrhs96:                                       ; preds = %entry
  %bin_299 = load float, ptr %bin_2, align 4
  %float_oeq100 = fcmp oeq float %bin_299, 5.000000e+00
  br label %endshortcircuit97

endshortcircuit97:                                ; preds = %checkrhs96, %entry
  %mergeandshortcircuit = phi i1 [ false, %entry ], [ %float_oeq100, %checkrhs96 ]
  br i1 %mergeandshortcircuit, label %checkrhs94, label %endshortcircuit95
}
