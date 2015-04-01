#include "stdafx.h"
#include "DirectoryMonitor.h"
#include <assert.h>

DirectoryMonitor::DirectoryMonitor(CWnd* pMainWindow)
	: mainWindow_(pMainWindow)
{
}

// This function monitors the traceDir_ directory and sends a message to the main thread
// whenever anything changes. That's it. All UI work is done in the main thread.
DWORD WINAPI DirectoryMonitor::DirectoryMonitorThreadStatic(LPVOID pVoidThis)
{
	DirectoryMonitor* pThis = static_cast<DirectoryMonitor*>(pVoidThis);
	return pThis->DirectoryMonitorThread();
}

DWORD DirectoryMonitor::DirectoryMonitorThread()
{
	HANDLE hChangeHandle = FindFirstChangeNotification(traceDir_->c_str(), FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);

	if (hChangeHandle == INVALID_HANDLE_VALUE)
	{
		assert(0);
		return 0;
	}

	HANDLE handles[] = { hChangeHandle, hShutdownRequest_ };
	for (;;)
	{
		DWORD dwWaitStatus = WaitForMultipleObjects(ARRAYSIZE(handles), &handles[0], FALSE, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:
			mainWindow_->PostMessage(WM_UPDATETRACELIST, 0, 0);
			if (FindNextChangeNotification(hChangeHandle) == FALSE)
			{
				assert(0);
				return 0;
			}
			break;
		case WAIT_OBJECT_0 + 1:
			// Shutdown requested.
			return 0;

		default:
			assert(0);
			return 0;
		}
	}

	assert(0);

	return 0;
}

void DirectoryMonitor::StartThread(const std::wstring* traceDir)
{
	assert(hThread_ == 0);
	assert(hShutdownRequest_ == 0);
	traceDir_ = traceDir;
	// No error checking -- what could go wrong?
	hThread_ = CreateThread(nullptr, 0, DirectoryMonitorThreadStatic, this, 0, 0);
	hShutdownRequest_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);;
}

DirectoryMonitor::~DirectoryMonitor()
{
	if (hThread_)
	{
		SetEvent(hShutdownRequest_);
		WaitForSingleObject(hThread_, INFINITE);
		CloseHandle(hThread_);
		CloseHandle(hShutdownRequest_);
	}
}