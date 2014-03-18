/*
 * BioKrnlStub.h
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#ifndef _BDAQ_KRNL_STUB_H_
#define _BDAQ_KRNL_STUB_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <algorithm>

// bionic daq std headers
#include <biouserbase.h>
#include <biopropattr.h>
#include <ioctls.h>
#include <enumstrings.h>
#include <bdaqbase.h>

// Note: NOT change the order of the following two files
#include "../lnx_ko/kshared.h"    // shared part of kernel driver
//#include <usbcommon.h>

class BioKrnlStub
{
public:

   BioKrnlStub()
   {
      m_fd = -1;
      m_shared = NULL;
      m_mode = ModeRead;
      m_deviceNumber = -1;
   }

   virtual ~BioKrnlStub()
   {
      Cleanup();
   }

   ErrorCode Initialize(int32 devNumber, AccessMode mode)
   {
      assert(m_fd < 0);

      int fmode, pmode;
      if (mode == ModeRead){
         fmode = O_RDONLY;
         pmode = PROT_READ;
      } else {
         fmode = O_RDWR;
         pmode = PROT_READ|PROT_WRITE;
      }

      // Device number to device name
      char devname[64];
      sprintf(devname,"/dev/daq%d", devNumber);

      // Does device exist?
      struct stat stat_buf;
      if (stat(devname, &stat_buf)){
         return ErrorDeviceNotExist;
      }

      // Open the device with proper privilege
      m_fd = open(devname, fmode);
      if (m_fd < 0){
         return ErrorPrivilegeNotAvailable;
      }

      // Map the shared part of kernel device with proper privilege
      m_shared = (DEVICE_SHARED *)mmap(NULL, sizeof(*m_shared), pmode, MAP_SHARED|MAP_LOCKED, m_fd, 0);
      if (m_shared == MAP_FAILED){
         Cleanup();
         return ErrorUndefined;
      }
      if (m_shared->size != sizeof(*m_shared)){
         Cleanup();
         return ErrorDriverVerMismatch;
      }

      // cache the opened device information
      m_mode = mode;
      m_deviceNumber = devNumber;

      return Success;
   }

   void Cleanup()
   {
      if (m_shared != NULL && m_shared != MAP_FAILED) {
         munmap(m_shared, sizeof(*m_shared));
      }

      if (m_fd != -1) {
         close(m_fd);
      }

      m_fd = -1;
      m_shared = NULL;
      m_mode = ModeRead;
      m_deviceNumber = -1;
   }

   int Ioctl(int cmd, void *arg)
   {
      return ioctl(m_fd, cmd, arg);
   }

   DEVICE_SHARED * getShared()
   {
      return m_shared;
   }

   AccessMode getMode()
   {
      return m_mode;
   }

   int32 getDeviceNumber()
   {
      return m_deviceNumber;
   }

   operator int()
   {
      return m_fd;
   }

protected:
   AccessMode    m_mode;
   int32         m_deviceNumber;
   int           m_fd;
   DEVICE_SHARED *m_shared;

   //Local copy to be used in the USB device removed and reconnected
   DEVICE_SHARED m_localCache;
};

#endif /* _BDAQ_KRNL_STUB_H_ */
