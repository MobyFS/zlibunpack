/***************************************************************************
*                         FUNCTIONS DECLARATION
***************************************************************************/

#ifndef _BC_H_
#define _BC_H_


//#include <srg\srgc.h>
//#include <srg/meta.h>
//#include <srg/srgc/utils.h>
//#include <srg/srgc/media.h>
#include <stdint.h>
#include <list>

typedef struct HeaderTP0{
  uint8_t Tp0Id;
  uint8_t Tp0Type;
  uint16_t Tp0PaketLen;
}stHeaderTP0;

typedef struct HeaderPldTP0{
  uint8_t PldType;
  uint8_t flags;
  uint16_t PldLen;
}stHeaderPldTP0;

typedef struct PldBlock{
  stHeaderPldTP0 *PldHeader = 0;
  uint8_t *BlockData = 0;
  uint16_t BlockSize = 0;
}stPldBlock;

typedef struct CommandTP0{
  uint8_t CommandType;
  uint8_t xz;
  uint16_t Tp0PaketLen;
}stCommandTP0;

typedef struct CmdBlockL0{
  //uint8_t *Commands = 0;
  std::list<stCommandTP0*> Commands;
  //uint16_t CommandsLen = 0;
  std::list<stPldBlock> PldBlocks;
  uint32_t PldLen = 0;
  uint32_t UnpackPldLen = 0;
  uint32_t EmptyBlockLen = 0;
  uint8_t payloadType = 0x06;
  uint8_t payloadFlags = 0x00;//���� payloadType = 0x07:
  //0x01 = ��� ��������� 2 ����� 48�7 �������� zlib
  //0x02 = ����� ��������������
  //
}stCmdBlockL0;

typedef struct PayloadBuf
{
  uint8_t *buf;
  size_t bufLen;
}stPayloadBuf;

typedef struct SessL1Header
{
  uint32_t MsgL0ID; //������������� ���������(lotus mail) � ������� ������������ ���������� ����
  uint32_t FileID; //������������� ����������� �����
  uint32_t SessionSize;//����� ���������� ������
  uint32_t SessionOffsetInFile;// ������� �����(������ ������ ����������� �����)

}stSessL1Header;

typedef struct SessL1
{
  stSessL1Header Header;
  uint8_t* SessL1Data;
  bool sessionFull;//������ ��������� ���������
  size_t SessionOffsetWrite;
}stSessL1;

typedef struct SessL1PaketHeader
{
  uint16_t packetSize; //������ ������(���������� 0x700f) ��������� ���������� ��� �������
  uint16_t packetNumber;//��
  uint16_t payloadSize;//����� ������
  uint16_t blockId; //������������� �����(������ l1)
  uint16_t blockType; //0x0023 - file 0x0008 - email???
  uint16_t lastPacket;//if 0x0001 - last packet, 0x0000 curent  
}stSessL1PaketHeader;


class BCWorker
{
public:
  BCWorker();
  ~BCWorker();
  int unpackBlockDefl(uint8_t *packBufer, size_t packBuferLen, uint8_t *unpackBufer, size_t *unpackBuferLen);
  int unpackBlockZlib(uint8_t *packBufer, size_t packBuferLen, uint8_t *unpackBufer, size_t *unpackBuferLen);

  //srg_data_buffer_list *mL0BufRes;
  size_t mPldBuffersLen;
};

#endif //_BC_H_