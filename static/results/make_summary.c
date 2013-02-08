#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNS 10

#define NUM_STATS 13
#define VALUE_INTS 1
#define INTS_VALUE 2
#define VALUE_VALUE 3

#define NUM_BENCH 4
#define BENCH_ALL 0
#define BENCH_FP  1
#define BENCH_INT 2
#define BENCH_SSE 3

long long calc_interrupts_file();

int warn_intr_already=0;

struct stat_type{
   char name[256];
   char filename[256];
   int type;
   long long expected[NUM_BENCH];
   long long value1[RUNS];
   long long hw_interrupts[RUNS];
   long long raw_average;
   long long adj_average;
   long long raw_diff;
   long long adj_diff;
} stats[NUM_STATS]={
   {"Retired Instructions","inst_retired", VALUE_INTS,{226990030LL,44800000LL,0LL,0LL}  },
   {"Retired Branches","branches_retired",VALUE_INTS,{   9240001LL,0LL,0LL,0LL} },   
   {"Conditional Branches","cond_branches",VALUE_INTS,{   8220000LL,0LL,0LL,0LL} },   
   {"Retired Uops","uops_retired",         VALUE_INTS,{         0LL,0LL,0LL,0LL} },      
   {"Retired Loads","loads_retired",       VALUE_INTS,{  79590000LL,0LL,0LL,0LL} },      
   {"Retired Stores","stores_retired",     VALUE_INTS,{  24060000LL,0LL,0LL,0LL} },      
   {"Multiplies","muls_retired",           INTS_VALUE,{         0LL,0LL,0LL,0LL} },      
   {"Divides","divs_retired",              INTS_VALUE,{         0LL,0LL,0LL,0LL} },         
   {"FP","fp_retired",                     VALUE_VALUE,{        0LL,0LL,0LL,0LL} },         
   {"SSE","sse_retired",                   VALUE_INTS,{         0LL,0LL,0LL,0LL} },            
   {"SW Prefetch NTA","swprfnta_retired",                   VALUE_INTS,{         200000LL,0LL,0LL,0LL} },            
   {"SW Prefetch L1","swprfl1_retired",                   VALUE_INTS,{         200000LL,0LL,0LL,0LL} },            
   {"SW Prefetch L2","swprfl2_retired",                   VALUE_INTS,{         400000LL,0LL,0LL,0LL} },            

};

#define RETIRED_INSTRUCTIONS 0
#define RETIRED_BRANCHES     1
#define COND_BRANCHES        2 
#define RETIRED_UOPS         3
#define RETIRED_LOADS        4
#define RETIRED_STORES       5
#define RETIRED_SWPREF_NTA  10
#define RETIRED_SWPREF_L1   11
#define RETIRED_SWPREF_L2   12

#define UNKNOWN     0
#define PENTIUMD    1
#define PENTIUM4    2
#define PHENOM      3
#define ISTANBUL    4
#define CORE2       5
#define NEHALEM     6
#define NEHALEMEX   7
#define SANDYBRIDGE 8
#define BOBCAT      9
#define WESTMERE   10
#define IVYBRIDGE  11
#define ATOM       12
#define MAX_NAME   13

char machine_names[MAX_NAME][BUFSIZ]={
  "UNKNOWN",
  "Pentium D",
  "Pentium 4",
  "Phenom",
  "Istanbul",
  "Core2",
  "Nehalem",
  "Nehalem-EX",
  "SandyBridge",
  "Bobcat",
  "Westmere",
  "IvyBridge",
  "Atom",
};

long long remove_commas(char *temp_string) {

   int y=0,z=0;

   while(y<strlen(temp_string)) {

      if (temp_string[y]==',') {
      }
      else {
         temp_string[z]=temp_string[y];
         z++;
      }
      y++;
   }
   temp_string[z]=0;

   return(atoll(temp_string));
}


long long total=0,max,min,range;
long long hwint_adjust=0;
long long double_ins_adjust=0;
long long undercount_adjust=0;
long long mem_adjust=0;
long long fpu_adjust=0;
long long exception_adjust=0;

void adjust_for_pagefaults(void) {
   printf("\tAdjusting 10,008 for first-time memory accesses\n");
   mem_adjust+=10008;
}

void adjust_for_lazy_fp(void) {
   printf("\tAdjusting 1 for first-time FPU use\n");
   fpu_adjust+=1;
}

