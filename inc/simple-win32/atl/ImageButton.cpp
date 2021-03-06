#include <atlbase.h>

#include "ImageButton.h"


ImageButton::ImageButton(void)
    :	m_tracking(false)
    ,	m_btndown(false) {
    SubImage_Clear(&NormalImage);
    SubImage_Clear(&SelectedImage);
    SubImage_Clear(&HoverImage);
    SubImage_Clear(&DisabledImage);
}


ImageButton::~ImageButton(void) {
}

UINT ImageButton::state() const {
    ATLASSERT(::IsWindow(m_hWnd));
    return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0L);
}

void ImageButton::set_state(bool selected) {
    ATLASSERT(::IsWindow(m_hWnd));
    ::SendMessage(m_hWnd, BM_SETSTATE, selected, 0L);
}

LRESULT ImageButton::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    return 1;
}


LRESULT ImageButton::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    if (m_tracking) {
        TRACKMOUSEEVENT t = {
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd,
            0
        };
        if (::_TrackMouseEvent(&t)) {
            m_tracking = false;
        }
    }

    m_btndown = true;

    bHandled = FALSE;
    return 0;
}


LRESULT ImageButton::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    m_btndown = false;

    bHandled = FALSE;
    return 0;
}


LRESULT ImageButton::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ATLASSERT(!m_tracking);
        m_btndown = false;
    }

    bHandled = FALSE;
    return 0;
}


LRESULT ImageButton::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
    if ((m_btndown) &&(::GetCapture() == m_hWnd)) {
        POINT p2;
        p2.x = LOWORD(lParam);
        p2.y = HIWORD(lParam);

        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((state() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            set_state(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                m_tracking = true;
                Invalidate();
            }
        }
    }

    bHandled = FALSE;
    return 0;
}


LRESULT ImageButton::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    m_tracking = false;
    Invalidate();

    return 0;
}


LRESULT ImageButton::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
    POINT	mouse_pos;
    UINT	curr_state	= this->state();
    if ((m_btndown) &&(::GetCapture() == m_hWnd) &&(::GetCursorPos(&mouse_pos))) {
        if (::WindowFromPoint(mouse_pos) == m_hWnd) {
            if ((curr_state & BST_PUSHED) != BST_PUSHED) {
                set_state(true);
                return -1;
            }
        } else {
            if ((curr_state & BST_PUSHED) == BST_PUSHED) {
                set_state(false);
                return -1;
            }
        }
    }

    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;

    Gdiplus::Rect rect(
        lpdis->rcItem.left,
        lpdis->rcItem.top,
        lpdis->rcItem.right	- lpdis->rcItem.left,
        lpdis->rcItem.bottom- lpdis->rcItem.top
    );

    SubImage	img	= this->NormalImage;
    if (lpdis->itemState & ODS_SELECTED) {
        if(NULL != this->SelectedImage.image) {
            img	= this->SelectedImage;
        }
    } else if (lpdis->itemState & ODS_DISABLED) {
        img	= this->DisabledImage;
    } else if (m_tracking) {
        if(NULL != this->HoverImage.image) {
            img	= this->HoverImage;
        }
    }

    if(NULL != img.image) {
        Gdiplus::Graphics	graphics(lpdis->hDC);
        graphics.DrawImage(img.image, rect,
                           img.rect.X, img.rect.Y,
                           img.rect.Width, img.rect.Height,
                           Gdiplus::UnitPixel
                          );
    }

    return 0;
}
