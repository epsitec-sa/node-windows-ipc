#include <node_api.h>
#include <napi-macros.h>

#include <windows.h>
#include <string>

struct SharedMemoryHandle
{
  HANDLE hMapFile;
  LPCTSTR pBuf;
};

// string name, int pageAccess, int fileMapAccess, int memSize, SharedMemory* memoryHandle -> int
NAPI_METHOD(CreateSharedMemory)
{
  int result = 0;

  NAPI_ARGV(5)

  NAPI_ARGV_UTF8(objectNameA, 1000, 0)
  NAPI_ARGV_INT32(sharedMemoryPageAccess, 1)
  NAPI_ARGV_INT32(sharedMemoryFileMapAccess, 2)
  NAPI_ARGV_INT32(memorySize, 3)
  NAPI_ARGV_BUFFER_CAST(struct SharedMemoryHandle *, memoryHandle, 4)

  memoryHandle->hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, sharedMemoryPageAccess, 0, memorySize, objectNameA);
  if (memoryHandle->hMapFile == NULL)
  {
    result = GetLastError();
    NAPI_RETURN_INT32(result)
  }

  memoryHandle->pBuf = (LPTSTR)MapViewOfFile(memoryHandle->hMapFile, sharedMemoryFileMapAccess, 0, 0, memorySize);
  if (memoryHandle->pBuf == NULL)
  {
    CloseHandle(memoryHandle->hMapFile);

    result = 0 - GetLastError();
    NAPI_RETURN_INT32(result);
  }

  NAPI_RETURN_INT32(result)
}

// string name, int fileMapAccess, int memSize, SharedMemory* memoryHandle -> int
NAPI_METHOD(OpenSharedMemory)
{
  int result = 0;

  NAPI_ARGV(4)

  NAPI_ARGV_UTF8(objectNameA, 1000, 0)
  NAPI_ARGV_INT32(sharedMemoryPageAccess, 1)
  NAPI_ARGV_INT32(memorySize, 2)
  NAPI_ARGV_BUFFER_CAST(struct SharedMemoryHandle *, memoryHandle, 3)

  memoryHandle->hMapFile = OpenFileMapping(sharedMemoryPageAccess, FALSE, objectNameA);
  if (memoryHandle->hMapFile == NULL)
  {
    result = GetLastError();
    NAPI_RETURN_INT32(result)
  }

  memoryHandle->pBuf = (LPTSTR)MapViewOfFile(memoryHandle->hMapFile, sharedMemoryPageAccess, 0, 0, memorySize);
  if (memoryHandle->pBuf == NULL)
  {
    CloseHandle(memoryHandle->hMapFile);

    result = 0 - GetLastError();
    NAPI_RETURN_INT32(result);
  }

  NAPI_RETURN_INT32(result)
}

NAPI_INIT()
{
  NAPI_EXPORT_FUNCTION(CreateSharedMemory)
  NAPI_EXPORT_FUNCTION(OpenSharedMemory)

  NAPI_EXPORT_SIZEOF_STRUCT(SharedMemoryHandle)
  NAPI_EXPORT_ALIGNMENTOF(SharedMemoryHandle)
}