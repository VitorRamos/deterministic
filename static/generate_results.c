#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "version.h"
#include "arch.h"

static char dirname[BUFSIZ + 32];

static int set_generic_modelname(int vendor, int family, int model)
{
   if (vendor == VENDOR_AMD)
   {
      /* 0fh */
      if (family == 15)
      {
         strcpy(cpuinfo.generic_modelname, "opteron");
         event_table = NULL; /* TODO */
      }
      /* 10h */
      else if (family == 16)
      {
         strcpy(cpuinfo.generic_modelname, "fam10h");
         event_table = &fam10h_event_table;
      }
      /* 14h -- bobcat */
      else if (family == 20)
      {
         strcpy(cpuinfo.generic_modelname, "fam14h");
         event_table = &fam14h_event_table;
      }
      /* 15h -- bulldozer/piledriver */
      else if (family == 21)
      {
         strcpy(cpuinfo.generic_modelname, "fam15h");
         event_table = &fam15h_event_table;
      }
      /* 16h -- jaguar */
      else if (family == 22)
      {
         strcpy(cpuinfo.generic_modelname, "fam16h");
         event_table = NULL; /* TODO */
      }
      else
      {
         strcpy(cpuinfo.generic_modelname, "UNKNOWN");
      }
   }

   if (vendor == VENDOR_INTEL)
   {
      if (family == 15)
      {
         switch (model)
         {
         case 0:
         case 1:
         case 2:
            strcpy(cpuinfo.generic_modelname, "pentium4");
            event_table = &pentium4_event_table;
            break;
         case 3:
         case 4:
         case 6:
            strcpy(cpuinfo.generic_modelname, "pentiumd");
            event_table = &pentium4_event_table;
            break;
         default:
            strcpy(cpuinfo.generic_modelname, "UNKNOWN");
            break;
         }
      }
      else if (family == 11)
      {
         /* MIC */
         strcpy(cpuinfo.generic_modelname, "UNKNOWN");
      }
      else if (family == 6)
      {
         switch (model)
         {

         case 14: /* yonah */
            strcpy(cpuinfo.generic_modelname, "UNKNOWN");
            break;

         case 15: /* Memrom/Conroe */
         case 22: /* Memrom-L/Conroe-L */
         case 23: /* Penryn/Wolfdale */
         case 29: /* Dunnington */
            strcpy(cpuinfo.generic_modelname, "core2");
            event_table = &core2_event_table;
            break;

         case 26: /* Bloomfield */
         case 30: /* Lynnfield */
            /* Nehalem */
            strcpy(cpuinfo.generic_modelname, "nehalem");
            event_table = &nhm_event_table;
            break;

         case 46: /* Beckton */
            /* Nehalem-EX */
            strcpy(cpuinfo.generic_modelname, "nehalem-ex");
            event_table = &nhm_event_table;
            break;

         case 28: /* Atom */
         case 38: /* Lincroft */
         case 39: /* Penwell */
         case 53: /* Cloverview */
            strcpy(cpuinfo.generic_modelname, "atom");
            event_table = &atom_event_table;
            break;

         case 54: /* Cedarview */
            strcpy(cpuinfo.generic_modelname, "atom-cedarview");
            event_table = &atom_event_table;
            break;

         case 37: /* Clarkdale */
         case 44: /* Gulftown */
            strcpy(cpuinfo.generic_modelname, "westmere");
            event_table = &wsm_event_table;
            break;

         case 47: /* Xeon E7 */
            strcpy(cpuinfo.generic_modelname, "westmere-ex");
            event_table = &wsm_event_table;
            break;

         case 42: /* Sandybridge */
            strcpy(cpuinfo.generic_modelname, "sandybridge");
            event_table = &snb_event_table;
            break;
         case 45: /* Sandybridge EP (Romley) */
            strcpy(cpuinfo.generic_modelname, "sandybridge-ep");
            event_table = &snb_event_table;
            break;

         case 58: /* Ivy Bridge */
            strcpy(cpuinfo.generic_modelname, "ivybridge");
            event_table = &ivb_event_table;
            break;

         case 62: /* Ivy Bridge EP */
            strcpy(cpuinfo.generic_modelname, "ivybridge-ep");
            event_table = &ivb_event_table;
            break;

         case 60: /* Haswell */
         case 69:
         case 70:
         case 71:
            strcpy(cpuinfo.generic_modelname, "haswell");
            event_table = &hsw_event_table;
            break;

         /* Haswell-EP */
         case 63:
            strcpy(cpuinfo.generic_modelname, "haswell-ep");
            event_table = &hsw_event_table;
            break;

         /* Broadwell-EP */
         case 79:
            strcpy(cpuinfo.generic_modelname, "broadwell-ep");
            event_table = &bdw_event_table;
            break;

         /* Skylake */
         case 94:
            strcpy(cpuinfo.generic_modelname, "skylake");
            event_table = &skl_event_table;
            break;

         default:
            strcpy(cpuinfo.generic_modelname, "UNKNOWN");
            break;
         }
      }
      else
      {
         strcpy(cpuinfo.generic_modelname, "UNKNOWN");
      }
   }

   if (event_table == NULL)
   {
      strcpy(cpuinfo.generic_modelname, "UNKNOWN");
      fprintf(stderr, "Error!  Unknown machine type!\n");
      return -1;
   }
   return 0;
}

