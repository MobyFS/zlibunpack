#include "bc.h"

#include <zlib.h>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif


#define LN_PROC_NO_ERR       0
#define LN_PROC_SMALL_DATA  (1)
#define LN_STRUCT_ERR       (2)
#define LN_STRUCT_L1_ERR    (3)
#define LN_NOT_FILE         (4)
#define MEM_ALLOC_ERR       (11)

#define LN_TOP_ID_0F00	0x00f0
#define LN_TOP_ID_0E78	0x780e
#define LN_TP0_ID		    0x06

BCWorker::BCWorker()
{
  mPldBuffersLen = 0;
}

BCWorker::~BCWorker()
{

}





int BCWorker::unpackBlockZlib(uint8_t *packBufer, size_t packBuferLen, uint8_t *unpackBufer, size_t *unpackBuferLen){
  z_stream    z;
  memset(&z, 0, sizeof(z));
  int zipwbits = 15;
  if (inflateInit2(&z, zipwbits) != Z_OK)
  {
    return -1;//TODO:
  }  
  z.next_in = packBufer;
  z.avail_in = packBuferLen;
  z.next_out = unpackBufer;
  z.avail_out = *unpackBuferLen;
  inflateReset(&z);
  int zerr = inflate(&z, Z_SYNC_FLUSH);
  if (z.total_out == 0)
  {
    inflateEnd(&z);
    return -1;
  }
  *unpackBuferLen = z.total_out;
  inflateEnd(&z);
  return 0;
}

int BCWorker::unpackBlockDefl(uint8_t *packBufer, size_t packBuferLen, uint8_t *unpackBufer, size_t *unpackBuferLen)
{
  z_stream    z;
  memset(&z, 0, sizeof(z));
  int zipwbits = -15;
  if (inflateInit2(&z, zipwbits) != Z_OK)
  {
    return -1;//TODO:
  }
  inflateReset(&z);
  z.next_in = packBufer;
  z.avail_in = packBuferLen;
  z.next_out = unpackBufer;
  z.avail_out = *unpackBuferLen;
  int zerr = inflate(&z, Z_SYNC_FLUSH);
  if (zerr != Z_OK)
  {
    inflateEnd(&z);
    return -1;
  }
  *unpackBuferLen = z.total_out;
  inflateEnd(&z);
  return 0;
}

