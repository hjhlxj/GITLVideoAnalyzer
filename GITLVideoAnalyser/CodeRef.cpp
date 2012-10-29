// CodeRef.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyser.h"
#include "CodeRef.h"
#include "afxdialogex.h"


// CodeRef dialog

IMPLEMENT_DYNAMIC(CodeRef, CDialogEx)

CodeRef::CodeRef(CWnd* pParent /*=NULL*/)
	: CDialogEx(CodeRef::IDD, pParent)
	, m_edit(0)
{

}

CodeRef::~CodeRef()
{
}

void CodeRef::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDV_MinMaxInt(pDX, m_edit, 0, 99999999);
}


BEGIN_MESSAGE_MAP(CodeRef, CDialogEx)
END_MESSAGE_MAP()


// CodeRef message handlers