static int get_cpuinfo(void)
{

   int ret = 0;
   FILE *fff;
   char temp_string[BUFSIZ], temp_string2[BUFSIZ];
   char *result;

   fff = fopen("/proc/cpuinfo", "r");
   if (fff == NULL)
      return -1;

   while (1)
   {
      result = fgets(temp_string, BUFSIZ, fff);
      if (result == NULL)
         break;

      if (!strncmp(temp_string, "vendor_id", 9))
      {
         sscanf(temp_string, "%*s%*s%s", temp_string2);

         if (!strncmp(temp_string2, "GenuineIntel", 12))
         {
            cpuinfo.vendor = VENDOR_INTEL;
         }
         else if (!strncmp(temp_string2, "AuthenticAMD", 12))
         {
            cpuinfo.vendor = VENDOR_AMD;
         }
         else
         {
            cpuinfo.vendor = VENDOR_UNKNOWN;
         }
      }

      if (!strncmp(temp_string, "cpu family", 10))
      {
         sscanf(temp_string, "%*s%*s%*s%d", &cpuinfo.family);
      }

      if (!strncmp(temp_string, "model", 5))
      {
         sscanf(temp_string, "%*s%s", temp_string2);
         if (temp_string2[0] == ':')
         {
            sscanf(temp_string, "%*s%*s%d", &cpuinfo.model);
         }
         else
         {
            result = strstr(temp_string, ":");
            strcpy(cpuinfo.modelname, result + 2);
            cpuinfo.modelname[strlen(cpuinfo.modelname) - 1] = 0;
         }
      }

      if (!strncmp(temp_string, "stepping", 8))
      {
         sscanf(temp_string, "%*s%*s%d", &cpuinfo.stepping);
      }
   }

   ret = set_generic_modelname(cpuinfo.vendor, cpuinfo.family, cpuinfo.model);
   if (ret < 0)
      return -2;

   printf("Family:    %d\n", cpuinfo.family);
   printf("Model:     %d\n", cpuinfo.model);
   printf("Stepping:  %d\n", cpuinfo.stepping);
   printf("Modelname: %s\n", cpuinfo.modelname);
   printf("Generic:   %s\n", cpuinfo.generic_modelname);

   fclose(fff);

   return 0;
}

static int create_output_dir(void)
{

   int result, next_avail;

   result = mkdir("results", 0755);
   if (result < 0)
   {
      if (errno == EEXIST)
      {
         /* this is OK */
      }
      else
      {
         fprintf(stderr, "ERROR creating results dir!\n");
         return -1;
      }
   }

   sprintf(dirname, "results/%s", cpuinfo.generic_modelname);

   result = mkdir(dirname, 0755);
   if (result < 0)
   {
      if (errno == EEXIST)
      {
         /* this is OK */
      }
      else
      {
         fprintf(stderr, "ERROR creating %s dir!\n", dirname);
         return -1;
      }
   }

   next_avail = 0;

   while (1)
   {

      sprintf(dirname, "results/%s/%d", cpuinfo.generic_modelname, next_avail);

      result = mkdir(dirname, 0755);
      if (result < 0)
      {
         if (errno == EEXIST)
         {
            /* already there move on */
         }
         else
         {
            fprintf(stderr, "ERROR creating %s dir!\n", dirname);
            return -1;
         }
      }
      else
      {
         printf("Found available directory: %s\n", dirname);
         return 0;
      }

      next_avail++;
   }

   return 0;
}

