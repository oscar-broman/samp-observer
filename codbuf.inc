#if defined _inc_codbuf
	#undef _inc_codbuf
#endif

#if !defined CODBUF_FUNC
	#error Define CODBUF_FUNC to be the accessor function.
#endif

#if !defined CODBUF_KB
	#error Define CODBUF_KB to be the buffer size in Kb.
#endif

#if CODBUF_KB < 1 || CODBUF_KB > 100
	#error CODBUF_KB must be between 1 and 100
#endif

#include <amx_assembly/codescan>

#define CBF(%1) %1

stock CODBUF_FUNC() {
	#undef CBF
	#define CBF(%1) %1@

	static addr;

	if (!addr) {
		new amxhdr[AMX_HDR];
		GetAmxHeader(amxhdr);
		new dat = amxhdr[AMX_HDR_DAT];
		new cod = amxhdr[AMX_HDR_COD];

		addr = (O@A_()?(((CODBUF_FUNC()),O@V_)?1:2):(O@V_)) + 4;
		addr = cod + addr - dat;

		if (UnrelocateOpcode(Opcode:ReadAmxMemory(addr)) == OP_BREAK) {
			addr += 4;
		}
	}

	return addr;
}

stock CODBUF_FUNC() {
	#include "codbuf_code"

	#if CODBUF_KB > 1
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 2
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 3
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 4
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 5
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 6
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 7
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 8
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 9
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 10
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 11
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 12
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 13
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 14
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 15
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 16
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 17
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 18
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 19
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 20
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 21
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 22
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 23
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 24
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 25
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 26
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 27
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 28
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 29
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 30
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 31
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 32
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 33
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 34
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 35
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 36
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 37
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 38
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 39
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 40
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 41
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 42
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 43
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 44
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 45
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 46
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 47
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 48
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 49
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 50
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 51
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 52
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 53
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 54
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 55
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 56
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 57
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 58
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 59
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 60
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 61
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 62
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 63
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 64
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 65
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 66
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 67
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 68
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 69
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 70
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 71
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 72
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 73
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 74
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 75
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 76
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 77
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 78
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 79
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 80
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 81
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 82
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 83
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 84
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 85
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 86
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 87
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 88
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 89
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 90
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 91
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 92
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 93
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 94
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 95
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 96
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 97
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 98
		#include "codbuf_code"
	#endif
	#if CODBUF_KB > 99
		#include "codbuf_code"
	#endif
}

#undef CODBUF_KB
#undef CODBUF_FUNC
#undef CBF