void adjust_for_fp_exception(void) {
   printf("\tAffected by FP Exceptions\n");
   exception_adjust=1;
}

void dont_adjust_for_hw_interrupts(int j) {
	         /* don't adjust */
                 stats[j].adj_average=stats[j].raw_average;
	    	 hwint_adjust=0;
}

void adjust_for_hw_interrupts(int j) {

   int i;

   total=0;
   max=stats[j].value1[0]-stats[j].hw_interrupts[0];
   min=stats[j].value1[0]-stats[j].hw_interrupts[0];

   for(i=0;i<RUNS;i++) {
      total+=stats[j].value1[i]-stats[j].hw_interrupts[i];
      if (stats[j].value1[i]-stats[j].hw_interrupts[i]>max) {
         max=stats[j].value1[i]-stats[j].hw_interrupts[i];
      }
      if (stats[j].value1[i]-stats[j].hw_interrupts[i]<min) {
         min=stats[j].value1[i]-stats[j].hw_interrupts[i];
      }
      stats[j].adj_average=total/RUNS;
   }

   /* calculate range */
   if ((max-stats[j].adj_average) > (stats[j].adj_average-min)) {
      range=max-stats[j].adj_average;
   }
   else {
      range=min-stats[j].adj_average;
   }

   for(i=0;i<RUNS;i++) {
      hwint_adjust+=stats[j].hw_interrupts[i];
   }
   hwint_adjust/=RUNS;
   printf("\tAdjusting %lld for hwints\n",hwint_adjust);
}


