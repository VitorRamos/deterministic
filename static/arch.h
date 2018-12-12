#include <stdio.h>

#define VENDOR_UNKNOWN 0
#define VENDOR_AMD     1
#define VENDOR_INTEL   2

struct cpuinfo_t {
	int vendor;
	int family;
	int model;
	int stepping;
	char modelname[BUFSIZ];
	char generic_modelname[BUFSIZ];
} cpuinfo;

struct event_table_t {
	char hw_int_name[BUFSIZ];
	char hw_int_event[BUFSIZ];
	char ret_instr_name[BUFSIZ];
	char ret_instr_event[BUFSIZ];
	char branches_name[BUFSIZ];
	char branches_event[BUFSIZ];
	char cond_branches_name[BUFSIZ];
	char cond_branches_event[BUFSIZ];
	char loads_event[BUFSIZ];
	char loads_name[BUFSIZ];
	char stores_event[BUFSIZ];
	char stores_name[BUFSIZ];
	char uops_event[BUFSIZ];
	char uops_name[BUFSIZ];
	char muls_event[BUFSIZ];
	char muls_name[BUFSIZ];
	char divs_event[BUFSIZ];
	char divs_name[BUFSIZ];
	char fp1_event[BUFSIZ];
	char fp1_name[BUFSIZ];
	char fp2_event[BUFSIZ];
	char fp2_name[BUFSIZ];
	char sse_event[BUFSIZ];
	char sse_name[BUFSIZ];
} *event_table = NULL;


struct event_table_t atom_event_table = {
   .hw_int_name =		"HW_INT_RCV", /* Warning, overcounts x2 */
   .hw_int_event =		"r5100c8:u",
   .ret_instr_name =		"INSTRUCTION_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BRANCH_INSTRUCTIONS_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"NONE",
   .cond_branches_event =	"NONE",
   .loads_name =		"MEM_LOAD_RETIRED:L2_MISS:L2_HIT",
   .loads_event =		"r5303cb:u",
   .stores_name =		"NONE",
   .stores_event =		"NONE",
   .uops_name =			"UOPS_RETIRED",
   .uops_event =		"r5010c2:u",
   .muls_name =			"MUL:AR",
   .muls_event = 		"r538112:u",
   .divs_name = 		"DIV:AR",
   .divs_event =		"r538113:u",
   .fp1_name =			"X87_COMP_OPS_EXE:ANY_AR",
   .fp1_event =			"r508110:u",
   .fp2_name =			"X87_COMP_OPS_EXE:ANY_S",
   .fp2_event =			"r530110:u",
   .sse_name =			"SIMD_INST_RETIRED",
   .sse_event =			"r501fc7:u",
};

struct event_table_t pentium4_event_table = {
   .hw_int_name =		"NONE",
   .hw_int_event =		"NONE",
   .ret_instr_name =		"INSTR_RETIRED:NBOGUSNTAG",
   .ret_instr_event =		"",
   .branches_name =		"BRANCH_RETIRED:MMNP:MMNM:MMTP:MMTM",
   .branches_event =		"branches:u",
   .cond_branches_name =	"NONE", /* is this true? */
   .cond_branches_event =	"NONE",
   .loads_name =		"FRONT_END_EVENT:NBOGUS,UOPS_TYPE:TAGLOADS",
   .loads_event =		"",
   .stores_name =		"FRONT_END_EVENT:NBOGUS,UOPS_TYPE:TAGSTORES",
   .stores_event =		"",
   .uops_name =			"UOPS_RETIRED:NBOGUS",
   .uops_event =		"",
   .muls_name =			"NONE",
   .muls_event = 		"NONE",
   .divs_name = 		"NONE",
   .divs_event =		"NONE",
   .fp1_name =			"EXECUTION_EVENT:NBOGUS1,X87_FP_UOP:ALL:TAG1",
   .fp1_event =			"",
   .fp2_name =			"NONE",
   .fp2_event =			"NONE",
   .sse_name =			"EXECUTION_EVENT:NBOGUS2,PACKED_SP_UOP:ALL:TAG2,PACKED_DP_UOP:ALL:TAG2",
   .sse_event =			"",
};

struct event_table_t core2_event_table = {
   .hw_int_name =		"HW_INT_RCV",
   .hw_int_event =		"r5100c8:u",
   .ret_instr_name =		"INSTRUCTION_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BRANCH_INSTRUCTIONS_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"BR_CND_EXEC",
   .cond_branches_event =	"r53008b:u",
   .loads_name =		"INST_RETIRED:LOADS",
   .loads_event =		"r5001c0:u",
   .stores_name =		"INST_RETIRED:STORES",
   .stores_event =		"r5002c0:u",
   .uops_name =			"UOPS_RETIRED",
   .uops_event =		"r500fc2:u",
   .muls_name =			"MUL",
   .muls_event = 		"r510012:u",
   .divs_name = 		"DIV",
   .divs_event =		"r510013:u",
   .fp1_name =			"FP_COMP_OPS_EXE",
   .fp1_event =			"r500010:u",
   .fp2_name =			"X87_OPS_RETIRED:ANY",
   .fp2_event =			"r50fec1:u",
   .sse_name =			"SIMD_INSTR_RETIRED",
   .sse_event =			"r5000ce:u",
};

