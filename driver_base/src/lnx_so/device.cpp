/*******************************************************************************
              Copyright (c) 1983-2009 Advantech Co., Ltd.
********************************************************************************
THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY INFORMATION
WHICH IS THE PROPERTY OF ADVANTECH CORP., ANY DISCLOSURE, USE, OR REPRODUCTION,
WITHOUT WRITTEN AUTHORIZATION FROM ADVANTECH CORP., IS STRICTLY PROHIBITED. 

================================================================================
REVISION HISTORY
--------------------------------------------------------------------------------
$Log: $
--------------------------------------------------------------------------------
$NoKeywords:  $
*/

// ----------------------------------------------------------------------------
// File:        Device.cpp
// Module:      UserLib.lib
// Environment: User Mode
// Description: device common functions
// ----------------------------------------------------------------------------
#include <sys/ioctl.h>
#include <assert.h>

#include <biouserbase.h>
#include <ioctls.h>

static
ErrorCode DeviceDbgReadPort(int fd, uint32 addr, uint32 length, void *buffer)
{
   DBG_REG_IO xbuf = { addr, length, buffer };

   assert (length <= sizeof(uint32));
   return ioctl(fd, IOCTL_DEVICE_DBG_REG_IN, &xbuf) ? ErrorParamOutOfRange : Success;
}

static
ErrorCode DeviceDbgWritePort(int fd, uint32 addr, uint32 length, void *buffer)
{
   DBG_REG_IO xbuf = { addr, length, buffer };

   assert (length <= sizeof(uint32));
   return ioctl(fd, IOCTL_DEVICE_DBG_REG_OUT, &xbuf) ? ErrorParamOutOfRange : Success;
}

static
ErrorCode DeviceDbgReadRegistersAligned(int fd, uint32 startAddr, uint32 length, uint8 *buffer)
{
   // Here the word 'aligned' means:
   // (startAddr & 0x3) == ((unsigned long)buffer & 0x3)
   // at this case no local buffer is needed.
   uint8 *end = buffer + length;
   int   mis_align, rd_len;

   ErrorCode ret = Success;

   // read one dword, one word or one byte,
   // according to the start address and the rest length
   while (buffer < end) {
      mis_align = startAddr & 0x3;
      if (!mis_align) {
         rd_len = __min(end - buffer, 4);
      } else {
         rd_len = __min(end - buffer, mis_align == 2 ? 2 : 1);
      }

      if ((ret = DeviceDbgReadPort(fd, startAddr, rd_len, buffer)) != Success){
         break;
      }

      startAddr += rd_len;
      buffer    += rd_len;
   }

   return ret;
}

static
ErrorCode DeviceDbgWriteRegistersAligned(int fd, uint32 startAddr, uint32 length, uint8 *buffer)
{
   // Here the word 'aligned' means:
   // (startAddr & 0x3) == ((unsigned long)buffer & 0x3)
   // at this case no local buffer is needed.
   uint8 *end = buffer + length;
   int   mis_align, wr_len;

   ErrorCode ret = Success;

   // read one dword, one word or one byte,
   // according to the start address and the rest length
   while (buffer < end) {
      mis_align = startAddr & 0x3;
      if (!mis_align) {
         wr_len = __min(end - buffer, 4);
      } else {
         wr_len = __min(end - buffer, mis_align == 2 ? 2 : 1);
      }

      if ((ret = DeviceDbgWritePort(fd, startAddr, wr_len, buffer)) != Success){
         break;
      }

      startAddr += wr_len;
      buffer    += wr_len;
   }

   return ret;
}

ErrorCode DeviceDbgReadRegisters(int fd, uint32 startAddr, uint32 length, void *buffer)
{
   ErrorCode ret;

   if ((startAddr & 0x3) == ((unsigned long)buffer & 0x3)) {
      ret = DeviceDbgReadRegistersAligned(fd, startAddr, length, (uint8 *)buffer);
   } else {
      // align the startAddr and the buffer, then call the 'aligned' read method
      uint32 stack_buf[8 + 1]; // total 36 bytes
      uint8  *local, *aligned;
      if (length < 8 * sizeof(uint32)) {
         local = (uint8 *)stack_buf;
      } else {
         local = (uint8 *)malloc(length + 4);
         if (local == NULL){
            return ErrorMemoryNotEnough;
         }
      }

      aligned = local + (startAddr & 0x3);
      ret     = DeviceDbgReadRegistersAligned(fd, startAddr, length, aligned);
      if (ret == Success){
         memcpy(buffer, aligned, length);
      }

      if (local != (uint8 *)stack_buf){
         free(local);
      }
   }

   return ret;
}

ErrorCode DeviceDbgWriteRegisters(int fd, uint32 startAddr, uint32 length, void *buffer)
{
   ErrorCode ret;

   if ((startAddr & 0x3) == ((unsigned long)buffer & 0x3)) {
      ret = DeviceDbgWriteRegistersAligned(fd, startAddr, length, (uint8 *)buffer);
   } else {
      // align the startAddr and the buffer, then call the 'aligned' read method
      uint32 stack_buf[8 + 1]; // total 36 bytes
      uint8  *local, *aligned;
      if (length < 8 * sizeof(uint32)) {
         local = (uint8 *)stack_buf;
      } else {
         local = (uint8 *)malloc(length + 4);
         if (local == NULL){
            return ErrorMemoryNotEnough;
         }
      }

      aligned = local + (startAddr & 0x3);
      memcpy(aligned, buffer, length);

      ret = DeviceDbgWriteRegistersAligned(fd, startAddr, length, aligned);

      if (local != (uint8 *)stack_buf){
         free(local);
      }
   }

   return ret;
}
