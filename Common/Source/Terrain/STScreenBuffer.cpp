/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: STScreenBuffer.cpp,v 8.2 2010/12/12 17:09:24 root Exp root $
*/

#include "externs.h"
#include "STScreenBuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CSTScreenBuffer
CSTScreenBuffer::CSTScreenBuffer(int nWidth, int nHeight, LKColor clr) : RawBitmap(nWidth, nHeight), m_pBufferTmp(NULL) {

 
#ifdef GREYSCALE
    #warning "not implemented for greyscale, check if need (also for non greyscale...)"
    // m_pBufferTmp is only used by Blur method, so no need to alocate. ( blur is not implemented in GreyScale Mode )
#else
    m_pBufferTmp = (BGRColor*)malloc(sizeof(BGRColor)*GetHeight()*GetCorrectedWidth());
    std::fill_n(GetBuffer(), GetHeight()*GetCorrectedWidth(), BGRColor(clr.Blue(), clr.Green(), clr.Red()));
#endif
    
}

CSTScreenBuffer::~CSTScreenBuffer() {
    if (m_pBufferTmp) {
        free(m_pBufferTmp);
        m_pBufferTmp = NULL;
    }
}

void CSTScreenBuffer::DrawStretch(LKSurface& Surface, const RECT& rcDest, int scale) {
    const unsigned cx = rcDest.right - rcDest.left;
    const unsigned cy = rcDest.bottom - rcDest.top;
    int cropsize;
    if ((cy < GetWidth()) || ScreenLandscape) {
        cropsize = GetHeight() * cx / cy;
    } else {
        // NOT TESTED!
        cropsize = GetWidth();
    }
    StretchTo(cropsize/scale, GetHeight()/scale, Surface , cx, cy);
}

void CSTScreenBuffer::HorizontalBlur(unsigned int boxw) {
#ifndef GREYSCALE
    const unsigned int muli = (boxw * 2 + 1);
    BGRColor* src = GetBuffer();
    BGRColor* dst = m_pBufferTmp;
    BGRColor *c;

    const unsigned int off1 = boxw + 1;
    const unsigned int off2 = GetCorrectedWidth() - boxw - 1;
    const unsigned int right = GetCorrectedWidth() - boxw;

    for (unsigned int y = GetHeight(); y--;) {
        unsigned int tot_r = 0;
        unsigned int tot_g = 0;
        unsigned int tot_b = 0;
        unsigned int x;

        c = src + boxw - 1;
        for (x = boxw; x--; --c) {
            tot_r += c->value.Red();
            tot_g += c->value.Green();
            tot_b += c->value.Blue();
        }
        
        for (x = 0; x < GetCorrectedWidth(); ++x) {
            unsigned int acc = muli;
            if (x > boxw) {
                c = src - off1;
                tot_r -= c->value.Red();
                tot_g -= c->value.Green();
                tot_b -= c->value.Blue();
            } else {
                acc += x - boxw;
            }
            if (x < right) {
                c = src + boxw;
                tot_r += c->value.Red();
                tot_g += c->value.Green();
                tot_b += c->value.Blue();
            } else {
                acc += off2 - x;
            }
            (*dst) = BGRColor((uint8_t) (tot_r / acc),(uint8_t) (tot_g / acc), (uint8_t) (tot_b / acc));

            src++;
            dst++;
        }
    }

    // copy it back to main buffer
    memcpy((char*) GetBuffer(), (char*) m_pBufferTmp, GetCorrectedWidth() * GetHeight() * sizeof (BGRColor));
#else
    #warning "HorizontalBlur ? not implemented for greyscale, Needed ?"
#endif    
}

void CSTScreenBuffer::VerticalBlur(unsigned int boxh) {
#ifndef GREYSCALE
    BGRColor* src = GetBuffer();
    BGRColor* dst = m_pBufferTmp;
    BGRColor *c, *d, *e;

    const unsigned int muli = (boxh * 2 + 1);
    const unsigned int iboxh = GetCorrectedWidth() * boxh;
    const unsigned int off1 = iboxh + GetCorrectedWidth();
    const unsigned int off2 = GetHeight() - boxh - 1;
    const unsigned int bottom = GetHeight() - boxh;

    for (unsigned int x = GetCorrectedWidth(); --x;) {
        unsigned int tot_r = 0;
        unsigned int tot_g = 0;
        unsigned int tot_b = 0;
        unsigned int y;

        c = d = src + x;
        e = dst + x;
        for (y = boxh; y--; c += GetCorrectedWidth()) {
            tot_r += c->value.Red();
            tot_g += c->value.Green();
            tot_b += c->value.Blue();
        }

        for (y = 0; y < GetHeight(); ++y) {
            unsigned int acc = muli;
            if (y > boxh) {
                c = d - off1;
                tot_r -= c->value.Red();
                tot_g -= c->value.Green();
                tot_b -= c->value.Blue();
            } else {
                acc += y - boxh;
            }
            if (y < bottom) {
                c = d + iboxh;
                tot_r += c->value.Red();
                tot_g += c->value.Green();
                tot_b += c->value.Blue();
            } else {
                acc += off2 - y;
            }
            (*e) = BGRColor((uint8_t) (tot_r / acc),(uint8_t) (tot_g / acc), (uint8_t) (tot_b / acc));
            d += GetCorrectedWidth();
            e += GetCorrectedWidth();
        }
    }

    // copy it back to main buffer
    memcpy((char*) GetBuffer(), (char*) m_pBufferTmp, GetCorrectedWidth() * GetHeight() * sizeof (BGRColor));
#else
    #warning "VerticalBlur : not implemented for greyscale, Needed ?"
#endif       
}
