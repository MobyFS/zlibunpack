#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bc.h"

#define OUTSZ           0x10000 // the buffer used for decompressing the data

#define IS_OK			(0)
#define FILE_NOT_OPEN	(1)
#define FILE_IS_EMPTY	(2)
#define FILE_IS_SMALL	(3)
#define ALLOC_MEM_ERR	(10)

void usage(const char *appName)
{
  printf("usage: \n\t %s <lotus_session_file> [dirOut]", appName);
}

size_t fsize(FILE *fd)
{
  size_t res = 0;
  if (fd == NULL)
  {
    return 0;
  }
  fseek(fd, 0, SEEK_END);
  res = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  return res;
}

void WriteFile(const char *fileName, uint8_t * bufer, size_t buferLen)
{
  FILE *fd = 0;
  fd = fopen(fileName, "wb");
  if (fd == NULL)
  {
    printf("error open file %s\n", fileName);
    return;
  }

  fwrite(bufer, 1, buferLen, fd);
  fclose(fd);
}


int procLotusSession(const char * lotusFile)
{

  FILE *fd = 0;
  fd = fopen(lotusFile, "rb");
  if (fd == NULL)
  {
    printf("error open file %s\n", lotusFile);
    return FILE_NOT_OPEN;
  }
  size_t fileSize = fsize(fd);
  if (fileSize <= 0)
  {
    fclose(fd);
    printf("file %s is empty\n");
    return FILE_IS_EMPTY;
  }
  uint8_t *inBufer = 0;
  inBufer = (uint8_t*)malloc(fileSize);
  if (!inBufer)
  {
    fclose(fd);
    return ALLOC_MEM_ERR;
  }

  int readingSize = fread(inBufer, 1, fileSize, fd);
  if (readingSize <= 0)
  {
    free(inBufer);
    fclose(fd);
    return FILE_IS_EMPTY;
  }
  fclose(fd);

  BCWorker *bcWorker = new BCWorker();
  size_t unpackBuferLen = 0x10000;
  uint8_t *unpackBufer = 0;
  unpackBufer = (uint8_t *)malloc(unpackBuferLen);
  if (unpackBufer)
  {
    if (bcWorker->unpackBlockZlib(inBufer, fileSize-4, unpackBufer, &unpackBuferLen) == 0)
    {
      std::string resFile = std::string(lotusFile) + "_unpack";
      WriteFile(resFile.c_str(), unpackBufer, unpackBuferLen);
    }
  }
 
  delete(bcWorker);
  free(inBufer);
}

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    usage(argv[0]);
    return 0;
  }

  if (argc == 2)//TODO:нужен параметр
  {
    procLotusSession(argv[1]);
  }
  else
  {
    //для директории
  }
  return 0;
}