int main(int argc, char **argv) {

   int i,j,k,lines=0,bench_type=BENCH_ALL;
   char path[BUFSIZ],string[BUFSIZ],temp_string[BUFSIZ];
   FILE *fff;

   char *ignore;
   int machine_type=UNKNOWN;
   (void) ignore;

   if (argc<3) {
      printf("Usage: %s machine bench\n\n",argv[0]);
      exit(-1);
   }

   /* Detect which benchmark we are using */
   if (!strncmp(argv[2],"fp",2)) bench_type=BENCH_FP;
   if (!strncmp(argv[2],"int",3)) bench_type=BENCH_INT;
   if (!strncmp(argv[2],"sse",3)) bench_type=BENCH_SSE;
   if (!strncmp(argv[2],"all",3)) bench_type=BENCH_ALL;

   /* detect machine */
   if (!strncmp(argv[1],"domori",6)) machine_type=PENTIUMD;
   if (!strncmp(argv[1],"domori2",7)) machine_type=PENTIUM4;
   if (!strncmp(argv[1],"venchi",6)) machine_type=PHENOM;
   if (!strncmp(argv[1],"ig",2)) machine_type=ISTANBUL;
   if (!strncmp(argv[1],"deater",6)) machine_type=CORE2;
   if (!strncmp(argv[1],"cl320",5)) machine_type=CORE2;
   if (!strncmp(argv[1],"gonzo",5)) machine_type=NEHALEM;
   if (!strncmp(argv[1],"saturn",6)) machine_type=NEHALEMEX;
   if (!strncmp(argv[1],"server",6)) machine_type=ATOM;
   if (!strncmp(argv[1],"toad4",5)) machine_type=SANDYBRIDGE;
   if (!strncmp(argv[1],"pianoman",8)) machine_type=BOBCAT;
   if (!strncmp(argv[1],"macbook",7)) machine_type=NEHALEM;
   if (!strncmp(argv[1],"mirasol",7)) machine_type=WESTMERE;
   if (!strncmp(argv[1],"vincent-weaver-1",17)) machine_type=IVYBRIDGE;

   printf("Machine type %s\n",machine_names[machine_type]);

   /***************************/
   /* Read in all the results */
   /***************************/

   for(j=0;j<NUM_STATS;j++) {

      for(i=0;i<RUNS;i++) {

         sprintf(path,"./%s/5/run.%i.%s.%s.counts",argv[1],i,stats[j].filename,argv[2]);
         fff=fopen(path,"r");
         if (fff==NULL) {
	    printf("Error opening %s\n",path);
	    continue;
         }

	 /* try perf_events first */
	 lines=0;
	 stats[j].value1[i]=0;
         stats[j].hw_interrupts[i]=-1;

         while(1) {
 	   if (fgets(string,BUFSIZ,fff)==NULL) break;

	   if (!strncmp(string," Performance counter",20)) {
	      if (fgets(string,BUFSIZ,fff)==NULL) break;
	      if (fgets(string,BUFSIZ,fff)==NULL) break;

	      /* Read in the first value */
	      sscanf(string,"%s",temp_string);

	      /* If comma delimited, then we have to remove the commas */
	      if (strstr(temp_string,",")) {
	         stats[j].value1[i]=remove_commas(temp_string);
	      }
	      else {
	         stats[j].value1[i]=atoll(temp_string);
	      }

	      if (fgets(string,BUFSIZ,fff)==NULL) break;

	      /* Read in second value */
	      sscanf(string,"%s",temp_string);
	      if (strstr(temp_string,",")) {
	         stats[j].hw_interrupts[i]=remove_commas(temp_string);
	      }
	      else {
	         stats[j].hw_interrupts[i]=atoll(temp_string);
	      }
	      break;
	   }
	   lines++;
	 }

	 /* try perfmon2 if that fails */
	 if (stats[j].value1[i]==0) {
	    rewind(fff);
	    for(k=0;k<lines-2;k++) {
               ignore=fgets(string,BUFSIZ,fff);
	    }
	    ignore=fgets(string,BUFSIZ,fff);
	    if (sscanf(string,"%lld",&stats[j].value1[i])==0) {
	       /* handle if only one stat in file */
	       ignore=fgets(string,BUFSIZ,fff);
	       sscanf(string,"%lld",&stats[j].value1[i]);
               stats[j].hw_interrupts[i]=-1;
	    }
            else {
               ignore=fgets(string,BUFSIZ,fff);
               if (sscanf(string,"%lld",&stats[j].hw_interrupts[i])!=1) 
	          stats[j].hw_interrupts[i]=-1;
	    }
	 }

	 /* check to see if getting interrupt count failed */
	 if (stats[j].hw_interrupts[i]<1) {
            sprintf(path,"./%s/5/run.%i.%s.%s",argv[1],i,stats[j].filename,argv[2]);
            stats[j].hw_interrupts[i]=calc_interrupts_file(path);
            if (!warn_intr_already) {
	       printf("Warning!: Using /proc/interrupts for int count %lld!\n",stats[j].hw_interrupts[i]);
               warn_intr_already=1;
	    }
	    //exit(-5);
	 }
      }


   /* Adjust and print values */

      printf("\n%s     expected value = %lld\n",
	     stats[j].name, stats[j].expected[0]);

      if (stats[j].type==VALUE_INTS) {
         total=0;
         max=stats[j].value1[0];
         min=stats[j].value1[0];

         for(i=0;i<RUNS;i++) {
	    total+=stats[j].value1[i];
	    if (stats[j].value1[i]>max) max=stats[j].value1[i];
	    if (stats[j].value1[i]<min) min=stats[j].value1[i];
         }
         stats[j].raw_average=total/RUNS;

         if ((max-stats[j].raw_average) > (stats[j].raw_average-min)) range=max-stats[j].raw_average;
         else range=min-stats[j].raw_average;

         printf("\tRaw Average: %lld +/- %lld (",stats[j].raw_average,range);
         for(i=0;i<3;i++) printf("%lld, ",stats[j].value1[i]);
         printf(")\n");
         printf("\tRaw diff: %lld +/- %lld\n",
		stats[j].raw_average-stats[j].expected[bench_type],range);

         hwint_adjust=0;
	 double_ins_adjust=0;
	 undercount_adjust=0;
	 mem_adjust=0;
	 fpu_adjust=0;
	 exception_adjust=0;

	 /* ADJUSTMENTS */
         printf("Adjustments:\n");
	 switch(machine_type) {

	 case PENTIUMD:
	      if (j==RETIRED_INSTRUCTIONS) {
	         adjust_for_hw_interrupts(j);
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
		 adjust_for_lazy_fp();
                 printf("\tAdjusting 200,000 for emms\n");
                 double_ins_adjust+=200000;
                 printf("\tAdjusting 100,000 for maskmovq\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 400,000 for cvtpd2pi\n");
                 double_ins_adjust+=400000;
                 printf("\tAdjusting 100,000 for mfence\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for sfence\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 1,500,000 for fldcw\n");
                 double_ins_adjust+=1500000;
                 printf("\tAdjusting 100,000 for fldenv\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for frstor\n");
                 double_ins_adjust+=100000;
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) {
		 adjust_for_hw_interrupts(j);

                 printf("\tAdjusting 50,040 for first-time memory accesses\n");
                 mem_adjust+=50040;

                 printf("\tSTACK: Adjusting 200,000 for pop fs/gs\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE1: Adjusting 200,000 for movups (load)\n");
                 double_ins_adjust+=200000;
	         printf("\tSSE1: Adjusting 400,000 for movdqu\n");
                 double_ins_adjust+=400000;
                 printf("\tSSE1: Adjusting -800,000 for SW prefetches\n");
                 double_ins_adjust-=800000;
                 printf("\tSSE1: Adjusting 200,000 for lddqu\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 200,000 for movupd\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 700,000 for movdqu again\n");
                 double_ins_adjust+=700000;
                 printf("\tSSE2: Adjusting 200,000 for lddqu\n");
                 double_ins_adjust+=200000;
                 printf("\tX87: Adjusting 200,000 for fstps\n");
                 double_ins_adjust+=200000;
	         printf("\tX87: Adjusting 100,000 for fldt\n");
                 double_ins_adjust+=100000;
	         printf("\tX87: Adjusting 600,000 for fldenv\n");
                 double_ins_adjust+=600000;
	         printf("\tX87: Adjusting 2,200,000 for frstor\n");
                 double_ins_adjust+=2200000;
	         printf("\tX87: Adjusting 2,500,000 for fxrstor\n");
                 double_ins_adjust+=2500000;

	         printf("\tSTRING: Adjusting 511,930,000 for rep lods\n");
                 double_ins_adjust+=511930000;
	         printf("\tSTRING: Adjusting 40,920,000 for rep movs\n");
                 double_ins_adjust+=40920000;
	         printf("\tSTRING: Adjusting 1,228,720,000 for rep cmps\n");
                 double_ins_adjust+=1228720000;
	         printf("\tSTRING: Adjusting 614,320,000 for rep scas\n");
                 double_ins_adjust+=614320000;
	      }
	      if (j==RETIRED_STORES) {
		 adjust_for_hw_interrupts(j);
                 printf("\tSTACK: Adjusting 100,000 for nested enter\n");
                 double_ins_adjust+=100000;
                 printf("\tCPUID: Adjusting 100,000 for cpuid\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 100,000 for fbstp\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 100,000 for fstps\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 200,000 for fstpt\n");
                 double_ins_adjust+=200000;
                 printf("\tX87: Adjusting 1,200,000 for fstenv\n");
                 double_ins_adjust+=1200000;
                 printf("\tX87: Adjusting 4,400,000 for fsave\n");
                 double_ins_adjust+=4400000;
                 printf("\tX87: Adjusting 2,500,000 for fxsave\n");
                 double_ins_adjust+=2500000;
                 printf("\tSSE1: Adjusting 200,000 for movups (store)\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE1: Adjusting 100,000 for sfence\n");
                 double_ins_adjust+=100000;
                 printf("\tSSE2: Adjusting 200,000 for movupd (store)\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 300,000 for movdqu (store)\n");
                 double_ins_adjust+=300000;
                 printf("\tSSE2: Adjusting 100,000 for clflush\n");
                 double_ins_adjust+=100000;
                 printf("\tSSE2: Adjusting 200,000 for maskmovdqu\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 100,000 for mfence\n");
                 double_ins_adjust+=100000;
                 printf("\tSTRING: Adjusting 71,610,000 for rep stos (forward)\n");
                 double_ins_adjust+=71610000;
                 printf("\tSTRING: Adjusting 40,950,000 for rep stos (backward)\n");
                 double_ins_adjust+=40950000;
                 printf("\tSTRING: Adjusting 40,920,000 for rep movs\n");
                 double_ins_adjust+=40920000;
	      }
	      break;
	 case PENTIUM4:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_BRANCHES) {
		 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_STORES) adjust_for_hw_interrupts(j);
	      break;
	 case PHENOM:
	 case ISTANBUL:
	 case BOBCAT:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
                 printf("\tAdjusting 100,000 for fclex with PE set\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for finit with PE set\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for fnsave with PE set\n");
                 double_ins_adjust+=100000;
	      }

	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_STORES) adjust_for_hw_interrupts(j);
	      break;
	 case ATOM:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_STORES) adjust_for_hw_interrupts(j);
	      break;

	 case CORE2:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_BRANCHES) {
		 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
                 printf("\tAdjusting 100,000 for cpuid instruction\n");
                 double_ins_adjust+=100000;
	      }
	      if (j==COND_BRANCHES) {
                 dont_adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
                 printf("\tAdjusting 200,000 for leave instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 200,000 for fstenv instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 200,000 for fsave instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 100,000 for fxsave instruction\n");
                 double_ins_adjust+=100000;
	         printf("\tAdjusting 100,000 for maskmovq instruction\n");
                 double_ins_adjust+=100000;
	         printf("\tAdjusting 200,000 for movups instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 200,000 for movupd instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 300,000 for movdqu instruction\n");
                 double_ins_adjust+=300000;
	         printf("\tAdjusting 200,000 for maskmovdqu instruction\n");
                 double_ins_adjust+=200000;
	      }
	      if (j==RETIRED_STORES) dont_adjust_for_hw_interrupts(j);
              if (j==RETIRED_SWPREF_NTA) dont_adjust_for_hw_interrupts(j);
              if (j==RETIRED_SWPREF_L1) {
                 dont_adjust_for_hw_interrupts(j);
	         printf("\tAdjusting 200,000 for fxsave instruction\n");
                 double_ins_adjust+=200000;
	         printf("\tAdjusting 500,000 for fxrstor instruction\n");
                 double_ins_adjust+=500000;
	      }
              if (j==RETIRED_SWPREF_L2) dont_adjust_for_hw_interrupts(j);
	      break;
	 case NEHALEM:
	 case NEHALEMEX:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) {
                 dont_adjust_for_hw_interrupts(j);
                 printf("\tAdjusting 8,300,000 for FP ops miscounted as cond branches\n");
                 double_ins_adjust+=8300000;
	      }
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) {
		 adjust_for_pagefaults();
                 adjust_for_hw_interrupts(j);
                 printf("\tAdjusting -100,000 for paddb instruction\n");
                 undercount_adjust+=100000;
                 printf("\tAdjusting -100,000 for paddd instruction\n");
                 undercount_adjust+=100000;
                 printf("\tAdjusting -100,000 for paddw instruction\n");
                 undercount_adjust+=100000;
	      }
	      if (j==RETIRED_STORES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
                 printf("\tAdjusting 100,000 for cpuid instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for sfence instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for clflush instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for mfence instruction\n");
                 double_ins_adjust+=100000;
	      }
	      break;
	 case WESTMERE:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) dont_adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) {
		 adjust_for_pagefaults();
                 adjust_for_hw_interrupts(j);
	      }
	      if (j==RETIRED_STORES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
                 printf("\tAdjusting 100,000 for cpuid instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for sfence instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for clflush instruction\n");
                 double_ins_adjust+=100000;
                 printf("\tAdjusting 100,000 for mfence instruction\n");
                 double_ins_adjust+=100000;
	      }
	      break;
	 case SANDYBRIDGE:
	      if (j==RETIRED_INSTRUCTIONS) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_lazy_fp();
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
                 printf("\tOS: Adjusting 10,000 for page faults\n");
                 mem_adjust+=10000;
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) dont_adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
              if (j==RETIRED_LOADS) {
                 adjust_for_hw_interrupts(j);
                 printf("\tSTACK: Adjusting 200,000 for pop fs/gs\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE1: Adjusting 200,000 for movups (load)\n");
                 double_ins_adjust+=200000;
	         printf("\tSSE1: Adjusting 400,000 for movdqu\n");
                 double_ins_adjust+=400000;
                 printf("\tSSE1: Adjusting -800,000 for SW prefetches\n");
                 double_ins_adjust-=800000;
                 printf("\tSSE1: Adjusting 200,000 for lddqu\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 200,000 for movupd\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 700,000 for movdqu again\n");
                 double_ins_adjust+=700000;
                 printf("\tSSE2: Adjusting 200,000 for lddqu\n");
                 double_ins_adjust+=200000;
                 printf("\tX87: Adjusting 200,000 for fstps\n");
                 double_ins_adjust+=200000;
	         printf("\tX87: Adjusting 100,000 for fldt\n");
                 double_ins_adjust+=100000;
	         printf("\tX87: Adjusting 600,000 for fldenv\n");
                 double_ins_adjust+=600000;
	         printf("\tX87: Adjusting 2,200,000 for frstor\n");
                 double_ins_adjust+=2200000;
	         printf("\tX87: Adjusting 2,500,000 for fxrstor\n");
                 double_ins_adjust+=2500000;

	         printf("\tSTRING: Adjusting 511,930,000 for rep lods\n");
                 double_ins_adjust+=511930000;
	         printf("\tSTRING: Adjusting 40,920,000 for rep movs\n");
                 double_ins_adjust+=40920000;
	         printf("\tSTRING: Adjusting 1,228,720,000 for rep cmps\n");
                 double_ins_adjust+=1228720000;
	         printf("\tSTRING: Adjusting 614,320,000 for rep scas\n");
                 double_ins_adjust+=614320000;
	      }
	      if (j==RETIRED_STORES) {
                 adjust_for_hw_interrupts(j);
                 printf("\tSTACK: Adjusting 100,000 for nested enter\n");
                 double_ins_adjust+=100000;
                 printf("\tCPUID: Adjusting 100,000 for cpuid\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 100,000 for fbstp\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 100,000 for fstps\n");
                 double_ins_adjust+=100000;
                 printf("\tX87: Adjusting 200,000 for fstpt\n");
                 double_ins_adjust+=200000;
                 printf("\tX87: Adjusting 1,200,000 for fstenv\n");
                 double_ins_adjust+=1200000;
                 printf("\tX87: Adjusting 4,400,000 for fsave\n");
                 double_ins_adjust+=4400000;
                 printf("\tX87: Adjusting 2,500,000 for fxsave\n");
                 double_ins_adjust+=2500000;
                 printf("\tSSE1: Adjusting 200,000 for movups (store)\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE1: Adjusting 100,000 for sfence\n");
                 double_ins_adjust+=100000;
                 printf("\tSSE2: Adjusting 200,000 for movupd (store)\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 300,000 for movdqu (store)\n");
                 double_ins_adjust+=300000;
                 printf("\tSSE2: Adjusting 100,000 for clflush\n");
                 double_ins_adjust+=100000;
                 printf("\tSSE2: Adjusting 200,000 for maskmovdqu\n");
                 double_ins_adjust+=200000;
                 printf("\tSSE2: Adjusting 100,000 for mfence\n");
                 double_ins_adjust+=100000;
                 printf("\tSTRING: Adjusting 71,610,000 for rep stos (forward)\n");
                 double_ins_adjust+=71610000;
                 printf("\tSTRING: Adjusting 40,950,000 for rep stos (backward)\n");
                 double_ins_adjust+=40950000;
                 printf("\tSTRING: Adjusting 40,920,000 for rep movs\n");
                 double_ins_adjust+=40920000;
	      }

	      break;
	 case IVYBRIDGE:
	      if (j==RETIRED_INSTRUCTIONS) {
		 adjust_for_lazy_fp();
                 adjust_for_hw_interrupts(j);
		 adjust_for_fp_exception();
		 adjust_for_pagefaults();
                 printf("\tOS: Adjusting 10,000 for page faults\n");
                 mem_adjust+=10000;
	      }
	      if (j==RETIRED_BRANCHES) {
                 adjust_for_hw_interrupts(j);
		 adjust_for_pagefaults();
	      }
	      if (j==COND_BRANCHES) dont_adjust_for_hw_interrupts(j);
	      if (j==RETIRED_UOPS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_LOADS) adjust_for_hw_interrupts(j);
	      if (j==RETIRED_STORES) adjust_for_hw_interrupts(j);
	      break;
	 }

         stats[j].adj_average=stats[j].raw_average;
         stats[j].adj_average-=hwint_adjust;
	 stats[j].adj_average-=double_ins_adjust;
	 stats[j].adj_average+=undercount_adjust;
	 stats[j].adj_average-=mem_adjust;
	 stats[j].adj_average-=fpu_adjust;

         printf("\tAdjusted Average: %lld +/- %lld\n",stats[j].adj_average,range);
         printf("\t==============================\n");
         printf("\tAdjusted diff: %lld +/- %lld\n",stats[j].adj_average-stats[j].expected[bench_type],range);
	 if ((stats[j].adj_average-stats[j].expected[bench_type]==0) && 
	     range==0 && double_ins_adjust==0){
	   printf("\t**** DETERMINISTIC ****\n");
	 }
	 else {
           printf("\tIssues: ");
	   if (hwint_adjust) printf("h");
	   if (mem_adjust) printf("p");
	   if (exception_adjust) printf("E");
	   if (double_ins_adjust) printf("D");
           if (undercount_adjust) printf("M");
	   if (fpu_adjust) printf("F");
	   printf("\n");

	 }
   }

      if (stats[j].type==INTS_VALUE) {
         total=0;
         max=stats[j].hw_interrupts[0];
         min=stats[j].hw_interrupts[0];

         for(i=0;i<RUNS;i++) {
	    total+=stats[j].hw_interrupts[i];
	    if (stats[j].hw_interrupts[i]>max) max=stats[j].hw_interrupts[i];
	    if (stats[j].hw_interrupts[i]<min) min=stats[j].hw_interrupts[i];
         }
         stats[j].raw_average=total/RUNS;

         if ((max-stats[j].raw_average) > (stats[j].raw_average-min)) range=max-stats[j].raw_average;
         else range=min-stats[j].raw_average;

         printf("\tRaw Average: %lld +/- %lld (",stats[j].raw_average,range);
         for(i=0;i<3;i++) printf("%lld, ",stats[j].hw_interrupts[i]);
         printf(")\n");
         printf("\tRaw diff: %lld +/- %lld\n",stats[j].raw_average-stats[j].expected[bench_type],range);

	 total=0;
         max=stats[j].hw_interrupts[0]-stats[j].value1[0];
         min=stats[j].hw_interrupts[0]-stats[j].value1[0];

         for(i=0;i<RUNS;i++) {
	    total+=stats[j].hw_interrupts[i]-stats[j].value1[i];
	    if (stats[j].hw_interrupts[i]-stats[j].value1[i]>max) max=stats[j].hw_interrupts[i]-stats[j].value1[i];
	    if (stats[j].hw_interrupts[i]-stats[j].value1[i]<min) min=stats[j].hw_interrupts[i]-stats[j].value1[i];	 
//	 printf("\t%lld\n",stats[j].value1[i]-stats[j].hw_interrupts[i]);
         }
         stats[j].adj_average=total/RUNS;

         if ((max-stats[j].adj_average) > (stats[j].adj_average-min)) range=max-stats[j].adj_average;
         else range=min-stats[j].adj_average;

         printf("\tAdjusted Average: %lld +/- %lld (",stats[j].adj_average,range);
         for(i=0;i<3;i++) printf("%lld, ",stats[j].hw_interrupts[i]-stats[j].value1[i]);
         printf(")\n");
         printf("\tAdjusted diff: %lld +/- %lld\n",stats[j].adj_average-stats[j].expected[bench_type],range);
      }

      if (stats[j].type==VALUE_VALUE) {

         total=0;
         max=stats[j].value1[0];
         min=stats[j].value1[0];

         for(i=0;i<RUNS;i++) {
	    total+=stats[j].value1[i];
	    if (stats[j].value1[i]>max) max=stats[j].value1[i];
	    if (stats[j].value1[i]<min) min=stats[j].value1[i];
         }
         stats[j].raw_average=total/RUNS;

         if ((max-stats[j].raw_average) > (stats[j].raw_average-min)) range=max-stats[j].raw_average;
         else range=min-stats[j].raw_average;

         printf("\tRaw Average: %lld +/- %lld (",stats[j].raw_average,range);
         for(i=0;i<3;i++) printf("%lld, ",stats[j].value1[i]);
         printf(")\n");
         printf("\tRaw diff: %lld +/- %lld\n",stats[j].raw_average-stats[j].expected[bench_type],range);

         total=0;
         max=stats[j].hw_interrupts[0];
         min=stats[j].hw_interrupts[0];

         for(i=0;i<RUNS;i++) {
	    total+=stats[j].hw_interrupts[i];
	    if (stats[j].hw_interrupts[i]>max) max=stats[j].hw_interrupts[i];
	    if (stats[j].hw_interrupts[i]<min) min=stats[j].hw_interrupts[i];
         }
         stats[j].raw_average=total/RUNS;

         if ((max-stats[j].raw_average) > (stats[j].raw_average-min)) range=max-stats[j].raw_average;
         else range=min-stats[j].raw_average;

         printf("\tRaw Average: %lld +/- %lld (",stats[j].raw_average,range);
         for(i=0;i<3;i++) printf("%lld, ",stats[j].hw_interrupts[i]);
         printf(")\n");
         printf("\tRaw diff: %lld +/- %lld\n",stats[j].raw_average-stats[j].expected[bench_type],range);
      }
   }

   return 0;
}

