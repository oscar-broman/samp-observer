// Copyright (C) 2016 Oscar Broman
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#if defined _INC_OBSERVE
	#endinput
#endif
#define _INC_OBSERVE

#include <a_samp>

#tryinclude "amx_assembly\codescan"
#tryinclude "amx\codescan"
#tryinclude "..\amx_assembly\codescan"
#tryinclude "..\amx\codescan"

#tryinclude "amx_assembly\asm"
#tryinclude "amx\asm"
#tryinclude "..\amx_assembly\asm"
#tryinclude "..\amx\asm"

#tryinclude "amx_assembly\phys_memory"
#tryinclude "amx\phys_memory"
#tryinclude "..\amx_assembly\phys_memory"
#tryinclude "..\amx\phys_memory"

#if !defined CODESCAN_INC
	#error The latest amx_assembly is required. Get it here: github.com/Zeex/amx_assembly
#endif

#if !defined OBSERVE_CODE_SIZE
	#define OBSERVE_CODE_SIZE 20480
#endif

#if OBSERVE_CODE_SIZE % 1024 != 0
	#error OBSERVE_CODE_SIZE must be a multiple of 1024
#endif

// Allocate space on COD to avoid getting burned by the bounds check in RETN
#define CODBUF_KB    (OBSERVE_CODE_SIZE / 1024)
#define CODBUF_FUNC  CBF(GetObCode)
#include "codbuf"

forward ObModifyVar_FoundCallback(m[CodeScanner]);

static stock
	s_ErrorHandler,
	s_InObserveCallback = false,
	s_Code,
	s_CodeOffset = 0,
	s_CodeOffsetAtStart,
	s_Func,
	s_AmxHdr[AMX_HDR],
	s_Base,
	s_Stk,
	s_Hea,
	s_Addr,
	s_Size,
	s_InPossibleAssignment;


static stock ObInit() {
	s_ErrorHandler = GetPublicAddressFromName("ObHandleAsmError");
	s_Base = GetAmxBaseAddress();
	GetAmxHeader(s_AmxHdr);
	s_Code = GetObCode();

	new publics = GetNumPublics(s_AmxHdr);

	for (new i = 0; i < publics; i++) {
		new name[32];

		if (GetPublicNameFromIndex(i, name) && !strcmp("dynR", name, _, 4)) {
			// Register the dynamic var
			CallLocalFunction(name, "");
			name[3] = 'U';
			// Set the initial first value
			CallLocalFunction(name, "");
		}
	}
}

#define ObserveVar(%1,%2) \
	ObserveVarPtr(ref(_:%1), addressofex(%2(%1,%1)))

#define addressofex(%1) \
	(O@A_()?(((%1),O@V_)?1:2):(O@V_))

#define observer%0\32;%1(%2,%3) \
	forward %1(%2,%3); public %1(%2,%3)

stock ObserveVarPtr(addr, func) {
	if (!s_InObserveCallback) {
		printf("ERROR: ObserveVar must be called within InstallObservers.");
		return;
	}

	s_Func = func;
	s_CodeOffsetAtStart = s_CodeOffset;

	new scanner[CodeScanner];
	CodeScanInit(scanner);

	new csm1[CodeScanMatcher];
	CodeScanMatcherInit(csm1, &ObModifyVar_FoundCallback);
	CodeScanMatcherPattern(csm1,
		OP(STOR_PRI, addr)
	);
	CodeScanAddMatcher(scanner, csm1);

	new csm2[CodeScanMatcher];
	CodeScanMatcherInit(csm2, &ObModifyVar_FoundCallback);
	CodeScanMatcherPattern(csm2,
		OP(STOR_ALT, addr)
	);
	CodeScanAddMatcher(scanner, csm2);

	new csm3[CodeScanMatcher];
	CodeScanMatcherInit(csm3, &ObModifyVar_FoundCallback);
	CodeScanMatcherPattern(csm3,
		OP(INC, addr)
	);
	CodeScanAddMatcher(scanner, csm3);

	new csm4[CodeScanMatcher];
	CodeScanMatcherInit(csm4, &ObModifyVar_FoundCallback);
	CodeScanMatcherPattern(csm4,
		OP(DEC, addr)
	);
	CodeScanAddMatcher(scanner, csm4);

	CodeScanRun(scanner);
}

