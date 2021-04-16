#include <node_api.h>

#include <windows.h>
#include <string>

// string serviceName, int cmd -> int
napi_value OpenSharedMemory(napi_env env, napi_callback_info info)
{
  napi_status status;

  size_t argc = 2;
  napi_value argv[2];

  napi_value napiResult;
  int result = 1;

  char serviceNameA[1000];
  size_t serviceNameARead = 0;
  int cmd = 0;

  SC_HANDLE managerHandle;
  SC_HANDLE serviceHandle;

  SERVICE_STATUS controlParms;
  DWORD retStatus;

  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Failed to parse arguments");
  }

  status = napi_get_value_string_utf8(env, argv[0], serviceNameA, 1000, &serviceNameARead);

  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Invalid serviceName was passed as argument");
  }

  status = napi_get_value_int32(env, argv[1], &cmd);

  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Invalid cmd was passed as argument");
  }

  /*
HANDLE hMapFile;
   LPCTSTR pBuf;

   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }

   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);
*/
  managerHandle = OpenSCManager(NULL, NULL, GENERIC_READ);
  if (managerHandle != NULL)
  {
    serviceHandle = OpenService(managerHandle, serviceNameA, SERVICE_USER_DEFINED_CONTROL | SERVICE_QUERY_STATUS);

    if (serviceHandle != NULL)
    {
      retStatus = ControlService(serviceHandle, cmd, &controlParms);

      if (retStatus)
      {
        // Get the return code from the service
        result = (int)controlParms.dwWin32ExitCode;
      }
      else
      {
        result = -3;
      }

      CloseServiceHandle(serviceHandle);
    }
    else
    {
      result = -2;
    }

    CloseServiceHandle(managerHandle);
  }
  else
  {
    result = -1;
  }

  status = napi_create_int32(env, result, &napiResult);

  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return napiResult;
}

napi_value Init(napi_env env, napi_value exports)
{
  napi_status status;
  napi_value fnOpenSharedMemory;
  napi_value fnCloseSharedMemory;

  status = napi_create_function(env, NULL, 0, OpenSharedMemory, NULL, &fnOpenSharedMemory);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to wrap OpenSharedMemory native function");
  }

  status = napi_set_named_property(env, exports, "open_shared_memory", fnOpenSharedMemory);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to populate open_shared_memory exports");
  }

  status = napi_create_function(env, NULL, 0, CloseSharedMemory, NULL, &fnCloseSharedMemory);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to wrap CloseSharedMemory native function");
  }

  status = napi_set_named_property(env, exports, "close_shared_memory", fnCloseSharedMemory);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to populate close_shared_memory exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)