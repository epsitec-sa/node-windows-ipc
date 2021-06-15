#include <node_api.h>
#include <napi-macros.h>

#include <windows.h>
#include <sddl.h>
#include <string>

struct WindowHandle
{
  HWND hwnd;
};

// string strHwnd, WindowHandle* hwnd -> int
NAPI_METHOD(StringToHwnd)
{
  int result = 0;

  NAPI_ARGV(2)

  NAPI_ARGV_UTF8(strHwndA, 1000, 0)
  NAPI_ARGV_BUFFER_CAST(struct WindowHandle *, hwnd, 1)

  std::string s(strHwndA);
  hwnd->hwnd = (HWND)std::stoul(s, nullptr, 16);

  NAPI_RETURN_INT32(result)
}

// *WindowHandle targetHwnd, *WindowHandle senderHwnd, byte* data, int dataSize, int sendMessageTimeoutFlags, int timeout -> int
NAPI_METHOD(SendCopyDataMessageTimeout)
{
  int result = 0;

  NAPI_ARGV(6)

  NAPI_ARGV_BUFFER_CAST(struct WindowHandle *, targetHwnd, 0)
  NAPI_ARGV_BUFFER_CAST(struct WindowHandle *, senderHwnd, 1)
  NAPI_ARGV_BUFFER_CAST(char *, data, 2)
  NAPI_ARGV_INT32(dataSize, 3)
  NAPI_ARGV_INT32(sendMessageTimeoutFlags, 4)
  NAPI_ARGV_INT32(timeout, 5)

  COPYDATASTRUCT cds;
  cds.dwData = 0;
  cds.cbData = dataSize;
  cds.lpData = data;

  auto res = SendMessageTimeoutA(targetHwnd->hwnd, WM_COPYDATA, (WPARAM)senderHwnd->hwnd, (LPARAM)(LPVOID)&cds, sendMessageTimeoutFlags, timeout, NULL);
  if (res == 0)
  {
    result = 1;
    NAPI_RETURN_INT32(result)
  }

  NAPI_RETURN_INT32(result)
}


NAPI_INIT()
{
  NAPI_EXPORT_FUNCTION(StringToHwnd)
  NAPI_EXPORT_FUNCTION(SendCopyDataMessageTimeout)

  NAPI_EXPORT_SIZEOF_STRUCT(WindowHandle)
  NAPI_EXPORT_ALIGNMENTOF(WindowHandle)
}