struct interrupts {
   long long hw;
   long long nmi;
   long long loc;
   long long res;
   long long cal;
   long long tlb;
   long long trm;
   long long thr;
   long long spu;
   long long total;
};


static void read_interrupts(char *string, int *cpus, struct interrupts **intr) {

   FILE *fff;
   char input[BUFSIZ];
   int i;
   char *ptr,*endptr,*type,*ignore;
   long long temp_interrupts=0;

//   struct interrupts *intr;

   fff=fopen(string,"r");
   if (fff==NULL) {
      fprintf(stderr,"Could not open %s\n",string);
      exit(-1);
   }
   *cpus=0;
   ignore=fgets(input,BUFSIZ,fff);
   strtok(input," \t");
   while (strtok(NULL," \t")) (*cpus)++;
   //   printf("%d cpus\n",*cpus);

   *intr=calloc(*cpus,sizeof(struct interrupts));

   while(1) {
      ignore=fgets(input,BUFSIZ,fff);

       type=strtok(input," \t");
       //printf("%s",ptr);

       temp_interrupts=0;

       for(i=0;i<*cpus;i++) {
          ptr=strtok(NULL," \t");
          if (ptr!=NULL) temp_interrupts=strtol(ptr,&endptr,10);

          if (!strncmp(type,"LOC",3)) {
	     (*intr)[i].loc+=temp_interrupts;
          }
          else if (!strncmp(type,"NMI",3)) {
	     (*intr)[i].nmi+=temp_interrupts;
          }
          else if (!strncmp(type,"RES",3)) {
	     (*intr)[i].res+=temp_interrupts;
          }
          else if (!strncmp(type,"CAL",3)) {
	     (*intr)[i].cal+=temp_interrupts;
          }
          else if (!strncmp(type,"TLB",3)) {
	     (*intr)[i].tlb+=temp_interrupts;
          }
          else {
	     (*intr)[i].hw+=temp_interrupts;
          }
	  (*intr)[i].total+=temp_interrupts;
       }

//       printf("%s %lld\n",type,(*intr)[0].total);

      if (feof(fff)) break;
   }

   fclose(fff);
   (void) ignore;
}