public ObHandleAsmError(ctx[AsmContext]) {
	if (AsmGetError(ctx) == ASM_ERROR_SPACE) {
		printf("(observe.inc) ERROR: Buffer size exceeded. Increase OBSERVE_CODE_SIZE.");

		#emit HALT 1
	} else {
		printf("AsmError in observe.inc: %d", _:AsmGetError(ctx));
	}
}

public ObModifyVar_FoundCallback(m[CodeScanner]) {
	static alt, pri;

	new dat = s_AmxHdr[AMX_HDR_DAT];
	new cod = s_AmxHdr[AMX_HDR_COD];
	new cip = m[CodeScanMatch_cip];
	new ctx[AsmContext];

	// Ignore matches inside the generated code
	if (cip >= s_Code + s_CodeOffsetAtStart && cip < s_Code + OBSERVE_CODE_SIZE - s_CodeOffsetAtStart) {
		return;
	}

	// Write to the allocated space in COD
	AsmInitPtr(ctx, s_Code + s_CodeOffset, OBSERVE_CODE_SIZE - s_CodeOffset);
	AsmSetErrorHandler(ctx, s_ErrorHandler);

	new val = ReadAmxMemory(cip + 4); // Get the variable's address (`addr` in ObserveVarPtr)
	new faddr = s_Base + cod + s_Func; // Absolute address to `func` in ObserveVarPtr
	new retaddr = s_Base + dat + cip + 8;

	// Save these so we can restore them later
	@emit stor.pri ref(pri)
	@emit stor.alt ref(alt)

	@emit load.pri ref(pri)
	@emit load.alt val

	// Update the value, while keeping the previous value in alt
	switch (UnrelocateOpcode(Opcode:ReadAmxMemory(cip))) {
		case OP_INC: {
			@emit move.pri
			@emit inc.pri
			@emit stor.pri val
		}
		case OP_DEC: {
			@emit move.pri
			@emit dec.pri
			@emit stor.pri val
		}
		case OP_STOR_PRI: {
			@emit stor.pri val
		}
		case OP_STOR_ALT: {
			@emit push.alt
			@emit load.alt ref(alt)
			@emit stor.alt val
			@emit move.pri
			@emit pop.alt
		}
	}

	@emit push ref(pri)
	@emit push ref(alt)

	// Invoke callback(prev, curr)
	@emit push.pri
	@emit push.alt
	@emit push.num.args 2
	@emit call faddr

	// Restore alt and pri
	@emit pop.alt
	@emit pop.pri

	// Jump back to the location we hooked
	@emit jump retaddr

	WriteAmxMemory(cip, _:RelocateOpcode(OP_JUMP));
	WriteAmxMemory(cip + 4, s_Base + dat + s_Code + s_CodeOffset);

	s_CodeOffset += ctx[AsmContext_buffer_offset];
}

forward ObHandleAsmError(ctx[AsmContext]);
forward ObArrayScan_FoundCallback(m[CodeScanner]);

#define ObserveArray(%1,%2) \
	ObserveArrayPtr(ref(_:%1), sizeof(%1), addressofex(%2(%1[0],%1[0],0)))

stock ObserveArrayPtr(addr, size, func) {
	if (!s_InObserveCallback) {
		printf("ERROR: ObserveArray must be called within InstallObservers.");
		return;
	}

	new scanner[CodeScanner];
	new csm[CodeScanMatcher];
	CodeScanInit(scanner);

	s_Addr = addr;
	s_Func = func;
	s_Size = size;
	s_CodeOffsetAtStart = s_CodeOffset;

	CodeScanMatcherInit(csm, &ObArrayScan_FoundCallback);
	csm[CodeScanMatcher_flags] = SCANNER_MATCH_ALL;
	CodeScanMatcherPattern(csm, OP(CONST_PRI, addr));
	CodeScanAddMatcher(scanner, csm);

	s_InPossibleAssignment = false;

	CodeScanRun(scanner);
}

