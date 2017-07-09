uint8_t keymap[192] = {
//	1, Escape, Control, RunStop, Space, Commodore, Q, 2
	KEYCODE_1, KEYCODE_ESC, KEYCODE_TAB, KEYCODE_LEFT_ALT, KEYCODE_SPACE, KEYCODE_LEFT_CONTROL, KEYCODE_Q, KEYCODE_2,
//	3		W 		A 			LSHIFT 		Z 		S 		E 		4
	0x20, 	0x1A, 	0x04, 		0x02, 		0x1D, 	0x16, 	0x08, 	0x21,
//	5 		R 		D 			X 			C 		F 		T 		6
	0x22, 	0x15, 	0x07, 		0x1B, 		0x06,	0x09, 	0x17, 	0x23,
//	7 		Y 		G 			V 			B 		H 		U 		8
	0x24, 	0x1C, 	0x0A, 		0x19, 		0x05, 	0x0B, 	0x18, 	0x25,
//	9 		I 		J 			N 			M 		K 		O 		0
	0x26, 	0x0C, 	0x0D, 		0x11, 		0x10, 	0x0E, 	0x12, 	0x27,
//	+ 		P 		L 			, 			. 		: 		@ 		-
	0x2D, 	0x13, 	0x0F, 		0x36, 		0x37, 	0x33, 	0x2F, 	0x2E,
//	£ 		* 		; 			/ 			RSHIFT 	= 		arrowup HOME
	0x35, 	0x30, 	0x34, 		0x38, 		0xE5, 	0x32, 	0x64, 	0x4A,
//	DEL 	RETURN 	<- ->		/\ \/		f1 		f3 		f5 		f7
	0x2A, 	0x28, 	KEYCODE_ARROW_RIGHT, 		KEYCODE_ARROW_DOWN, 		0x3A,	0x3C, 	0x3E, 	0x40
};

uint8_t keymapmodifiers[192] = {
//	1 	  	Escape 	Lft Ctrol 	RunStop 	Space 	Cmmdr 	Q 		2
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	3		W 		A 			LSHIFT 		Z 		S 		E 		4
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	5 		R 		D 			X 			C 		F 		T 		6
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	7 		Y 		G 			V 			B 		H 		U 		8
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	9 		I 		J 			N 			M 		K 		O 		0
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	+ 		P 		L 			, 			. 		: 		@ 		-
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	£ 		* 		; 			/ 			RSHIFT 	= 		arrowup HOME
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0,
//	DEL 	RETURN 	<- ->		/\ \/		f1 		f3 		f5 		f7
	0, 		0, 		0, 			0, 			0, 		0, 		0, 		0
};