static int generate_results(char *directory, char *name,
                            char *event1, char *event2,
                            char *name1, char *name2,
                            char *type, int count)
{

   int i, ret = 0;
   char filename[BUFSIZ], temp_string[BUFSIZ];
   FILE *fff;
   struct utsname uname_buf;

   sprintf(filename, "%s/%s.%s", directory, name, type);
   printf("Generating file %s (%s/%s) ", filename, name1, name2);

   if (!strncmp(event1, "NONE", 4))
   {
      printf("Skipped!\n");
      return 0;
   }

   uname(&uname_buf);

   fff = fopen(filename, "w");
   if (fff == NULL)
      return -1;
   fprintf(fff, "### System info\n");
   fprintf(fff, "Kernel:    %s %s\n", uname_buf.sysname, uname_buf.release);
   fprintf(fff, "Interface: perf_event\n");
   fprintf(fff, "Hostname:  %s\n", uname_buf.nodename);
   fprintf(fff, "Family:    %d\n", cpuinfo.family);
   fprintf(fff, "Model:     %d\n", cpuinfo.model);
   fprintf(fff, "Stepping:  %d\n", cpuinfo.stepping);
   fprintf(fff, "Modelname: %s\n", cpuinfo.modelname);
   fprintf(fff, "Generic:   %s\n", cpuinfo.generic_modelname);
   fprintf(fff, "Counters used: %s/%s\n", name1, name2);
   fprintf(fff, "generate_results version: %s\n", DETERMINISTIC_VERSION);
   fprintf(fff, "Runs:      %d\n", count);
   fclose(fff);

   for (i = 0; i < count; i++)
   {
      printf("%d ", i);
      fflush(stdout);

      /* before */
      sprintf(temp_string, "echo \"### Interrupts %d before\">> %s",
              i, filename);
      ret |= system(temp_string);
      sprintf(temp_string, "cat /proc/interrupts >> %s", filename);
      ret |= system(temp_string);
      /* results */
      sprintf(temp_string, "echo \"### Perf Results %d\">> %s",
              i, filename);
      ret |= system(temp_string);
      sprintf(temp_string, "taskset -c 0 perf stat "
                           "--log-fd 2 " // comment out this line if using old perf
                           "-e %s,%s,cs:u "
                           "./binaries/retired_instr.%s.x86_64 1>/dev/null 2>>%s",
              event1, event2, type, filename);
      ret |= system(temp_string);
      /* after */
      sprintf(temp_string, "echo \"### Interrupts %d after\">> %s",
              i, filename);
      ret |= system(temp_string);
      sprintf(temp_string, "cat /proc/interrupts >> %s", filename);
      ret |= system(temp_string);

      if (ret < 0) // Ignoring erros
      {
         fprintf(stderr, "Error! system command\n");
      }
   }
   printf("\n");

   return 0;
}

int main(int argc, char **argv)
{
   int RUNS= 10;
   if(argc == 2)
      RUNS= atoi(argv[1]);

   get_cpuinfo();
   create_output_dir();
   char *programs[10] = {"inst_retired",
                         "branches_retired",
                         "cond_branches",
                         "loads_retired",
                         "stores_retired",
                         "uops_retired",
                         "muls_retired",
                         "divs_retired",
                         "fp_retired",
                         "sse_retired"};
   char *ptypes[4] = {"int", "fp", "sse", "all"};
   char* evs[10]= {event_table->ret_instr_event,
                   event_table->branches_event,
                   event_table->cond_branches_event,
                   event_table->loads_event,
                   event_table->stores_event,
                   event_table->uops_event,
                   event_table->muls_event,
                   event_table->divs_event,
                   event_table->fp1_event,
                   event_table->sse_event };
   char* evs_name[10]= {event_table->ret_instr_name,
                   event_table->branches_name,
                   event_table->cond_branches_name,
                   event_table->loads_name,
                   event_table->stores_name,
                   event_table->uops_name,
                   event_table->muls_name,
                   event_table->divs_name,
                   event_table->fp1_name,
                   event_table->sse_name };
   int i, j;
   for (i = 0; i < 10; i++)
   {
      for (j = 0; j < 4; j++)
      {
         if(event_table->hw_int_event == NULL)
            continue;
         generate_results(dirname, programs[i],
                          evs[i],
                          event_table->hw_int_event,
                          evs_name[i],
                          event_table->hw_int_name,
                          ptypes[j], RUNS);
      }
   }

   return 0;
}
