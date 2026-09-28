#pragma once

// パッド入力定義
constexpr int PAD_DOWN = 0x00000001;	// ↓チェックマスク
constexpr int PAD_LEFT = 0x00000002;	// ←チェックマスク
constexpr int PAD_RIGHT = 0x00000004;	// →チェックマスク
constexpr int PAD_UP = 0x00000008;	// ↑チェックマスク
constexpr int PAD_A = 0x00000010;	// Ａボタンチェックマスク
constexpr int PAD_B = 0x00000020;	// Ｂボタンチェックマスク
constexpr int PAD_C = 0x00000040;	// Ｃボタンチェックマスク
constexpr int PAD_X = 0x00000080;	// Ｘボタンチェックマスク
constexpr int PAD_Y = 0x00000100;	// Ｙボタンチェックマスク
constexpr int PAD_Z = 0x00000200;	// Ｚボタンチェックマスク
constexpr int PAD_L = 0x00000400;	// Ｌボタンチェックマスク
constexpr int PAD_R = 0x00000800;	// Ｒボタンチェックマスク
constexpr int PAD_START = 0x00001000;	// ＳＴＡＲＴボタンチェックマスク
constexpr int PAD_M = 0x00002000;	// Ｍボタンチェックマスク
constexpr int PAD_D = 0x00004000;
constexpr int PAD_F = 0x00008000;
constexpr int PAD_G = 0x00010000;
constexpr int PAD_H = 0x00020000;
constexpr int PAD_I = 0x00040000;
constexpr int PAD_J = 0x00080000;
constexpr int PAD_K = 0x00100000;
constexpr int PAD_LL = 0x00200000;
constexpr int PAD_N = 0x00400000;
constexpr int PAD_O = 0x00800000;
constexpr int PAD_P = 0x01000000;
constexpr int PAD_RR = 0x02000000;
constexpr int PAD_S = 0x04000000;
constexpr int PAD_T = 0x08000000;
constexpr int PAD_U = 0x10000000;
constexpr int PAD_V = 0x20000000;
constexpr int PAD_W = 0x40000000;
constexpr int PAD_XX = 0x80000000;

constexpr int PAD_1 = 0x00000010;
constexpr int PAD_2 = 0x00000020;
constexpr int PAD_3 = 0x00000040;
constexpr int PAD_4 = 0x00000080;
constexpr int PAD_5 = 0x00000100;
constexpr int PAD_6 = 0x00000200;
constexpr int PAD_7 = 0x00000400;
constexpr int PAD_8 = 0x00000800;
constexpr int PAD_9 = 0x00001000;
constexpr int PAD_10 = 0x00002000;
constexpr int PAD_11 = 0x00004000;
constexpr int PAD_12 = 0x00008000;
constexpr int PAD_13 = 0x00010000;
constexpr int PAD_14 = 0x00020000;
constexpr int PAD_15 = 0x00040000;
constexpr int PAD_16 = 0x00080000;
constexpr int PAD_17 = 0x00100000;
constexpr int PAD_18 = 0x00200000;
constexpr int PAD_19 = 0x00400000;
constexpr int PAD_20 = 0x00800000;
constexpr int PAD_21 = 0x01000000;
constexpr int PAD_22 = 0x02000000;
constexpr int PAD_23 = 0x04000000;
constexpr int PAD_24 = 0x08000000;
constexpr int PAD_25 = 0x10000000;
constexpr int PAD_26 = 0x20000000;
constexpr int PAD_27 = 0x40000000;
constexpr int PAD_28 = 0x80000000;

// XInputボタン入力定義
constexpr int XINPUT_DPAD_UP = 0;			// デジタル方向ボタン上
constexpr int XINPUT_DPAD_DOWN = 1;			// デジタル方向ボタン下
constexpr int XINPUT_DPAD_LEFT = 2;			// デジタル方向ボタン左
constexpr int XINPUT_DPAD_RIGHT = 3;		// デジタル方向ボタン右
constexpr int XINPUT_START = 4;				// STARTボタン
constexpr int XINPUT_BACK = 5;				// BACKボタン
constexpr int XINPUT_LEFT_THUMB = 6;		// 左スティック押し込み
constexpr int XINPUT_RIGHT_THUMB = 7;		// 右スティック押し込み
constexpr int XINPUT_LEFT_SHOULDER = 8;		// LBボタン
constexpr int XINPUT_RIGHT_SHOULDER = 9;	// RBボタン
constexpr int XINPUT_A = 12;				// Aボタン
constexpr int XINPUT_B = 13;				// Bボタン
constexpr int XINPUT_X = 14;				// Xボタン
constexpr int XINPUT_Y = 15;				// Yボタン

