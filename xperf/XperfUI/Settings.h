#pragma once

#include <string>

// CSettings dialog

class CSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettings();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

	std::string heapTracingExe_;

protected:
	CEdit btHeapTracingExe_;
	CEdit btTraceDir_;
	CEdit btTempTraceDir_;
	CComboBox btBufferSizes_;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnOK();
};
