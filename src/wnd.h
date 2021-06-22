#include <windows.h>
#include <string>

HMODULE ExecutingModule()
{
  HMODULE handle = NULL;
  ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&ExecutingModule, &handle);
  return handle;
}

LPCTSTR RegisterWndClass(HINSTANCE hInst, LPCTSTR className, WNDPROC classWndProc)
{
  static TCHAR defClassName[] = L"Epsitec.Node.Windows.Worker";
  if (!className || !*className)
  {
    className = defClassName;
  }
  WNDCLASS wndcls;
  if (!::GetClassInfo(hInst, className, &wndcls))
  {
    wndcls.style = 0;
    wndcls.lpfnWndProc = classWndProc;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = sizeof(Window *);
    wndcls.hInstance = hInst;
    wndcls.hIcon = 0;
    wndcls.hCursor = 0;
    wndcls.hbrBackground = 0;
    wndcls.lpszMenuName = NULL;
    wndcls.lpszClassName = className;

    VERIFY(::RegisterClass(&wndcls));
  }
  return className;
}

HWND CreateWndHandle(HINSTANCE hInst, LPCTSTR className, DWORD style, HWND parent)
{
  if (!hInst)
    hInst = ExecutingModule();
  className = RegisterWndClass(hInst, className);

  auto hWnd = ::CreateWindowEx(0, className, NULL, style, 0, 0, 0, 0, parent, NULL, hInst, NULL);
  ASSERT(hWnd != 0);
  ::SetWindowText(hWnd, className);
  return hWnd;
}
