#pragma once

#include "atlwin.h"

#include	"simple-win32/gdiplus/subimage.h"

class ImageButton :
    public CWindowImpl<ImageButton, CWindow, CWinTraits<WS_VISIBLE|WS_CHILD|BS_OWNERDRAW,0> > {
public:
    ImageButton(void);
    ~ImageButton(void);

    DECLARE_WND_SUPERCLASS(NULL, _T("BUTTON"))

public:
    SubImage	NormalImage
    ,			SelectedImage
    ,			HoverImage
    ,			DisabledImage
    ;

    UINT		state() const;
    void		set_state(bool selected);

private:
    bool		m_tracking;
    bool		m_btndown;

private:
    BEGIN_MSG_MAP(ImageButton)
    MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    END_MSG_MAP()

private:
    LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