struct event_table_t nhm_event_table = {
   .hw_int_name = 		"HW_INTERRUPTS (dropped from documentation?)",
   .hw_int_event =		"r50011d:u",
   .ret_instr_name =		"INSTRUCTION_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BRANCH_INSTRUCTIONS_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
   .cond_branches_event =	"r5301c4:u",
   .loads_name =		"MEM_INST_RETIRED:LOADS",
   .loads_event =		"r53010b:u",
   .stores_name =		"MEM_INST_RETIRED:STORES",
   .stores_event =		"r53020b:u",
   .uops_name =			"UOPS_RETIRED:ANY",
   .uops_event =		"r5301c2:u",
   .muls_name =			"ARITH:MUL",
   .muls_event =		"r530214:u",
   .divs_name =			"ARITH:DIV",
   .divs_event =		"r1d70114:u",
   .fp1_name =			"FP_COMP_OPS_EXE:X87",
   .fp1_event =			"r530110:u",
   .fp2_name =			"INST_RETIRED:X87",
   .fp2_event =			"r5302c0:u",
    /* SSEX_UOPS_RETIRED? */
   .sse_name =			"FP_COMP_OPS_EXE:SSE_FP",
   .sse_event =			"r530410:u",
};

struct event_table_t wsm_event_table = {
   .hw_int_name = 		"HW_INTERRUPTS (Dropped from Documentation",
   .hw_int_event =		"r50011d:u",
   .ret_instr_name =		"INSTRUCTION_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BRANCH_INSTRUCTIONS_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
   .cond_branches_event =	"r5301c4:u",
   .loads_name =		"MEM_INST_RETIRED:LOADS",
   .loads_event =		"r53010b:u",
   .stores_name =		"MEM_INST_RETIRED:STORES",
   .stores_event =		"r53020b:u",
   .uops_name =			"UOPS_RETIRED:ANY",
   .uops_event =		"r5301c2:u",
   .muls_name =			"ARITH:MUL",
   .muls_event =		"r530214:u",
   .divs_name =			"ARITH:DIV",
   .divs_event =		"r1d70114:u",
   .fp1_name =			"FP_COMP_OPS_EXE:X87",
   .fp1_event =			"r530110:u",
   .fp2_name =			"INST_RETIRED:X87",
   .fp2_event =			"r5302c0:u",
   .sse_name =			"FP_COMP_OPS_EXE:SSE_FP",
   .sse_event =			"r530410:u",
};

struct event_table_t snb_event_table = {
   .hw_int_name = 		"HW_INTERRUPTS",
   .hw_int_event =		"r53cb01:u",
   .ret_instr_name =		"INSTRUCTIONS_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BRANCH_INSTRUCTIONS_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
   .cond_branches_event =	"r5301c4:u",
   .loads_name =		"MEM_UOP_RETIRED:ANY_LOADS",
   .loads_event =		"r5381d0:u",
   .stores_name =		"MEM_UOP_RETIRED:ANY_STORES",
   .stores_event =		"r5382d0:u",
   .uops_name =			"UOPS_RETIRED:ANY",
   .uops_event =		"r5301c2:u",
   .muls_name =			"NONE",
   .muls_event =		"NONE",
   .divs_name =			"ARITH:FPU_DIV",
   .divs_event =		"r1570114:u",
   .fp1_name =			"FP_COMP_OPS_EXE:X87",
   .fp1_event =			"r530110:u",
   .fp2_name =			"INST_RETIRED:X87",
   .fp2_event =			"r5302c0:u",
   .sse_name =			"FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION",
   .sse_event =			"r538010:u",
};

