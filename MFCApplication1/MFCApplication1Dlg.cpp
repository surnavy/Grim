
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RADIUS, m_sliderRadius);
	DDX_Control(pDX, IDC_SLIDER_THICKNESS, m_sliderThickness);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_MOVE, &CMFCApplication1Dlg::OnBnClickedButtonRandomMove)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMFCApplication1Dlg::OnBnClickedButtonReset)
	ON_EN_CHANGE(IDC_EDIT_POINT_RADIUS, &CMFCApplication1Dlg::OnEnChangeEditSettings)
	ON_EN_CHANGE(IDC_EDIT_LINE_THICKNESS, &CMFCApplication1Dlg::OnEnChangeEditSettings)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bIsUpdating = true;

	srand((unsigned int)time(NULL));
	m_nPointRadius = 5;
	m_nLineThickness = 2;

	m_sliderRadius.SetRange(1, 100);
	m_sliderRadius.SetPos(m_nPointRadius); // 초기값 5

	// 두께 슬라이더 설정 (1 ~ 20)
	m_sliderThickness.SetRange(1, 20);
	m_sliderThickness.SetPos(m_nLineThickness); // 초기값 2

	SetDlgItemInt(IDC_EDIT_POINT_RADIUS, m_nPointRadius);
	SetDlgItemInt(IDC_EDIT_LINE_THICKNESS, m_nLineThickness);

	m_bIsUpdating = false; // [수정 포인트 2] 초기화 끝났으므로 해제

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this);
	CWnd* pCanvas = GetDlgItem(IDC_STATIC_CANVAS);
	if (!pCanvas) return;

	CRect rect;
	pCanvas->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.SetWindowOrg(rect.left, rect.top);
	memDC.FillSolidRect(rect, RGB(255, 255, 255));

	// 격자 그리기 (단순 펜 하나만 사용)
	CPen gridPen(PS_DOT, 1, RGB(230, 230, 230));
	CPen* pOldPen = memDC.SelectObject(&gridPen);
	for (int i = rect.left; i < rect.right; i += 50) { memDC.MoveTo(i, rect.top); memDC.LineTo(i, rect.bottom); }
	for (int i = rect.top; i < rect.bottom; i += 50) { memDC.MoveTo(rect.left, i); memDC.LineTo(rect.right, i); }
	memDC.SelectObject(pOldPen); // 격자 펜 해제

	// 원과 점 그리기 (DrawCircle이 색상/두께를 알아서 처리함)
	if (m_bCircleReady) {
		DrawCircle(&memDC, m_circleCenter, m_circleRadius, m_nLineThickness, false);
	}

	for (const auto& pt : m_points) {
		DrawCircle(&memDC, pt, m_nPointRadius, 1, true);
	}

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, rect.left, rect.top, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::DrawCircle(CDC* pDC, CPoint center, double radius, int thickness, bool fill) {
	if (radius <= 0 || pDC == nullptr) return;

	// 두께 보정: thickness가 0이면 아무것도 안 보이므로 최소 1로 고정
	int nSafeThickness = (thickness < 1) ? 1 : thickness;
	int r = (int)radius;
	COLORREF color = fill ? RGB(255, 0, 0) : RGB(0, 0, 255);

	if (fill) {
		// [채우기 모드] - 중점 알고리즘으로 내부 꽉 채우기
		int x = 0;
		int y = r;
		int d = 3 - 2 * r;
		while (y >= x) {
			for (int i = center.x - x; i <= center.x + x; ++i) {
				pDC->SetPixelV(i, center.y + y, color);
				pDC->SetPixelV(i, center.y - y, color);
			}
			for (int i = center.x - y; i <= center.x + y; ++i) {
				pDC->SetPixelV(i, center.y + x, color);
				pDC->SetPixelV(i, center.y - x, color);
			}
			if (d < 0) d = d + 4 * x + 6;
			else { d = d + 4 * (x - y) + 10; y--; }
			x++;
		}
	}
	else {
		// [테두리 모드] - thickness만큼 겹쳐 그려서 두께 표현
		// 반지름을 (r - 두께/2)부터 (r + 두께/2)까지 반복
		int startR = r - (nSafeThickness / 2);
		int endR = startR + nSafeThickness;

		for (int curR = startR; curR < endR; ++curR) {
			int x = 0;
			int y = curR;
			int d = 3 - 2 * curR;
			while (y >= x) {
				pDC->SetPixelV(center.x + x, center.y + y, color);
				pDC->SetPixelV(center.x - x, center.y + y, color);
				pDC->SetPixelV(center.x + x, center.y - y, color);
				pDC->SetPixelV(center.x - x, center.y - y, color);
				pDC->SetPixelV(center.x + y, center.y + x, color);
				pDC->SetPixelV(center.x - y, center.y + x, color);
				pDC->SetPixelV(center.x + y, center.y - x, color);
				pDC->SetPixelV(center.x - y, center.y - x, color);

				if (d < 0) d = d + 4 * x + 6;
				else { d = d + 4 * (x - y) + 10; y--; }
				x++;
			}
		}
	}
}