// The code assumes that: CONST.alt/pri [array] followed by STOR.I means an array edit, iff the stack and heap never dips below their values at the time of the CONST.alt/pri op.
// This is a dangerous assumption and will most likely lead to false positives, therefore a runtime check is added incase something is falsely detected as an edit.
public ObArrayScan_FoundCallback(m[CodeScanner]) {
	static s_prev_cip;

	new cip = m[CodeScanMatch_cip];
	new stk = m[CodeScanMatch_stack];
	new hea = m[CodeScanMatch_heap];
	new Opcode:op = UnrelocateOpcode(Opcode:ReadAmxMemory(cip));
	new oper = ReadAmxMemory(cip + 4);

	if (!s_InPossibleAssignment) {
		if ((op == OP_CONST_PRI || op == OP_CONST_ALT) && oper == s_Addr) {
			s_Stk = stk;
			s_Hea = hea;

			s_InPossibleAssignment = true;
		}
	} else {
		if (stk < s_Stk) {
			s_InPossibleAssignment = false;
		} else if (hea < s_Hea) {
			s_InPossibleAssignment = false;
		} else switch (op) {
			case OP_STOR_I: {
				InstallArrayHook(m, s_prev_cip);
				s_InPossibleAssignment = false;
			}
			case OP_RETN, OP_RET: {
				s_InPossibleAssignment = false;
			}
		}
	}

	s_prev_cip = cip;
}

static stock InstallArrayHook(m[CodeScanner], prev_cip) {
	new dat = s_AmxHdr[AMX_HDR_DAT];
	new cod = s_AmxHdr[AMX_HDR_COD];
	new cip = m[CodeScanMatch_cip];
	new ctx[AsmContext];

	// Ignore matches inside the generated code
	if (cip >= s_Code + s_CodeOffsetAtStart && cip < s_Code + OBSERVE_CODE_SIZE - s_CodeOffsetAtStart) {
		return;
	}

	// Write to the allocated space in COD
	AsmInitPtr(ctx, s_Code + s_CodeOffset, OBSERVE_CODE_SIZE - s_CodeOffset);
	AsmSetErrorHandler(ctx, s_ErrorHandler);

	// We're in STOR.I so we'll need to use the previous opcode to fit the JUMP instruction
	AsmEmitOpcode(ctx, UnrelocateOpcode(Opcode:ReadAmxMemory(prev_cip)));
	if (cip - prev_cip == 8) {
		AsmEmitOperand(ctx, ReadAmxMemory(prev_cip + 4));
		WriteAmxMemory(prev_cip, _:RelocateOpcode(OP_NOP));
	} else {
		assert cip - prev_cip == 4;
	}

	new faddr = s_Base + cod + s_Func; // Absolute address to `func` in ObserveArrayPtr
	new retaddr = s_Base + dat + cip + 4;
	new low = s_Addr, high = s_Addr + s_Size * 4;
	static prev, tmp;

	// Make sure it's a valid edit (also that it's within bounds)
	// This will safeguard against any false positives in the code scanner
	@emit push.pri
	@emit push.alt
	@emit move.pri
	@emit const.alt low
	@emit jless.rel 24
	@emit const.alt high
	@emit jgeq.rel 12
	@emit jump.rel 20
	@emit pop.alt
	@emit pop.pri
	@emit stor.i
	@emit jump          retaddr
	@emit pop.alt
	@emit pop.pri

	// Save the previous value i prev and modify the array
	@emit push.pri
	@emit move.pri
	@emit load.i
	@emit stor.pri ref(prev)
	@emit pop.pri
	@emit stor.pri ref(tmp)
	// This is used to support multiple observers on the same array
	@emit const.pri s_Addr
	@emit load.pri ref(tmp)
	@emit stor.i
	@emit push.pri
	@emit push.alt

	// Invoke callback(prev, curr, idx)
	// Calculate and push the array index
	@emit push.pri
	@emit const.pri s_Addr
	@emit sub.alt
	@emit shr.c.pri 2
	@emit pop.alt
	@emit push.pri
	// Push the current value
	@emit push.alt
	// Push the previous value
	@emit load.pri ref(prev)
	@emit push.pri
	@emit push.num.args 3
	@emit call faddr

	// Restore pri/alt and jump back
	@emit pop.alt
	@emit pop.pri
	@emit jump          retaddr

	// Install the hook
	WriteAmxMemory(cip - 4, _:RelocateOpcode(OP_JUMP));
	WriteAmxMemory(cip, s_Base + dat + s_Code + s_CodeOffset);

	s_CodeOffset += ctx[AsmContext_buffer_offset];
}