struct event_table_t ivb_event_table = {
   .hw_int_name = 		"HW_INTERRUPTS",
   .hw_int_event =		"r5301cb:u",
   .ret_instr_name =		"INSTRUCTION_RETIRED",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"BR_INST_RETIRED",
   .branches_event =		"branches:u",
   .cond_branches_name =	"BR_INST_RETIRED:COND",
   .cond_branches_event =	"r5301c4:u",
   .loads_name =		"MEM_UOPS_RETIRED:ALL_LOADS",
   .loads_event =		"r5381d0:u",
   .stores_name =		"MEM_UOPS_RETIRED:ALL_STORES",
   .stores_event =		"r5382d0:u",
   .uops_name =			"UOPS_RETIRED:ALL",
   .uops_event =		"r5301c2:u",
   .muls_name =			"UOPS_ISSUED:SINGLE_MUL",
   .muls_event =		"r53400e:u",
   .divs_name =			"ARITH:FPU_DIV",
   .divs_event =		"r1570114:u",
   .fp1_name =			"FP_COMP_OPS_EXE:X87 (SNB)",
   .fp1_event =			"r530110:u",
   .fp2_name =			"INST_RETIRED:X87 (SNB)",
   .fp2_event =			"r5302c0:u",
   .sse_name =			"FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION (SNB)",
   .sse_event =			"r538010:u",
};

struct event_table_t hsw_event_table = {
	.hw_int_name = 		"HW_INTERRUPTS",
	.hw_int_event =		"r5301cb:u",
	.ret_instr_name =	"INSTRUCTION_RETIRED",
	.ret_instr_event =	"instructions:u",
	.branches_name =	"BR_INST_RETIRED",
	.branches_event =	"branches:u",
	.cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
	.cond_branches_event =	"r5301c4:u",
	/* MEM_LOAD_UOPS_RETIRED ?*/
	.loads_name =		"MEM_UOPS_RETIRED:ALL_LOADS",
	.loads_event =		"r5381d0:u",
	.stores_name =		"MEM_UOPS_RETIRED:ALL_STORES",
	.stores_event =		"r5382d0:u",
	.uops_name =		"UOPS_RETIRED:ALL",
	.uops_event =		"r5301c2:u",
	.muls_name =		"UOPS_ISSUED:SINGLE_MUL",
	.muls_event =		"r53400e:u",
	.divs_name =		"NONE",
	.divs_event =		"NONE",
	.fp1_name =		"FP_COMP_OPS_EXE:X87 (SNB)",
	.fp1_event =		"r530110:u",
	.fp2_name =		"INST_RETIRED:X87 (SNB)",
	.fp2_event =		"r5302c0:u",
	.sse_name =		"FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION (SNB)",
	.sse_event =		"r538010:u",
};

struct event_table_t bdw_event_table = {
	.hw_int_name = 		"HW_INTERRUPTS",
	.hw_int_event =		"r5301cb:u",
	.ret_instr_name =	"INSTRUCTION_RETIRED",
	.ret_instr_event =	"instructions:u",
	.branches_name =	"BR_INST_RETIRED",
	.branches_event =	"branches:u",
	.cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
	.cond_branches_event =	"r5301c4:u",
	/* MEM_LOAD_UOPS_RETIRED ?*/
	.loads_name =		"MEM_UOPS_RETIRED:ALL_LOADS",
	.loads_event =		"r5381d0:u",
	.stores_name =		"MEM_UOPS_RETIRED:ALL_STORES",
	.stores_event =		"r5382d0:u",
	.uops_name =		"UOPS_RETIRED:ALL",
	.uops_event =		"r5301c2:u",
	.muls_name =		"UOPS_ISSUED:SINGLE_MUL",
	.muls_event =		"r53400e:u",
	.divs_name =		"NONE",
	.divs_event =		"NONE",
	.fp1_name =		"FP_COMP_OPS_EXE:X87 (SNB)",
	.fp1_event =		"r530110:u",
	.fp2_name =		"INST_RETIRED:X87 (SNB)",
	.fp2_event =		"r5302c0:u",
	.sse_name =		"FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION (SNB)",
	.sse_event =		"r538010:u",
};

struct event_table_t skl_event_table = {
	.hw_int_name = 		"HW_INTERRUPTS",
	.hw_int_event =		"r5301cb:u",
	.ret_instr_name =	"INSTRUCTION_RETIRED",
	.ret_instr_event =	"instructions:u",
	.branches_name =	"BR_INST_RETIRED",
	.branches_event =	"branches:u",
	.cond_branches_name =	"BR_INST_RETIRED:CONDITIONAL",
	.cond_branches_event =	"r5301c4:u",
	/* MEM_LOAD_UOPS_RETIRED ?*/
	.loads_name =		"MEM_UOPS_RETIRED:ALL_LOADS",
	.loads_event =		"r5381d0:u",
	.stores_name =		"MEM_UOPS_RETIRED:ALL_STORES",
	.stores_event =		"r5382d0:u",
	.uops_name =		"UOPS_RETIRED:ALL",
	.uops_event =		"r5301c2:u",
	.muls_name =		"UOPS_ISSUED:SINGLE_MUL",
	.muls_event =		"r53400e:u",
	.divs_name =		"NONE",
	.divs_event =		"NONE",
	.fp1_name =		"FP_COMP_OPS_EXE:X87 (SNB)",
	.fp1_event =		"r530110:u",
	.fp2_name =		"INST_RETIRED:X87 (SNB)",
	.fp2_event =		"r5302c0:u",
	.sse_name =		"FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION (SNB)",
	.sse_event =		"r538010:u",
};


