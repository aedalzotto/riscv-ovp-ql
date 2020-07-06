#include "op/op.h"
#include <stdio.h>

#define APP_LDR_CTRL    OP_LDR_SET_START|OP_LDR_FAIL_IS_ERROR
#define HARNESS_NAME "harness" 

static OP_CONSTRUCT_FN(moduleConstructor) 
{
	/* Get the processor path */
	const char *riscvModel = opVLNVString(
		0, // use the default VLNV path
		"riscv.ovpworld.org",
		"processor",
		"riscv",
		"1.0",
		OP_PROCESSOR,
		1 // report errors
	);

	/* Get the semihost file */
	const char *riscvSemihost = opVLNVString(
		0, // use the default VLNV path
		0,
		0,
		"pk",
		0,
		OP_EXTENSION,
		1 // report errors
	);

	optBusP bus[28];
	optProcessorP cpu[28];
	
	int i;
	for(i = 0; i < 28; i++){
		char name[255];

		sprintf(name, "bus%d", i);
		/* Create the processor bus */
		bus[i] = opBusNew(mi, name, 32, 0, 0);

		sprintf(name, "cpu%d", i);
		/* Create the processor */
		cpu[i] = opProcessorNew(
			mi,
			riscvModel,
			name,
			/* Connect instruction and data to the same bus */
			OP_CONNECTIONS(
				OP_BUS_CONNECTIONS(
					OP_BUS_CONNECT(bus[i], "INSTRUCTION"),
					OP_BUS_CONNECT(bus[i], "DATA")
				)
			),
			OP_PARAMS(
				OP_PARAM_STRING_SET("variant", "RV32I")
			)
		);

		sprintf(name, "pk_%d", i);
		/* Set the CPU semihost */
		opProcessorExtensionNew(
			cpu[i],
			riscvSemihost,
			name,
			0
		);

		sprintf(name, "local%d", i);
		/* Create and connect processor local memory */
		opMemoryNew(
			mi,
			name,
			OP_PRIV_RWX,
			0x60000ULL-1,
			OP_CONNECTIONS(
				OP_BUS_CONNECTIONS(
					OP_BUS_CONNECT(bus[i], "mp0", .slave=1, .addrLo=0x0ULL, .addrHi=0x5FFFFULL)
				)
			),
			0
		);

		sprintf(name, "stack%d", i);
		/* Create and connect processor stack memory */
		opMemoryNew(
			mi,
			name,
			OP_PRIV_RWX,
			0x10000ULL-1,
			OP_CONNECTIONS(
				OP_BUS_CONNECTIONS(
					OP_BUS_CONNECT(bus[i], "mp1", .slave=1, .addrLo=0xFFFF0000ULL, .addrHi=0xFFFFFFFFULL)
				)
			),
			0
		);
	}
}

// int main(int argc, const char *argv[])
// {
// 	/* Init OP */
// 	opSessionInit(OP_VERSION);

// 	/* Default parser. --program argument sets the elf */
// 	opCmdParseStd(argv[0], OP_AC_ALL, argc, argv);

// 	/* Init root module enabling verbose mode to get statistics */
// 	optParamP paramList = 0;
// 	paramList = opParamBoolSet(paramList, OP_FP_VERBOSE, 1);
// 	paramList = opParamBoolSet(paramList, OP_FP_STOPONCONTROLC, 1);

// 	optModuleP mi = opRootModuleNew(0, 0, paramList);

// 	constructPlatform(mi);

// 	opRootModuleSimulate(mi);
// 	opSessionTerminate();

// 	return 0;
// }

optModuleAttr modelAttrs = {
 .versionString = OP_VERSION,
 .type = OP_MODULE,
 .name = HARNESS_NAME,
 .releaseStatus = OP_UNSET,
 .purpose = OP_PP_BAREMETAL,
 .visibility = OP_VISIBLE,
 .constructCB = moduleConstructor,
};