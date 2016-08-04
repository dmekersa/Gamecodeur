#ifndef EFONT_H
#define EFONT_H
#include "KPTK.h"

class EFontBank;

class EFont: public KTextFace {
public:
   EFont();
   EFont(EFontBank *font);
   ~EFont();
   // Copy constructor
   EFont(const EFont &b);
//   EFont(const EFont *b);
   // Assignement operator
   EFont & operator=(const EFont &b);

   long getTextEncoding (void);
   long getHeightPix (void);
   float getStringWidth (const char *lpszText, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1);
   float getNCharWidth (const char *lpszText, long nCharCount, float fKerning = 0, float fScale = 1);
   float getCharWidth (unsigned long c, float fKerning = 0);
   float getCharXOffset (unsigned long c);
   void setColor (float r, float g, float b, float a);
   void setBlitColor (float r, float g, float b, float a);
   void setBlend (float fBlend);
   void setAlphaMode (long nBlendingMode);
   void drawStringFromLeft (const char *lpszText, long dx, long dy, float fKerning = 0, long nTextEncodingOverride = -1);
   void drawStringFromRight (const char *lpszText, long dx, long dy, float fKerning = 0);
   void drawStringCentered (const char *lpszText, long dx1, long dx2, long dy, float fKerning = 0);
   void drawStringFromLeft (long nValue, long dx, long dy, float fKerning = 0);
   void drawStringFromRight (long nValue, long dx, long dy, float fKerning = 0);
   void drawStringCentered (long nValue, long dx1, long dx2, long dy, float fKerning = 0);
   void drawStringClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nXOffset, long nYOffset, float fKerning = 0, long nCursorPos = -1,
      KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, long nTextEncodingOverride = -1);
   bool getClippedCharCoord (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nCharPos, long &nX1Char, long &nX2Char,
      long nTextEncodingOverride = -1);
   long getClippedCharPos (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nXChar,
      bool bSnapToEdges = false, long nTextEncodingOverride = -1);
   void drawStringFromLeftF (const char *lpszText, float dx, float dy, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1);
   void drawStringFromRightF (const char *lpszText, float dx, float dy, float fKerning = 0);
   void drawStringCenteredF (const char *lpszText, float dx1, float dx2, float dy, float fKerning = 0);
   void drawStringFromLeftF (long nValue, float dx, float dy, float fKerning = 0, float fScale = 1);
   void drawStringFromRightF (long nValue, float dx, float dy, float fKerning = 0);
   void drawStringCenteredF (long nValue, float dx1, float dx2, float dy, float fKerning = 0);
   void drawStringClippedF (const char *lpszText, float dx1, float dy1, float dx2, float dy2, float fXOffset, float fYOffset, float fKerning, long nCursorPos = -1,
      KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, long nTextEncodingOverride = -1);
   bool getClippedCharCoordF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, long nCharPos, float &fX1Char, float &fX2Char,
      long nTextEncodingOverride = -1);
   long getClippedCharPosF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, float fXChar,
      bool bSnapToEdges = false, long nTextEncodingOverride = -1);
   bool doesCharExist (unsigned long nChar);
   float getYSpacing (void);
   void setYSpacing (float fYSpacing);
   void setSpaceAdjustment (float fKerning);
   void drawMultiline (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1);
   long getMultilineWidth (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1);
   long getMultilineHeight (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1);
   void drawMultilineClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning,
      long nCursorPos = -1, KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, float fFontHeight = -1);
   bool getMultilineClippedCharCoord (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning,
      long nCursorPos, long &nX1Char, long &nY1Char, long &nX2Char, long &nY2Char, float fFontHeight = -1);
   long getMultilineClippedCharPos (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning,
      long nXChar, long nYChar, float fFontHeight = -1);

   void ResetFont();
private:
   friend class EFontBank;

   EFontBank *_lpBankInst;
   KTextFace *_lpFont;
   float _colors[4];
   float _blend;
   float _fFontHeight;
   bool _bTTFFont;
};
#endif