/* ILCODICI.CPP	G. Toffoli

991012	aggiunto delimitatori simmetrici di stringa (« e »)
		(ma in realtà non sono delimitatori)
980914	re-definizione di LINEFEED per recupero funzioni di Foglio
980904	re-definizione di { e } per array
980831	tipo di desc_c: unsigned char -> char
960424	prima vers. Windows da ILCODICI.ASM del 28.10.88
*/

#include "stdafx.h"

#include "logo.h"

#define ILWIN

/* char. category for parse (ILANALIS.C)
     and text analysis (ILTESTO.C)  */

#ifdef ILDOS

#define C_EINI	1
#define C_DEL	2
#define C_OPER	4
#define C_WDEL 	8
#define C_LDEL	16

#endif // ILDOS

#define C_S	1	// can precede Signed number
#define C_D	2	// LOGO Delimiter
#define C_O	4	// infix Operator


// unsigned char desc_c [] = {
char desc_c [] = {

/*	classi codice 	commento */

C_D + C_S + C_WDEL + C_LDEL, // 0	^ 2
	0,				// 1	^ A
	0,				// 2	^ B
	0,				// 3	^ C
	0,				// 4	^ D
	0,				// 5	^ E
	0,				// 6	^ F
	0,				// 7	^ G
	0,				// 8	^ H  (BS)
#ifdef ILDOS
C_S,				// 9	^ I  (TAB)
#endif // ILDOS
#ifdef ILWIN
C_D + C_S + C_WDEL,	// 9	^ I  (TAB)
#endif // ILWIN
#ifdef ILDOS
	0,				// 10	^ J  (LINEFEED)
#endif // ILDOS
#ifdef ILWIN
C_D + C_WDEL + C_LDEL,// 10	^ J  (LINEFEED)
#endif // ILWIN
	0,				// 11	^ K
	0,				// 12	^ L
C_D + C_WDEL + C_LDEL, 	// 13	^ M  (ENTER + RETURN)
	0,				// 14	^ N
	0,				// 15	^ O
	0,				// 16	^ P
	0,				// 17	^ Q
	0,				// 18	^ R
	0,				// 19	^ S
	0,				// 20	^ T
	0,				// 21	^ U
	0,				// 22	^ V
	0,				// 23	^ W
	0,				// 24	^ X
	0,				// 25	^ Y
	0,				// 26	^ Z
	0,				// 27	^ [  (ESC)
	0,				// 28	^ backslash
	0,				// 29	^ ]
	0,				// 30	^ 6
	0,				// 31	^ _
C_D + C_S + C_WDEL,	// 32	spazio
	0,				// 33	!
C_D + C_S,			// 34	"
	0,				// 35	#
	0,				// 36	$
	0,				// 37	%
	0,				// 38	&
	0,				// 39	'
C_D + C_S,			// 40	(
C_D,     			// 41	)
C_D + C_O + C_S,	// 42	*
C_D + C_O + C_S,	// 43	+
	0,				// 44	,
C_D + C_O + C_S,	// 45	-
	0,				// 46	.
C_D + C_O + C_S,	// 47	/
	0,				// 48	0
	0,				// 49	1
	0,				// 50	2
	0,				// 51	3
	0,				// 52	4
	0,				// 53	5
	0,				// 54	6
	0,				// 55	7
	0,				// 56	8
	0,				// 57	9
C_D,				// 58	:
#ifdef ILDOS
	0,				// 59	;
#endif // ILDOS
#ifdef ILWIN
C_D + C_WDEL + C_LDEL,// 59	;
#endif // ILWIN
C_D + C_O + C_S,	// 60	<
C_D + C_O + C_S,	// 61	=
C_D + C_O + C_S,	// 62	>
	0,				// 63	?
	0,				// 64	@
	0,				// 65	A
	0,				// 66	B
	0,				// 67	C
	0,				// 68	D
	0,				// 69	E
	0,				// 70	F
	0,				// 71	G
	0,				// 72	H
	0,				// 73	I
	0,				// 74	J
	0,				// 75	K
	0,				// 76	L
	0,				// 77	M
	0,				// 78	N
	0,				// 79	O
	0,				// 80	P
	0,				// 81	Q
	0,				// 82	R
	0,				// 83	S
	0,				// 84	T
	0,				// 85	U
	0,				// 86	V
	0,				// 87	W
	0,				// 88	X
	0,				// 89	Y
	0,				// 90	Z
C_D + C_S,			// 91	[
C_D,				// 92	backslash
C_D + C_S,			// 93	]
C_D + C_O + C_S,	// 94	^
	0,				// 95	_
	0,				// 96	'
	0,				// 97	a
	0,				// 98	b
	0,				// 99	c
	0,				// 100	d
	0,				// 101	e
	0,				// 102	f
	0,				// 103	g
	0,				// 104	h
	0,				// 105	i
	0,				// 106	j
	0,				// 107	k
	0,				// 108	l
	0,				// 109	m
	0,				// 110	n
	0,				// 111	o
	0,				// 112	p
	0,				// 113	q
	0,				// 114	r
	0,				// 115	s
	0,				// 116	t
	0,				// 117	u
	0,				// 118	v
	0,				// 119	w
	0,				// 120	x
	0,				// 121	y
	0,				// 122	z
#ifdef ILDOS
	0,				// 123	{
#endif // ILDOS
#ifdef ILWIN
C_D + C_S,			// 123	{
#endif // ILWIN
	0,				// 124	|
#ifdef ILDOS
	0,				// 125	}
#endif // ILDOS
#ifdef ILWIN
C_D + C_S,			// 125	}
#endif // ILWIN
#ifdef ILDOS
	0,				// 126	~
#endif // ILDOS
#ifdef ILWIN
C_D + C_S + C_WDEL,	// 126	~
#endif // ILWIN
	0,				// 127	^ BS
	0,				// 128
	0,				// 129
	0,				// 130
	0,				// 131
	0,				// 132
	0,				// 133
	0,				// 134
	0,				// 135
	0,				// 136
	0,				// 137
	0,				// 138
	0,				// 139
	0,				// 140
	0,				// 141
	0,				// 142
	0,				// 143
	0,				// 144
	0,				// 145
	0,				// 146
	0,				// 147
	0,				// 148
	0,				// 149
	0,				// 150
	0,				// 151
	0,				// 152
	0,				// 153
	0,				// 154
	0,				// 155
	0,				// 156
	0,				// 157
	0,				// 158
	0,				// 159
	0,				// 160
	0,				// 161
	0,				// 162
	0,				// 163
	0,				// 164
	0,				// 165
	0,				// 166
	0,				// 167
	0,				// 168
	0,				// 169
	0,				// 170
	0,				// 171	«
	0,				// 172
	0,				// 173
	0,				// 174
	0,				// 175
	0,				// 176
	0,				// 177
	0,				// 178
	0,				// 179
	0,				// 180
	0,				// 181
	0,				// 182
	0,				// 183
	0,				// 184
	0,				// 185
	0,				// 186
	0,				// 187	»
	0,				// 188
	0,				// 189
	0,				// 190
	0,				// 191
	0,				// 192
	0,				// 193
	0,				// 194
	0,				// 195
	0,				// 196
	0,				// 197
	0,				// 198
	0,				// 199
	0,				// 200
	0,				// 201
	0,				// 202
	0,				// 203
	0,				// 204
	0,				// 205
	0,				// 206
	0,				// 207
	0,				// 208
	0,				// 209
	0,				// 210
	0,				// 211
	0,				// 212
	0,				// 213
	0,				// 214
	0,				// 215
	0,				// 216
	0,				// 217
	0,				// 218
	0,				// 219
	0,				// 220
	0,				// 221
	0,				// 222
	0,				// 223
	0,				// 224
	0,				// 225
	0,				// 226
	0,				// 227
	0,				// 228
	0,				// 229
	0,				// 230
	0,				// 231
	0,				// 232
	0,				// 233
	0,				// 234
	0,				// 235
	0,				// 236
	0,				// 237
	0,				// 238
	0,				// 239
	0,				// 240
	0,				// 241
	0,				// 242
	0,				// 243
	0,				// 244
	0,				// 245
	0,				// 246
	0,				// 247
	0,				// 248
	0,				// 249
	0,				// 250
	0,				// 251
	0,				// 252
	0,				// 253
	0,				// 254
	0,				// 255

};

