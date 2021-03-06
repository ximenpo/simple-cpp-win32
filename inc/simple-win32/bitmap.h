#ifndef BITMAPHDC_H_WIN_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define BITMAPHDC_H_WIN_66BC65DB_AFF6_43C8_8654_D1A2801635E2

//	获取HBITMAP大小
bool	Bitmap_GetSize(HBITMAP hbmp, long& nWidth, long& nHeight);

//
//  内存Bitmap绘图类
//
class Bitmap_HDC {
public:
    Bitmap_HDC();
    virtual		~Bitmap_HDC();

    void		initialize(int nWidth, int nHeight, HDC hDC = NULL, COLORREF clrBackground = RGB(255,255,255));
    void		uninitialize();

    void		set_use_bitmap(bool bSelect);

    HBITMAP     get_HBITMAP() {
        return  m_hBitmap;
    }
    int         get_bitmap_width() {
        return  m_nBmpWidth;
    }
    int         get_bitmap_height() {
        return  m_nBmpWidth;
    }
    HDC			get_HDC() {
        return m_hDC;
    }
    operator	HDC() {
        return m_hDC;
    }

protected:
    HDC			m_hDC;
    HBITMAP		m_hBitmap;
    HBITMAP		m_hOldBmp;
    int         m_nBmpWidth;
    int         m_nBmpHeight;
};

#endif  //BITMAPHDC_H__089BB269_EC67_495d_885A_CAB04219E370__INCLUDED_