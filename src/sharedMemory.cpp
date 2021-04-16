#include <node_api.h>
#include <napi-macros.h>

#include <windows.h>
#include <string>

struct SharedMemory
{
  HANDLE hMapFile;
  LPCTSTR pBuf;
};

// string name, int access, int memSize, SharedMemory* memoryHandle -> int
NAPI_METHOD(OpenSharedMemory)
{
  HANDLE hMapFile;
  LPCTSTR pBuf;
  int result = 0;

  NAPI_ARGV(4)

  NAPI_ARGV_UTF8(objectNameA, 1000, 0)
  NAPI_ARGV_INT32(sharedMemoryAccess, 1)
  NAPI_ARGV_INT32(memorySize, 2)
  NAPI_ARGV_BUFFER_CAST(struct SharedMemory *, memoryHandle, 3)

  memoryHandle->hMapFile = OpenFileMapping(sharedMemoryAccess, FALSE, objectNameA);
  if (memoryHandle->hMapFile == NULL)
  {
    result = 1;
    NAPI_RETURN_INT32(result)
  }

  memoryHandle->pBuf = (LPTSTR)MapViewOfFile(hMapFile, sharedMemoryAccess, 0, 0, memorySize);
  if (memoryHandle->pBuf == NULL)
  {
    CloseHandle(memoryHandle->hMapFile);

    result = 2;
    NAPI_RETURN_INT32(result);
  }

  NAPI_RETURN_INT32(result)
}

NAPI_INIT()
{
  NAPI_EXPORT_FUNCTION(OpenSharedMemory)
  NAPI_EXPORT_SIZEOF_STRUCT(SharedMemory)
}