#define dynamic%0\32;%1<-%0(%2) \
	new %1; \
	forward dynR%1(); \
	public dynR%1() { \
		DynamicObserveMulti(addressofex(dynU%1()), DOL(%2)); \
	} \
	forward dynU%1(...); \
	public dynU%1(...) { \
		%1 = dyn@%1(); \
	} \
	forward dyn@%1(...); \
	public dyn@%1(...)

// (a, b, ..) -> (ref(a), sizeof(a), ref(b), sizeof(b), ..)
#define DOL(%1) \
	@oaE:%1,@oaL:
// Var list loop
#define @oaI(%1) \
	ref(%1),sizeof(%1)
#define @oaP:%1||| \
	@oaI(%1),@oaE:
// Var list loop entry
#define @oaE:%1, \
	_:@oa0:@oa1:@oaPl:@oaP:%1|||
// End of var list
#define @oaPl:@oaP:%1|||@oaL: \
	@oaI(%1)

stock DynamicObserveMulti(func, ...) {
	new args = numargs();
	for (new i = 1; i < args; i += 2) {
		new addr = getarg(i);
		new size = getarg(i + 1);

		if (size > 1) {
			ObserveArrayPtr(addr, func);
		} else {
			ObserveVarPtr(addr, func);
		}
	}
}

forward InstallObservers();

public OnGameModeInit() {
	InstallObservers();

	#if defined OBS_OnGameModeInit
		OBS_OnGameModeInit();
	#else
		return 1;
	#endif
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit OBS_OnGameModeInit

#if defined OBS_OnGameModeInit
	forward OBS_OnGameModeInit();
#endif

forward OnJITCompile();
public OnJITCompile() {
	InstallObservers();

	#if defined OBS_OnJITCompile
		OBS_OnJITCompile();
	#else
		return 1;
	#endif
}

#if defined _ALS_OnJITCompile
	#undef OnJITCompile
#else
	#define _ALS_OnJITCompile
#endif

#define OnJITCompile OBS_OnJITCompile

#if defined OBS_OnJITCompile
	forward OBS_OnJITCompile();
#endif

public OnFilterScriptInit() {
	InstallObservers();

	#if defined OBS_OnFilterScriptInit
		return OBS_OnFilterScriptInit();
	#else
		return 1;
	#endif
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit OBS_OnFilterScriptInit

#if defined OBS_OnFilterScriptInit
	forward OBS_OnFilterScriptInit();
#endif


public InstallObservers() {
	static installed = false;

	if (installed) {
		return;
	}

	installed = true;

	s_InObserveCallback = true;

	ObInit();

	#if defined OBS_InstallObservers
		OBS_InstallObservers();
	#endif

	s_InObserveCallback = false;
}

#if defined _ALS_InstallObservers
	#undef InstallObservers
#else
	#define _ALS_InstallObservers
#endif

#define InstallObservers OBS_InstallObservers

#if defined OBS_InstallObservers
	forward OBS_InstallObservers();
#endif