// マウス入力定義
constexpr int MOUSE_LEFT = 0x0001;		// マウス左ボタン
constexpr int MOUSE_RIGHT = 0x0002;		// マウス右ボタン
constexpr int MOUSE_MIDDLE = 0x0004;		// マウス中央ボタン
constexpr int MOUSE_1 = 0x0001;			// マウス１ボタン
constexpr int MOUSE_2 = 0x0002;			// マウス２ボタン
constexpr int MOUSE_3 = 0x0004;			// マウス３ボタン
constexpr int MOUSE_4 = 0x0008;			// マウス４ボタン
constexpr int MOUSE_5 = 0x0010;			// マウス５ボタン
constexpr int MOUSE_6 = 0x0020;			// マウス６ボタン
constexpr int MOUSE_7 = 0x0040;			// マウス７ボタン
constexpr int MOUSE_8 = 0x0080;			// マウス８ボタン

// マウスのログ情報タイプ
constexpr int MOUSE_LOG_DOWN = 0;			// ボタンを押した
constexpr int MOUSE_LOG_UP = 1;				// ボタンを離した

// キー定義
constexpr int KEY_BACK = 0x0E;			// BackSpaceキー	D_DIK_BACK
constexpr int KEY_TAB = 0x0F;			// Tabキー			D_DIK_TAB
constexpr int KEY_RETURN = 0x1C;		// Enterキー		D_DIK_RETURN

constexpr int KEY_LSHIFT = 0x2A;		// 左Shiftキー		D_DIK_LSHIFT
constexpr int KEY_RSHIFT = 0x36;		// 右Shiftキー		D_DIK_RSHIFT
constexpr int KEY_LCONTROL = 0x1D;		// 左Ctrlキー		D_DIK_LCONTROL
constexpr int KEY_RCONTROL = 0x9D;		// 右Ctrlキー		D_DIK_RCONTROL
constexpr int KEY_ESCAPE = 0x01;		// Escキー			D_DIK_ESCAPE
constexpr int KEY_SPACE = 0x39;		// スペースキー		D_DIK_SPACE
constexpr int KEY_PGUP = 0xC9;			// PageUpキー		D_DIK_PGUP
constexpr int KEY_PGDN = 0xD1;			// PageDownキー		D_DIK_PGDN
constexpr int KEY_END = 0xCF;			// Endキー			D_DIK_END
constexpr int KEY_HOME = 0xC7;			// Homeキー			D_DIK_HOME
constexpr int KEY_LEFT = 0xCB;			// 左キー			D_DIK_LEFT
constexpr int KEY_UP = 0xC8;			// 上キー			D_DIK_UP
constexpr int KEY_RIGHT = 0xCD;		// 右キー			D_DIK_RIGHT
constexpr int KEY_DOWN = 0xD0;			// 下キー			D_DIK_DOWN
constexpr int KEY_INSERT = 0xD2;		// Insertキー		D_DIK_INSERT
constexpr int KEY_DELETE = 0xD3;		// Deleteキー		D_DIK_DELETE

