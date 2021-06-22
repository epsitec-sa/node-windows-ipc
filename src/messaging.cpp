#include <node_api.h>
#include <napi-macros.h>

#include <windows.h>
#include <string>
#include "./wipc/WipcUtf8Listener.h"

struct WindowHandle
{
  HWND hwnd;
};

struct CopyDataListener
{
  Epsitec::Wipc::WipcUtf8Listener listener;
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

// WindowHandle* hwnd -> uint32
NAPI_METHOD(HwndToUint)
{
  unsigned int result = 0;
  NAPI_ARGV(1)

  NAPI_ARGV_BUFFER_CAST(struct WindowHandle *, hwnd, 0)

  result = (UINT32)hwnd->hwnd;
  NAPI_RETURN_UINT32(result)
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

// *CopyDataListener dataListener -> uint
NAPI_METHOD(CreateCopyDataListener)
{
  unsigned int result = 0;

  NAPI_ARGV(1)

  NAPI_ARGV_BUFFER_CAST(struct CopyDataListener *, dataListener, 0)

  auto onMessage = [&](HWND sender, LPCWSTR message)
  {


    return true;
  };

	dataListener->listener = Epsitec::Wipc::WipcUtf8Listener(onMessage);

  result = (UINT32)dataListener->listener.Handle();

  NAPI_RETURN_UINT32(result)
}

// *CopyDataListener dataListener -> int
NAPI_METHOD(DisposeCopyDataListener)
{
  int result = 0;

  NAPI_ARGV(1)

  NAPI_ARGV_BUFFER_CAST(struct CopyDataListener *, dataListener, 0)

  delete dataListener->listener;

  NAPI_RETURN_INT32(result)
}


NAPI_INIT()
{
  NAPI_EXPORT_FUNCTION(StringToHwnd)
  NAPI_EXPORT_FUNCTION(HwndToUint)

  NAPI_EXPORT_FUNCTION(SendCopyDataMessageTimeout)

  NAPI_EXPORT_FUNCTION(CreateCopyDataListener)
  NAPI_EXPORT_FUNCTION(DisposeCopyDataListener)

  NAPI_EXPORT_SIZEOF_STRUCT(WindowHandle)
  NAPI_EXPORT_ALIGNMENTOF(WindowHandle)

  NAPI_EXPORT_SIZEOF_STRUCT(CopyDataListener)
  NAPI_EXPORT_ALIGNMENTOF(CopyDataListener)
}