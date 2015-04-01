// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "UIforETW.h"
#include "Settings.h"
#include "Utility.h"
#include "afxdialogex.h"


// CSettings dialog

IMPLEMENT_DYNAMIC(CSettings, CDialogEx)

CSettings::CSettings(CWnd* pParent /*=NULL*/, const std::wstring& exeDir, const std::wstring& wptDir)
	: CDialogEx(CSettings::IDD, pParent)
	, exeDir_(exeDir)
	, wptDir_(wptDir)
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_HEAPEXE, btHeapTracingExe_);
	DDX_Control(pDX, IDC_EXTRAPROVIDERS, btExtraProviders_);
	DDX_Control(pDX, IDC_EXTRASTACKWALKS, btExtraStackwalks_);
	DDX_Control(pDX, IDC_BUFFERSIZES, btBufferSizes_);
	DDX_Control(pDX, IDC_COPYSTARTUPPROFILE, btCopyStartupProfile_);
	DDX_Control(pDX, IDC_COPYSYMBOLDLLS, btCopySymbolDLLs_);

	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettings, CDialogEx)
	ON_BN_CLICKED(IDC_COPYSTARTUPPROFILE, &CSettings::OnBnClickedCopystartupprofile)
	ON_BN_CLICKED(IDC_COPYSYMBOLDLLS, &CSettings::OnBnClickedCopysymboldlls)
END_MESSAGE_MAP()

BOOL CSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_HEAPEXE, heapTracingExe_.c_str());
	btExtraProviders_.EnableWindow(FALSE);
	btExtraStackwalks_.EnableWindow(FALSE);
	btBufferSizes_.EnableWindow(FALSE);

	if (toolTip_.Create(this))
	{
		toolTip_.SetMaxTipWidth(400);
		toolTip_.Activate(TRUE);

		toolTip_.AddTool(&btHeapTracingExe_, L"Specify the file name of the exe to be heap traced. "
				L"Enter just the file part (with the .exe extension) not a full path. For example, "
				L"'chrome.exe'. This is for use with the heap-tracing-to-file mode.");
		toolTip_.AddTool(&btCopyStartupProfile_, L"Copy a startup profile to 'My Documents\\WPA Files' "
					L"so that next time WPA starts up it will have reasonable analysis defaults.");
		toolTip_.AddTool(&btCopySymbolDLLs_, L"Copy dbghelp.dll and symsrv.dll to the xperf directory to "
					L"try to resolve slow or failed symbol loading in WPA. See "
					L"https://randomascii.wordpress.com/2012/10/04/xperf-symbol-loading-pitfalls/ "
					L"for details.");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSettings::OnOK()
{
	heapTracingExe_ = GetEditControlText(btHeapTracingExe_);
	if (heapTracingExe_.size() <= 4 || heapTracingExe_.substr(heapTracingExe_.size() - 4, heapTracingExe_.size()) != L".exe")
	{
		AfxMessageBox(L"The heap-profiled process name must end in .exe");
		return;
	}
	CDialog::OnOK();
}


BOOL CSettings::PreTranslateMessage(MSG* pMsg)
{
	toolTip_.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}


void CSettings::OnBnClickedCopystartupprofile()
{
	const wchar_t* fileName = L"Startup.wpaProfile";
	std::wstring source = exeDir_ + fileName;

	wchar_t documents[MAX_PATH];
	if (!SHGetSpecialFolderPath(*this, documents, CSIDL_MYDOCUMENTS, TRUE))
	{
		assert(!"Failed to find My Documents directory.\n");
		return;
	}
	std::wstring dest = documents + std::wstring(L"\\WPA Files\\") + fileName;
	if (CopyFile(source.c_str(), dest.c_str(), FALSE))
	{
		AfxMessageBox(L"Copied Startup.wpaProfile to the WPA Files directory.");
	}
	else
	{
		AfxMessageBox(L"Failed to copy Startup.wpaProfile to the WPA Files directory.");
	}
}


void CSettings::OnBnClickedCopysymboldlls()
{
	// Attempt to deal with these problems:
	// https://randomascii.wordpress.com/2012/10/04/xperf-symbol-loading-pitfalls/
	const wchar_t* fileNames[] =
	{
		L"dbghelp.dll",
		L"symsrv.dll",
	};

	bool bIsWin64 = Is64BitWindows();

	bool failed = false;
	for (size_t i = 0; i < ARRAYSIZE(fileNames); ++i)
	{
		std::wstring source = exeDir_ + fileNames[i];
		if (bIsWin64)
			source = exeDir_ + L"x64\\" + fileNames[i];

		std::wstring dest = wptDir_ + fileNames[i];
		if (!CopyFile(source.c_str(), dest.c_str(), FALSE))
			failed = true;
	}

	if (failed)
		AfxMessageBox(L"Failed to copy dbghelp.dll and symsrv.dll to the WPT directory. Is WPA running?");
	else
		AfxMessageBox(L"Copied dbghelp.dll and symsrv.dll to the WPT directory. If this doesn't help "
				L"with symbol loading then consider deleting them to restore the previous state.");
}
