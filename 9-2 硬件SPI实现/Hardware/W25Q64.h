#ifndef __W25Q64_H
#define __W25Q64_H


void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void WriteEnable(void);
void WaitBusy(void);
void Clean4KB(uint32_t Address);
void WriteData(uint32_t Address, uint8_t *DataArray, uint16_t NUM);
void ReadData(uint32_t Address, uint8_t *DataArray, uint32_t NUM);

#endif
