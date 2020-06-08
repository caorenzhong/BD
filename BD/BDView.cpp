// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// BDView.cpp: CBDView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BD.h"
#endif

#include "BDDoc.h"
#include "BDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBDView

IMPLEMENT_DYNCREATE(CBDView, CView)

BEGIN_MESSAGE_MAP(CBDView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBDView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CBDView 构造/析构

CBDView::CBDView() noexcept
{
	// TODO: 在此处添加构造代码

}

CBDView::~CBDView()
{
}

BOOL CBDView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBDView 绘图

void CBDView::OnDraw(CDC* /*pDC*/)
{
	CBDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CBDView 打印


void CBDView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CBDView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBDView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBDView 诊断

#ifdef _DEBUG
void CBDView::AssertValid() const
{
	CView::AssertValid();
}

void CBDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBDDoc* CBDView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBDDoc)));
	return (CBDDoc*)m_pDocument;
}
#endif //_DEBUG


// CBDView 消息处理程序


void CBDView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	RECT rt;
	GetClientRect(&rt);//获取自己窗口的大小

	CefWindowInfo window_info;
	window_info.SetAsChild(GetSafeHwnd(), rt); // cefsimple中的是用SetAsPopup，这里嵌入控件要用SetAsChild
	m_CefHandler = new SimpleHandler(false);
	//std::string url = "http://www.baidu.com";
	//std::string url = "http://61.183.231.150:8003/topeakgis-pc/#/";
	std::string url = "D:/Code/CEF3/geodemo.html";
	
	CefBrowserSettings browser_settings;
	CefBrowserHost::CreateBrowser(window_info, m_CefHandler, CefString(url), browser_settings, NULL, NULL);


}


void CBDView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_CefHandler != nullptr)
	{
		if (m_CefHandler->GetCefBrowser(0) != nullptr)
		{
			auto hwnd = m_CefHandler->GetCefBrowser(0)->GetHost()->GetWindowHandle();
			auto rect = RECT{ 0 };
			GetClientRect(&rect);

			::SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		}
	}


}