long long calc_interrupts_file(char *name) {

   int cpus;
   struct interrupts *before_interrupts,*after_interrupts;
   char path[BUFSIZ];

   sprintf(path,"%s.before",name);
   read_interrupts(path,&cpus,&before_interrupts);

   sprintf(path,"%s.after",name);
   read_interrupts(path,&cpus,&after_interrupts);
#if 0
   for(i=0;i<cpus;i++) {
      printf("HW%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].hw,after_interrupts[i].hw,
	                        after_interrupts[i].hw-before_interrupts[i].hw);
      printf("LOC%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].loc,after_interrupts[i].loc,
	                        after_interrupts[i].loc-before_interrupts[i].loc);
      printf("NMI%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].nmi,after_interrupts[i].nmi,
	                        after_interrupts[i].nmi-before_interrupts[i].nmi);      
      printf("RES%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].res,after_interrupts[i].res,
	                        after_interrupts[i].res-before_interrupts[i].res);
      printf("CAL%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].cal,after_interrupts[i].cal,
	                        after_interrupts[i].cal-before_interrupts[i].cal);
      printf("TLB%d\t%lld %lld %lld\n",i,
	                        before_interrupts[i].tlb,after_interrupts[i].tlb,
	                        after_interrupts[i].tlb-before_interrupts[i].tlb);      

      printf("---\n");
      printf("TOT%d:\t%lld %lld %lld\n",i,
	                        before_interrupts[i].total,after_interrupts[i].total,
	                        after_interrupts[i].total-before_interrupts[i].total);      
      printf("\n");
   }
#endif
   return after_interrupts[0].total-before_interrupts[0].total;
}
