#if defined _inc_codbuf
	#undef _inc_codbuf
#endif

#if !defined CODBUF_FUNC
	#error Define CODBUF_FUNC to be the accessor function.
#endif

#if !defined CODBUF_KB
	#error Define CODBUF_KB to be the buffer size in Kb.
#endif

#if CODBUF_KB < 1 || CODBUF_KB > 20
	#error CODBUF_KB must be between 1 and 20
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
}

#undef CODBUF_KB
#undef CODBUF_FUNC
#undef CBF
