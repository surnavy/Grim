#pragma once
#include <vector>

class CMFCApplication1Dlg : public CDialogEx
{
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEnChangeEditSettings();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonRandomMove();
	afx_msg void OnBnClickedButtonReset();
	static UINT RandomMoveThread(LPVOID pParam);
	DECLARE_MESSAGE_MAP();

private:
	// --- 데이터 관련 ---
	std::vector<CPoint> m_points;

	CPoint m_circleCenter = { 0, 0 };
	double m_circleRadius = 0.0;
	bool   m_bCircleReady = false; // 초기화 습관
	int m_nClickCount = 0;

	// --- 설정 관련 ---
	int m_nPointRadius = 5;
	int m_nLineThickness = 2;

	// --- 상태 제어 ---
	int  m_nDraggingPointIndex = -1; // -1로 시작하는 것이 안전
	bool m_bIsDragging = false;
	bool m_bIsUpdating = false;

	// --- 상수 (나중에 여기만 바꾸면 됨) ---
	const double MAX_CIRCLE_RADIUS = 3000.0; // 렉 방지용 임계값
	const int    CLICK_MARGIN = 10;     // 클릭 판정 여유분

	// --- 헬퍼 함수 ---
	void DrawCircle(CDC* pDC, CPoint center, double radius, int thickness, bool fill);
	bool CalculateCircumcircle(CPoint p1, CPoint p2, CPoint p3);

	CWinThread* m_pThread = nullptr; // 스레드 포인터 저장
	bool m_bStopThread = false;      // 스레드 중단 플래그

	// --- UI 컨트롤 ---
	CSliderCtrl m_sliderRadius;
	CSliderCtrl m_sliderThickness;
};