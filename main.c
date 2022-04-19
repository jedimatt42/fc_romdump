#include "fc_api.h"

unsigned char romdump[8192];

int fc_main(char* args) {
  struct DeviceServiceRoutine* dsr;
  char fname_buffer[30];
  fc_parse_path_param(args, &dsr, fname_buffer, PR_REQUIRED);
  if (dsr == 0) {
    fc_tputs("error no save file specified\n\n");
    fc_tputs("Note: on TIPI or classic99 use .?X headerless modifier\n");
    fc_tputs("ROMDUMP <file>\n");
  }

  char crubaseStr[40];
  fc_strset(crubaseStr, 0, 40);
  int crubaseToDump = fc_htoi(crubaseStr);
  while(crubaseToDump == 0) {
    fc_tputs("\nEnter CRUBASE of ROM to dump: ");
    fc_strset(crubaseStr, 0, 40);
    fc_getstr(crubaseStr, 5, 1);
    crubaseToDump = fc_htoi(crubaseStr);
  }

  // enable the rom, but then copy it to free ram, so we can turn it off
  // and use the disk to save it.
  __asm__("mov %0,r12\n\tsbo 0" : : "r"(crubaseToDump) : "r12");
  unsigned char* romsource = (unsigned char*) 0x4000;  
  int found = 0xAA == romsource[0];
  int copied = 0;
  for(int i = 0; i < 8192; i++) {
    romdump[i] = romsource[i];
    if (romdump[i]) {
      copied++;
    }
  }
  __asm__("mov %0,r12\n\tsbz 0" : : "r"(crubaseToDump) : "r12");

  int limit = 8192;
  if (copied <= 2048) {
    limit = 2048;
  } else if (copied <= 4096) {
    limit = 4096;
  }

  if (!found) {
    fc_tputs("\nNo ROM detected\n");
    return 1;
  }

  fc_tputs("\nDetected ROM size ");
  fc_tputs(fc_uint2str(limit));
  fc_tputs("\nOverride: ");
  char limitstr[20];
  fc_strset(limitstr, 0, 20);
  fc_strcpy(limitstr, fc_uint2str(limit));
  fc_getstr(limitstr, 5, 1);
  int altlimit = fc_atoi(limitstr);
  if (altlimit) {
    limit = altlimit;
  }

  fc_tputs("\nSaving ROM to DIS/FIX 128\n");
  struct PAB pab;
  int err = fc_dsr_open(dsr, &pab, fname_buffer, DSR_TYPE_OUTPUT, 128);
  if (err) {
    fc_tputs("error opening file to save\n");
    return 1;
  }

  for(int i = 0; i < limit; i+=128) {
    fc_dsr_write(dsr, &pab, (char*)(romdump + i), 128);
  }

  fc_dsr_close(dsr, &pab);

  fc_tputs("\n");
  return 0;
}