bool CMFCApplication1Dlg::CalculateCircumcircle(CPoint p1, CPoint p2, CPoint p3) {
	 double D = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
	 if (abs(D) < 1e-6) {
		 CString strCenter;
		 strCenter.Format(_T("원이 아닙니다."), m_circleCenter.x, m_circleCenter.y);
		 SetDlgItemText(IDC_STATIC_CENTER, strCenter);
		 return false;
	 }

	 double p1_sq = p1.x * p1.x + p1.y * p1.y;
	 double p2_sq = p2.x * p2.x + p2.y * p2.y;
	 double p3_sq = p3.x * p3.x + p3.y * p3.y;

	 m_circleCenter.x = (long)((p1_sq * (p2.y - p3.y) + p2_sq * (p3.y - p1.y) + p3_sq * (p1.y - p2.y)) / D);
	 m_circleCenter.y = (long)((p1_sq * (p3.x - p2.x) + p2_sq * (p1.x - p3.x) + p3_sq * (p2.x - p1.x)) / D);
	 m_circleRadius = sqrt(pow(p1.x - m_circleCenter.x, 2) + pow(p1.y - m_circleCenter.y, 2));

	 if (m_circleRadius > 5000) {
		 return false;
	 }

	 CString strCenter;
	 strCenter.Format(_T("중심: (%d, %d)"), m_circleCenter.x, m_circleCenter.y);
	 SetDlgItemText(IDC_STATIC_CENTER, strCenter);
	 return true;
}

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pCanvas = GetDlgItem(IDC_STATIC_CANVAS);
	CRect rectCanvas;
	pCanvas->GetWindowRect(&rectCanvas);
	ScreenToClient(&rectCanvas);

	if (rectCanvas.PtInRect(point)) {
		// 1. 기존 점 드래그 체크
		for (int i = 0; i < (int)m_points.size(); ++i) {
			double dist = sqrt(pow(m_points[i].x - point.x, 2) + pow(m_points[i].y - point.y, 2));
			if (dist <= m_nPointRadius + 10) {
				m_bIsDragging = true;
				m_nDraggingPointIndex = i;
				SetCapture();
				return; // 점을 잡았으므로 여기서 종료 (새 점을 찍지 않음)
			}
		}

		// 2. 새 점 추가 (3개 미만일 때)
		if (m_points.size() < 3) {
			m_points.push_back(point);
			int currentIdx = (int)m_points.size(); // 현재 몇 번째 점인지

			// [중요] 좌표 UI 실시간 갱신
			UINT nStaticIDs[] = { IDC_STATIC_P1, IDC_STATIC_P2, IDC_STATIC_P3 };
			CString strCoord;
			strCoord.Format(_T("P%d: (%d, %d)"), currentIdx, point.x, point.y);
			SetDlgItemText(nStaticIDs[currentIdx - 1], strCoord);

			// 3. 점이 3개가 되면 원 계산
			if (m_points.size() == 3) {
				m_bCircleReady = CalculateCircumcircle(m_points[0], m_points[1], m_points[2]);
			}

			// 화면 갱신
			InvalidateRect(&rectCanvas, FALSE);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsDragging && m_nDraggingPointIndex != -1)
	{
		// [추가] 캔버스 영역 가져오기 및 좌표 제한 로직
		CWnd* pCanvas = GetDlgItem(IDC_STATIC_CANVAS);
		if (pCanvas)
		{
			CRect rect;
			pCanvas->GetWindowRect(&rect);
			ScreenToClient(&rect); // 다이얼로그 기준 좌표로 변환

			// 마우스 좌표가 캔버스 영역을 벗어나지 않도록 고정 (Clamping)
			if (point.x < rect.left)   point.x = rect.left;
			if (point.x > rect.right)  point.x = rect.right;
			if (point.y < rect.top)    point.y = rect.top;
			if (point.y > rect.bottom) point.y = rect.bottom;
		}

		// 1. 보정된 좌표를 현재 마우스 위치로 업데이트
		m_points[m_nDraggingPointIndex] = point;

		// 2. 바뀐 좌표로 원 다시 계산
		if (CalculateCircumcircle(m_points[0], m_points[1], m_points[2])) {
			m_bCircleReady = true;
		}
		else {
			m_bCircleReady = false;
		}

		// 3. 우측 좌표 표시 Static Text 실시간 업데이트
		UINT nStaticIDs[] = { IDC_STATIC_P1, IDC_STATIC_P2, IDC_STATIC_P3 };
		CString str;
		str.Format(_T("P%d: (%d, %d)"), m_nDraggingPointIndex + 1, point.x, point.y);
		SetDlgItemText(nStaticIDs[m_nDraggingPointIndex], str);

		// 4. 화면 갱신
		Invalidate(FALSE);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsDragging)
	{
		m_bIsDragging = false;
		m_nDraggingPointIndex = -1;
		ReleaseCapture(); // 마우스 제어권 해제
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCApplication1Dlg::OnBnClickedButtonRandomMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bCircleReady) {
		AfxMessageBox(_T("먼저 점 3개를 찍어 원을 생성하세요."));
		return;
	}

	// 워커 스레드 생성
	AfxBeginThread(RandomMoveThread, this);
}