constexpr int KEY_MINUS = 0x0C;		// －キー			D_DIK_MINUS
constexpr int KEY_YEN = 0x7D;			// ￥キー			D_DIK_YEN
constexpr int KEY_PREVTRACK = 0x90;	// ＾キー			D_DIK_PREVTRACK
constexpr int KEY_PERIOD = 0x34;		// ．キー			D_DIK_PERIOD
constexpr int KEY_SLASH = 0x35;		// ／キー			D_DIK_SLASH
constexpr int KEY_LALT = 0x38;			// 左Altキー		D_DIK_LALT
constexpr int KEY_RALT = 0xB8;			// 右Altキー		D_DIK_RALT
constexpr int KEY_SCROLL = 0x46;		// ScrollLockキー	D_DIK_SCROLL
constexpr int KEY_SEMICOLON = 0x27;	// ；キー			D_DIK_SEMICOLON
constexpr int KEY_COLON = 0x92;		// ：キー			D_DIK_COLON
constexpr int KEY_LBRACKET = 0x1A;		// ［キー			D_DIK_LBRACKET
constexpr int KEY_RBRACKET = 0x1B;		// ］キー			D_DIK_RBRACKET
constexpr int KEY_AT = 0x91;			// ＠キー			D_DIK_AT
constexpr int KEY_BACKSLASH = 0x2B;	// ＼キー			D_DIK_BACKSLASH
constexpr int KEY_COMMA = 0x33;		// ，キー			D_DIK_COMMA
constexpr int KEY_KANJI = 0x94;		// 漢字キー			D_DIK_KANJI
constexpr int KEY_CONVERT = 0x79;		// 変換キー			D_DIK_CONVERT
constexpr int KEY_NOCONVERT = 0x7B;	// 無変換キー		D_DIK_NOCONVERT
constexpr int KEY_KANA = 0x70;			// カナキー			D_DIK_KANA
constexpr int KEY_APPS = 0xDD;			// アプリケーションメニューキー		D_DIK_APPS
constexpr int KEY_CAPSLOCK = 0x3A;		// CaspLockキー		D_DIK_CAPSLOCK
constexpr int KEY_SYSRQ = 0xB7;		// PrintScreenキー	D_DIK_SYSRQ
constexpr int KEY_PAUSE = 0xC5;		// PauseBreakキー	D_DIK_PAUSE
constexpr int KEY_LWIN = 0xDB;			// 左Winキー		D_DIK_LWIN
constexpr int KEY_RWIN = 0xDC;			// 右Winキー		D_DIK_RWIN

constexpr int KEY_NUMLOCK = 0x45;		// テンキーNumLockキー		D_DIK_NUMLOCK
constexpr int KEY_NUMPAD0 = 0x52;		// テンキー０				D_DIK_NUMPAD0
constexpr int KEY_NUMPAD1 = 0x4F;		// テンキー１				D_DIK_NUMPAD1
constexpr int KEY_NUMPAD2 = 0x50;		// テンキー２				D_DIK_NUMPAD2
constexpr int KEY_NUMPAD3 = 0x51;		// テンキー３				D_DIK_NUMPAD3
constexpr int KEY_NUMPAD4 = 0x4B;		// テンキー４				D_DIK_NUMPAD4
constexpr int KEY_NUMPAD5 = 0x4C;		// テンキー５				D_DIK_NUMPAD5
constexpr int KEY_NUMPAD6 = 0x4D;		// テンキー６				D_DIK_NUMPAD6
constexpr int KEY_NUMPAD7 = 0x47;		// テンキー７				D_DIK_NUMPAD7
constexpr int KEY_NUMPAD8 = 0x48;		// テンキー８				D_DIK_NUMPAD8
constexpr int KEY_NUMPAD9 = 0x49;		// テンキー９				D_DIK_NUMPAD9
constexpr int KEY_MULTIPLY = 0x37;		// テンキー＊キー			D_DIK_MULTIPLY
constexpr int KEY_ADD = 0x4E;			// テンキー＋キー			D_DIK_ADD
constexpr int KEY_SUBTRACT = 0x4A;		// テンキー－キー			D_DIK_SUBTRACT
constexpr int KEY_DECIMAL = 0x53;		// テンキー．キー			D_DIK_DECIMAL
constexpr int KEY_DIVIDE = 0xB5;		// テンキー／キー			D_DIK_DIVIDE
constexpr int KEY_NUMPADENTER = 0x9C;	// テンキーのエンターキー	D_DIK_NUMPADENTER

constexpr int KEY_F1 = 0x3B;			// Ｆ１キー			D_DIK_F1
constexpr int KEY_F2 = 0x3C;			// Ｆ２キー			D_DIK_F2
constexpr int KEY_F3 = 0x3D;			// Ｆ３キー			D_DIK_F3
constexpr int KEY_F4 = 0x3E;			// Ｆ４キー			D_DIK_F4
constexpr int KEY_F5 = 0x3F;			// Ｆ５キー			D_DIK_F5
constexpr int KEY_F6 = 0x40;			// Ｆ６キー			D_DIK_F6
constexpr int KEY_F7 = 0x41;			// Ｆ７キー			D_DIK_F7
constexpr int KEY_F8 = 0x42;			// Ｆ８キー			D_DIK_F8
constexpr int KEY_F9 = 0x43;			// Ｆ９キー			D_DIK_F9
constexpr int KEY_F10 = 0x44;			// Ｆ１０キー		D_DIK_F10
constexpr int KEY_F11 = 0x57;			// Ｆ１１キー		D_DIK_F11
constexpr int KEY_F12 = 0x58;			// Ｆ１２キー		D_DIK_F12

