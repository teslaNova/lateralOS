/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - gateways.S
*/
.extern _ism_handler

.macro irq_gateway int_num
	.align 16 						// to calc. just too lazy to write it down somewhere else for each function. its also much more cleaner 
	.global _irq_gateway\int_num;
	_irq_gateway\int_num:
	cli 							// stop interrupts for now

	pushl $\int_num
	jmp _ism_gateway


.endm

_ism_gateway:
	pushal
	
	pushl %gs
	pushl %fs
	pushl %es
	pushl %ds

	mov %cr4, %eax
	pushl %eax
	
	mov %cr3, %eax
	pushl %eax
	
	mov %cr2, %eax
	pushl %eax

	mov %cr0, %eax
	pushl %eax

	pushl %esp
	call _ism_handler
	add $20, %esp 					// esp, segment register

	popl %ds
	popl %es
	popl %fs
	popl %gs

	popal
	add $4, %esp 					// int_no

	iret


irq_gateway 0
irq_gateway 1
irq_gateway 2
irq_gateway 3
irq_gateway 4
irq_gateway 5
irq_gateway 6
irq_gateway 7
irq_gateway 8
irq_gateway 9
irq_gateway 10
irq_gateway 11
irq_gateway 12
irq_gateway 13
irq_gateway 14
irq_gateway 15
irq_gateway 16
irq_gateway 17
irq_gateway 18
irq_gateway 19
irq_gateway 20
irq_gateway 21
irq_gateway 22
irq_gateway 23
irq_gateway 24
irq_gateway 25
irq_gateway 26
irq_gateway 27
irq_gateway 28
irq_gateway 29
irq_gateway 30
irq_gateway 31
irq_gateway 32
irq_gateway 33
irq_gateway 34
irq_gateway 35
irq_gateway 36
irq_gateway 37
irq_gateway 38
irq_gateway 39
irq_gateway 40
irq_gateway 41
irq_gateway 42
irq_gateway 43
irq_gateway 44
irq_gateway 45
irq_gateway 46
irq_gateway 47
irq_gateway 48
irq_gateway 49
irq_gateway 50
irq_gateway 51
irq_gateway 52
irq_gateway 53
irq_gateway 54
irq_gateway 55
irq_gateway 56
irq_gateway 57
irq_gateway 58
irq_gateway 59
irq_gateway 60
irq_gateway 61
irq_gateway 62
irq_gateway 63
irq_gateway 64
irq_gateway 65
irq_gateway 66
irq_gateway 67
irq_gateway 68
irq_gateway 69
irq_gateway 70
irq_gateway 71
irq_gateway 72
irq_gateway 73
irq_gateway 74
irq_gateway 75
irq_gateway 76
irq_gateway 77
irq_gateway 78
irq_gateway 79
irq_gateway 80
irq_gateway 81
irq_gateway 82
irq_gateway 83
irq_gateway 84
irq_gateway 85
irq_gateway 86
irq_gateway 87
irq_gateway 88
irq_gateway 89
irq_gateway 90
irq_gateway 91
irq_gateway 92
irq_gateway 93
irq_gateway 94
irq_gateway 95
irq_gateway 96
irq_gateway 97
irq_gateway 98
irq_gateway 99
irq_gateway 100
irq_gateway 101
irq_gateway 102
irq_gateway 103
irq_gateway 104
irq_gateway 105
irq_gateway 106
irq_gateway 107
irq_gateway 108
irq_gateway 109
irq_gateway 110
irq_gateway 111
irq_gateway 112
irq_gateway 113
irq_gateway 114
irq_gateway 115
irq_gateway 116
irq_gateway 117
irq_gateway 118
irq_gateway 119
irq_gateway 120
irq_gateway 121
irq_gateway 122
irq_gateway 123
irq_gateway 124
irq_gateway 125
irq_gateway 126
irq_gateway 127
irq_gateway 128
irq_gateway 129
irq_gateway 130
irq_gateway 131
irq_gateway 132
irq_gateway 133
irq_gateway 134
irq_gateway 135
irq_gateway 136
irq_gateway 137
irq_gateway 138
irq_gateway 139
irq_gateway 140
irq_gateway 141
irq_gateway 142
irq_gateway 143
irq_gateway 144
irq_gateway 145
irq_gateway 146
irq_gateway 147
irq_gateway 148
irq_gateway 149
irq_gateway 150
irq_gateway 151
irq_gateway 152
irq_gateway 153
irq_gateway 154
irq_gateway 155
irq_gateway 156
irq_gateway 157
irq_gateway 158
irq_gateway 159
irq_gateway 160
irq_gateway 161
irq_gateway 162
irq_gateway 163
irq_gateway 164
irq_gateway 165
irq_gateway 166
irq_gateway 167
irq_gateway 168
irq_gateway 169
irq_gateway 170
irq_gateway 171
irq_gateway 172
irq_gateway 173
irq_gateway 174
irq_gateway 175
irq_gateway 176
irq_gateway 177
irq_gateway 178
irq_gateway 179
irq_gateway 180
irq_gateway 181
irq_gateway 182
irq_gateway 183
irq_gateway 184
irq_gateway 185
irq_gateway 186
irq_gateway 187
irq_gateway 188
irq_gateway 189
irq_gateway 190
irq_gateway 191
irq_gateway 192
irq_gateway 193
irq_gateway 194
irq_gateway 195
irq_gateway 196
irq_gateway 197
irq_gateway 198
irq_gateway 199
irq_gateway 200
irq_gateway 201
irq_gateway 202
irq_gateway 203
irq_gateway 204
irq_gateway 205
irq_gateway 206
irq_gateway 207
irq_gateway 208
irq_gateway 209
irq_gateway 210
irq_gateway 211
irq_gateway 212
irq_gateway 213
irq_gateway 214
irq_gateway 215
irq_gateway 216
irq_gateway 217
irq_gateway 218
irq_gateway 219
irq_gateway 220
irq_gateway 221
irq_gateway 222
irq_gateway 223
irq_gateway 224
irq_gateway 225
irq_gateway 226
irq_gateway 227
irq_gateway 228
irq_gateway 229
irq_gateway 230
irq_gateway 231
irq_gateway 232
irq_gateway 233
irq_gateway 234
irq_gateway 235
irq_gateway 236
irq_gateway 237
irq_gateway 238
irq_gateway 239
irq_gateway 240
irq_gateway 241
irq_gateway 242
irq_gateway 243
irq_gateway 244
irq_gateway 245
irq_gateway 246
irq_gateway 247
irq_gateway 248
irq_gateway 249
irq_gateway 250
irq_gateway 251
irq_gateway 252
irq_gateway 253
irq_gateway 254
irq_gateway 255