struct event_table_t fam15h_event_table = {
	/* SMIS_RECEIVED? */
	.hw_int_name = 		"INTERRUPTS_TAKEN",
	.hw_int_event =		"r5000cf:u",
	.ret_instr_name =	"RETIRED_INSTRUCTIONS",
	.ret_instr_event =	"instructions:u",
	.branches_name =	"RETIRED_BRANCH_INSTRUCTIONS",
	.branches_event =	"branches:u",
	.cond_branches_name =	"RETIRED_TAKEN_BRANCH_INSTRUCTIONS",
	.cond_branches_event =	"r5300c4:u",
	.loads_name =		"LS_DISPATCH:LOADS",
	.loads_event =		"r530129:u",
	.stores_name =		"LS_DISPATCH:STORES",
	.stores_event =		"r530229:u",
	.uops_name =		"RETIRED_UOPS",
	.uops_event =		"r5000c1:u",
	.muls_name =		"RETIRED_SSE_OPS:SINGLE_MUL_OPS:DOUBLE_MUL_OPS",
	.muls_event =		"r532203:u",
	.divs_name =		"RETIRED_SSE_OPS:SINGLE_DIV_OPS:DOUBLE_DIV_OPS",
	.divs_event =		"r534403:u",
	.fp1_name =		"DISPATCHED_FPU_OPS:ALL",
	.fp1_event =		"r53ff00:u",
	.fp2_name =		"RETIRED_MMX_FP_INSTRUCTIONS:ALL",
	.fp2_event =		"r5307cb:u",
	.sse_name =		"RETIRED_SSE_OPS:ALL",
	.sse_event =		"r53ff03:u",
};


struct event_table_t fam14h_event_table = {
   .hw_int_name = 		"INTERRUPTS_TAKEN",
   .hw_int_event =		"r5000cf:u",
   .ret_instr_name =		"RETIRED_INSTRUCTIONS",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"RETIRED_BRANCH_INSTRUCTIONS",
   .branches_event =		"branches:u",
   .cond_branches_name =	"NONE",
   .cond_branches_event =	"NONE",
   .loads_name =		"NONE",
   .loads_event =		"NONE",
   .stores_name =		"NONE",
   .stores_event =		"NONE",
   .uops_name =			"RETIRED_UOPS",
   .uops_event =		"r5000c1:u",
   /* RETIRED_X87_FPU_OPS:MULT_OPS?*/
   .muls_name =			"RETIRED_SSE_OPERATIONS:SINGLE_MUL_OPS:DOUBLE_MUL_OPS",
   .muls_event =		"r531203:u",
   .divs_name =			"RETIRED_SSE_OPERATIONS:SINGLE_DIV_OPS:DOUBLE_DIV_OPS",
   .divs_event =		"r532403:u",
   .fp1_name =			"RETIRED_FLOATING_POINT_INSTRUCTIONS",
   .fp1_event =			"r5303cb:u",
   .fp2_name =			"DISPATCHED_FPU:ANY",
   .fp2_event =			"r530300:u",
   .sse_name =			"RETIRED_SSE_OPERATIONS:ALL",
   .sse_event =			"r507f03:u",
};

struct event_table_t fam10h_event_table = {
   .hw_int_name = 		"INTERRUPTS_TAKEN",
   .hw_int_event =		"r5000cf:u",
   .ret_instr_name =		"RETIRED_INSTRUCTIONS",
   .ret_instr_event =		"instructions:u",
   .branches_name =		"RETIRED_BRANCH_INSTRUCTIONS",
   .branches_event =		"r5000c2:u", /* linux 2.6.34 and earlier gets this wrong with branches:u */
   .cond_branches_name =	"NONE",
   .cond_branches_event =	"NONE",
   .loads_name =		"NONE",
   .loads_event =		"NONE",
   .stores_name =		"NONE",
   .stores_event =		"NONE",
   .uops_name =			"RETIRED_UOPS",
   .uops_event =		"r5000c1:u",
   .muls_name =			"DISPATCHED_FPU:OPS_MULTIPLY",
   .muls_event =		"r500200:u",
   .divs_name =			"NONE",
   .divs_event =		"NONE",
   .fp1_name =			"RETIRED_MMX_AND_FP_INSTRUCTIONS:X87",
   .fp1_event =			"r5001cb:u",
   .fp2_name =			"RETIRED_MMX_AND_FP_INSTRUCTIONS:ALL",
   .fp2_event =			"r5007cb:u",
   .sse_name =			"RETIRED_SSE_OPERATIONS:ALL",
   .sse_event =			"r507f03:u",
};