constexpr int KEY_A = 0x1E;			// Ａキー			D_DIK_A
constexpr int KEY_B = 0x30;			// Ｂキー			D_DIK_B
constexpr int KEY_C = 0x2E;			// Ｃキー			D_DIK_C
constexpr int KEY_D = 0x20;			// Ｄキー			D_DIK_D
constexpr int KEY_E = 0x12;			// Ｅキー			D_DIK_E
constexpr int KEY_F = 0x21;			// Ｆキー			D_DIK_F
constexpr int KEY_G = 0x22;			// Ｇキー			D_DIK_G
constexpr int KEY_H = 0x23;			// Ｈキー			D_DIK_H
constexpr int KEY_I = 0x17;			// Ｉキー			D_DIK_I
constexpr int KEY_J = 0x24;			// Ｊキー			D_DIK_J
constexpr int KEY_K = 0x25;			// Ｋキー			D_DIK_K
constexpr int KEY_L = 0x26;			// Ｌキー			D_DIK_L
constexpr int KEY_M = 0x32;			// Ｍキー			D_DIK_M
constexpr int KEY_N = 0x31;			// Ｎキー			D_DIK_N
constexpr int KEY_O = 0x18;			// Ｏキー			D_DIK_O
constexpr int KEY_P = 0x19;			// Ｐキー			D_DIK_P
constexpr int KEY_Q = 0x10;			// Ｑキー			D_DIK_Q
constexpr int KEY_R = 0x13;			// Ｒキー			D_DIK_R
constexpr int KEY_S = 0x1F;			// Ｓキー			D_DIK_S
constexpr int KEY_T = 0x14;			// Ｔキー			D_DIK_T
constexpr int KEY_U = 0x16;			// Ｕキー			D_DIK_U
constexpr int KEY_V = 0x2F;			// Ｖキー			D_DIK_V
constexpr int KEY_W = 0x11;			// Ｗキー			D_DIK_W
constexpr int KEY_X = 0x2D;			// Ｘキー			D_DIK_X
constexpr int KEY_Y = 0x15;			// Ｙキー			D_DIK_Y
constexpr int KEY_Z = 0x2C;			// Ｚキー			D_DIK_Z

constexpr int KEY_0 = 0x0B;			// ０キー			D_DIK_0
constexpr int KEY_1 = 0x02;			// １キー			D_DIK_1
constexpr int KEY_2 = 0x03;			// ２キー			D_DIK_2
constexpr int KEY_3 = 0x04;			// ３キー			D_DIK_3
constexpr int KEY_4 = 0x05;			// ４キー			D_DIK_4
constexpr int KEY_5 = 0x06;			// ５キー			D_DIK_5
constexpr int KEY_6 = 0x07;			// ６キー			D_DIK_6
constexpr int KEY_7 = 0x08;			// ７キー			D_DIK_7
constexpr int KEY_8 = 0x09;			// ８キー			D_DIK_8
constexpr int KEY_9 = 0x0A;			// ９キー			D_DIK_9

// アスキーコントロールキーコード
constexpr int CTRL_BS = 0x08;			// バックスペース
constexpr int CTRL_TAB = 0x09;			// タブ
constexpr int CTRL_CR = 0x0d;			// 改行
constexpr int CTRL_DEL = 0x10;			// ＤＥＬキー

constexpr int CTRL_COPY = 0x03;			// コピー
constexpr int CTRL_PASTE = 0x16;			// ペースト
constexpr int CTRL_CUT = 0x18;			// カット
constexpr int CTRL_ALL = 0x01;			// 全て選択

constexpr int CTRL_LEFT = 0x1d;			// ←キー
constexpr int CTRL_RIGHT = 0x1c;			// →キー
constexpr int CTRL_UP = 0x1e;			// ↑キー
constexpr int CTRL_DOWN = 0x1f;			// ↓キー

constexpr int CTRL_HOME = 0x1a;			// ＨＯＭＥボタン
constexpr int CTRL_END = 0x19;			// ＥＮＤボタン
constexpr int CTRL_PAGE_UP = 0x17;		// ＰＡＧＥ ＵＰ
constexpr int CTRL_PAGE_DOWN = 0x15;	// ＰＡＧＥ ＤＯＷＮ

constexpr int CTRL_ESC = 0x1b;			// ＥＳＣキー
constexpr int CTRL_CMP = 0x20;			// 制御コード敷居値