UINT CMFCApplication1Dlg::RandomMoveThread(LPVOID pParam)
{
	CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)pParam;

	CWnd* pCanvas = pDlg->GetDlgItem(IDC_STATIC_CANVAS);
	CRect rect;
	pCanvas->GetWindowRect(&rect);
	pDlg->ScreenToClient(&rect);

	UINT nStaticIDs[] = { IDC_STATIC_P1, IDC_STATIC_P2, IDC_STATIC_P3 };

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			// 1. 랜덤 좌표 생성
			pDlg->m_points[j].x = rect.left + (rand() % rect.Width());
			pDlg->m_points[j].y = rect.top + (rand() % rect.Height());

			// 2. [추가] UI 텍스트도 랜덤 좌표에 맞춰 실시간 업데이트
			CString str;
			str.Format(_T("P%d: (%d, %d)"), j + 1, pDlg->m_points[j].x, pDlg->m_points[j].y);
			pDlg->SetDlgItemText(nStaticIDs[j], str);
		}

		// 3. 원 계산 및 화면 갱신
		pDlg->CalculateCircumcircle(pDlg->m_points[0], pDlg->m_points[1], pDlg->m_points[2]);
		pDlg->Invalidate(FALSE);

		Sleep(500); // 초당 2회
	}
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 데이터 초기화
	m_points.clear();
	m_nClickCount = 0;
	m_bCircleReady = false;
	m_bIsDragging = false;

	// UI 텍스트 초기화
	SetDlgItemText(IDC_STATIC_P1, _T("P1:"));
	SetDlgItemText(IDC_STATIC_P2, _T("P2:"));
	SetDlgItemText(IDC_STATIC_P3, _T("P3:"));
	SetDlgItemText(IDC_STATIC_CENTER, _T("중심:"));

	// 화면 깨끗이 지우기
	Invalidate(TRUE);
}

BOOL CMFCApplication1Dlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	// 점 위에 마우스가 있으면 손가락 모양(IDC_HAND)으로 변경
	for (const auto& ptPoint : m_points) {
		double distSq = pow(ptPoint.x - pt.x, 2) + pow(ptPoint.y - pt.y, 2);
		double thresholdSq = pow(m_nPointRadius + 5, 2);
		if (distSq <= thresholdSq) {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
			return TRUE;
		}
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CMFCApplication1Dlg::OnEnChangeEditSettings()
{
	if (m_bIsUpdating) return; // 슬라이더나 초기화 로직에 의해 바뀐 거라면 무시!

	if (!m_sliderRadius.GetSafeHwnd() || !m_sliderThickness.GetSafeHwnd()) return;

	m_nPointRadius = GetDlgItemInt(IDC_EDIT_POINT_RADIUS);
	m_nLineThickness = GetDlgItemInt(IDC_EDIT_LINE_THICKNESS);

	m_sliderRadius.SetPos(m_nPointRadius);
	m_sliderThickness.SetPos(m_nLineThickness);

	if (m_points.size() == 3) {
		CalculateCircumcircle(m_points[0], m_points[1], m_points[2]);
	}
	Invalidate(FALSE);
}

void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		m_bIsUpdating = true; // "지금 내가 수동으로 바꾸는 중이야!" 라고 알림

		m_nPointRadius = m_sliderRadius.GetPos();
		m_nLineThickness = m_sliderThickness.GetPos();

		SetDlgItemInt(IDC_EDIT_POINT_RADIUS, m_nPointRadius);
		SetDlgItemInt(IDC_EDIT_LINE_THICKNESS, m_nLineThickness);

		m_bIsUpdating = false; // 업데이트 끝났으니 다시 이벤트 허용

		Invalidate(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}