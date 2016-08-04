#include "FontModeDefs.h"

#include "KGame.h"
#include "global.h"
#include "FontBank.h"
#include "EFont.h"

#define R _colors[0]
#define G _colors[1]
#define B _colors[2]
#define A _colors[3]

EFont::EFont(): KTextFace(),_lpBankInst(NULL),_lpFont(NULL),_fFontHeight(0),_blend(1),_bTTFFont(true) {
}
EFont::EFont(EFontBank *font): KTextFace() {
   KAssert(font->_nHeight >0);
   _lpBankInst = font;
   _lpFont = _lpBankInst->_lpFont;
   _fFontHeight = _lpBankInst->_nHeight;

   setColor(floorf(1.0f), floorf(1.0f), floorf(1.0f), floorf(1.0f));
   setBlend(1.0f);
   ResetFont();

   _bTTFFont = true;
   if (_lpBankInst->_type == EFontBank::KTEXT)
      _bTTFFont = false;

   _lpBankInst->_lpEFontRef.push_front(this);
}
EFont::~EFont(){
   if (_lpBankInst != NULL) {
      _lpBankInst->_lpEFontRef.remove(this);
#ifdef NOTTF
      if (!EFontBank::_bCleanUp && _lpBankInst->_lpEFontRef.size() == 0) {
         XDELETE(_lpBankInst);
      }
#endif
      _lpBankInst = NULL;
      _lpFont = NULL;
   }
}
EFont::EFont(const EFont &b): KTextFace(), _lpBankInst(NULL),_lpFont(NULL),_fFontHeight(0),_blend(1),_bTTFFont(true)  {
   R=b.R;G=b.G;B=b.B;A=b.A;
   _blend = b._blend;
   _lpBankInst = b._lpBankInst;
   _fFontHeight = b._fFontHeight;
   _lpFont= b._lpFont;
   _bTTFFont = b._bTTFFont;
   if (_lpBankInst != NULL)
      _lpBankInst->_lpEFontRef.push_front(this);
}
//EFont::EFont(const EFont *b): _lpBankInst(NULL),_lpFont(NULL),_fFontHeight(0),_blend(1),_bTTFFont(true)  {
//   R=b->R;G=b->G;B=b->B;A=b->A;
//   _blend = b->_blend;
//   _lpBankInst = b->_lpBankInst;
//   _fFontHeight = b->_fFontHeight;
//   _lpFont= b->_lpFont;
//   _bTTFFont = b->_bTTFFont;
//   _lpBankInst->_lpEFontRef.push_front(this);
//}
EFont& EFont::operator=(const EFont &b) {
   EFontBank *tmp = b._lpBankInst;
   if (_lpBankInst != NULL && _lpBankInst != tmp) {
      _lpBankInst->_lpEFontRef.remove(this);
      if (!EFontBank::_bCleanUp && _lpBankInst->_lpEFontRef.size() == 0) {
         XDELETE(_lpBankInst);
      }
      _lpBankInst = NULL;
      _lpFont = NULL;
   }
   R=b.R;G=b.G;B=b.B;A=b.A;
   _blend = b._blend;
   _lpBankInst = b._lpBankInst;
   _fFontHeight = b._fFontHeight;
   _lpFont= b._lpFont;
   _bTTFFont = b._bTTFFont;
   if (_lpBankInst != tmp)
      _lpBankInst->_lpEFontRef.push_front(this);
   return *this;
}
void EFont::ResetFont() {
   if (_lpBankInst->_lpLastMod == this)
      return;
   _lpFont->setColor(R,G,B,A);
   _lpFont->setBlend(_blend);
   _lpBankInst->_lpLastMod = this;
}

long EFont::getTextEncoding (void) {
   return _lpFont->getTextEncoding();
}
long EFont::getHeightPix (void){
   return _fFontHeight;
}
float EFont::getStringWidth (const char *lpszText, float fKerning, float fScale, long nTextEncodingOverride){
   return _lpFont->getStringWidth (lpszText, fKerning, fScale, nTextEncodingOverride);
}
float EFont::getNCharWidth (const char *lpszText, long nCharCount, float fKerning, float fScale){
   return _lpFont->getNCharWidth (lpszText, nCharCount, fKerning, fScale);
}
float EFont::getCharWidth (unsigned long c, float fKerning){
   return _lpFont->getCharWidth (c, fKerning);
}
float EFont::getCharXOffset (unsigned long c){
   return _lpFont->getCharXOffset (c);
}
void EFont::setColor (float r, float g, float b, float a){
   R=r;G=g;B=b;A=a;
   if (_lpBankInst->_lpLastMod == this)
      _lpBankInst->_lpLastMod = NULL;
}
void EFont::setBlitColor (float r, float g, float b, float a){
   R=r;G=g;B=b;A=a;
   if (_lpBankInst->_lpLastMod == this)
      _lpBankInst->_lpLastMod = NULL;   
}
void EFont::setBlend (float fBlend){
   _blend=fBlend;
   if (_lpBankInst->_lpLastMod == this)
      _lpBankInst->_lpLastMod = NULL;
}
void EFont::setAlphaMode (long nBlendingMode){
   _lpFont->setAlphaMode (nBlendingMode);
}
void EFont::drawStringFromLeft (const char *lpszText, long dx, long dy, float fKerning, long nTextEncodingOverride){
    ResetFont();
   _lpFont->drawStringFromLeft (lpszText, dx, dy, fKerning, nTextEncodingOverride);
}
void EFont::drawStringFromRight (const char *lpszText, long dx, long dy, float fKerning){
    ResetFont();
   _lpFont->drawStringFromRight (lpszText, dx, dy, fKerning);
}
void EFont::drawStringCentered (const char *lpszText, long dx1, long dx2, long dy, float fKerning){
    ResetFont();
   _lpFont->drawStringCentered (lpszText, dx1, dx2, dy, fKerning);
}
void EFont::drawStringFromLeft (long nValue, long dx, long dy, float fKerning){
    ResetFont();
   _lpFont->drawStringFromLeft (nValue, dx, dy, fKerning);
}
void EFont::drawStringFromRight (long nValue, long dx, long dy, float fKerning){
    ResetFont();
   _lpFont->drawStringFromRight (nValue, dx, dy, fKerning);
}
void EFont::drawStringCentered (long nValue, long dx1, long dx2, long dy, float fKerning){
    ResetFont();
   _lpFont->drawStringCentered (nValue, dx1, dx2, dy, fKerning);
}
void EFont::drawStringClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nXOffset, long nYOffset, float fKerning, long nCursorPos, KTextFaceCursorCallBack lpCursorCallBack, void *lpCallBackData, long nTextEncodingOverride){
    ResetFont();
   _lpFont->drawStringClipped (lpszText, dx1, dy1, dx2, dy2, nXOffset, nYOffset, fKerning, nCursorPos, lpCursorCallBack, lpCallBackData, nTextEncodingOverride);
}
bool EFont::getClippedCharCoord (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nCharPos, long &nX1Char, long &nX2Char, long nTextEncodingOverride){
   return _lpFont->getClippedCharCoord (lpszText, dx1, dx2, nXOffset, fKerning, nCharPos, nX1Char, nX2Char, nTextEncodingOverride);
}
long EFont::getClippedCharPos (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nXChar, bool bSnapToEdges, long nTextEncodingOverride){
   return _lpFont->getClippedCharPos (lpszText, dx1, dx2, nXOffset, fKerning, nXChar, bSnapToEdges, nTextEncodingOverride);
}
void EFont::drawStringFromLeftF (const char *lpszText, float dx, float dy, float fKerning, float fScale, long nTextEncodingOverride){
    ResetFont();
   _lpFont->drawStringFromLeftF (lpszText, dx, dy, fKerning, fScale, nTextEncodingOverride);
}
void EFont::drawStringFromRightF (const char *lpszText, float dx, float dy, float fKerning){
    ResetFont();
   _lpFont->drawStringFromRightF (lpszText, dx, dy, fKerning);
}
void EFont::drawStringCenteredF (const char *lpszText, float dx1, float dx2, float dy, float fKerning){
    ResetFont();
   _lpFont->drawStringCenteredF (lpszText, dx1, dx2, dy, fKerning);
}
void EFont::drawStringFromLeftF (long nValue, float dx, float dy, float fKerning, float fScale){
    ResetFont();
   _lpFont->drawStringFromLeftF (nValue, dx, dy, fKerning, fScale);
}
void EFont::drawStringFromRightF (long nValue, float dx, float dy, float fKerning){
    ResetFont();
   _lpFont->drawStringFromRightF (nValue, dx, dy, fKerning);
}
void EFont::drawStringCenteredF (long nValue, float dx1, float dx2, float dy, float fKerning){
    ResetFont();
   _lpFont->drawStringCenteredF (nValue, dx1, dx2, dy, fKerning);
}
void EFont::drawStringClippedF (const char *lpszText, float dx1, float dy1, float dx2, float dy2, float fXOffset, float fYOffset, float fKerning, long nCursorPos, KTextFaceCursorCallBack lpCursorCallBack, void *lpCallBackData, long nTextEncodingOverride){
    ResetFont();
   _lpFont->drawStringClippedF (lpszText, dx1, dy1, dx2, dy2, fXOffset, fYOffset, fKerning, nCursorPos, lpCursorCallBack, lpCallBackData, nTextEncodingOverride);
}
bool EFont::getClippedCharCoordF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, long nCharPos, float &fX1Char, float &fX2Char, long nTextEncodingOverride){
   return _lpFont->getClippedCharCoordF (lpszText, dx1, dx2, fXOffset, fKerning, nCharPos, fX1Char, fX2Char, nTextEncodingOverride);
}
long EFont::getClippedCharPosF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, float fXChar, bool bSnapToEdges, long nTextEncodingOverride){
   return _lpFont->getClippedCharPosF (lpszText, dx1, dx2, fXOffset, fKerning, fXChar, bSnapToEdges, nTextEncodingOverride);
}
bool EFont::doesCharExist (unsigned long nChar){
   return _lpFont->doesCharExist (nChar);
}
float EFont::getYSpacing (void){
   return _lpFont->getYSpacing ();
}
void EFont::setYSpacing (float fYSpacing){
   _lpFont->setYSpacing (fYSpacing);
}
void EFont::setSpaceAdjustment (float fKerning){
   _lpFont->setSpaceAdjustment (fKerning);
}
void EFont::drawMultiline (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning, float fFontHeight){
    ResetFont();
   _lpFont->drawMultiline (lpszText, dx1, dx2, dy, nStyle, fKerning, _fFontHeight);
}
long EFont::getMultilineWidth (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning, float fFontHeight){
   return _lpFont->getMultilineWidth (lpszText, dx1, dx2, dy, nStyle, fKerning, _fFontHeight);
}
long EFont::getMultilineHeight (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning, float fFontHeight){
   return _lpFont->getMultilineHeight (lpszText, dx1, dx2, dy, nStyle, fKerning, _fFontHeight);
}
void EFont::drawMultilineClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning, long nCursorPos, KTextFaceCursorCallBack lpCursorCallBack, void *lpCallBackData, float fFontHeight){
    ResetFont();
   _lpFont->drawMultilineClipped (lpszText, dx1, dy1, dx2, dy2, nStyle, nXOffset, nYOffset, fKerning, nCursorPos, lpCursorCallBack, lpCallBackData, _fFontHeight);
}
bool EFont::getMultilineClippedCharCoord (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning, long nCursorPos, long &nX1Char, long &nY1Char, long &nX2Char, long &nY2Char, float fFontHeight){
    return _lpFont->getMultilineClippedCharCoord (lpszText, dx1, dy1, dx2, dy2, nStyle, nXOffset, nYOffset, fKerning, nCursorPos, nX1Char, nY1Char, nX2Char, nY2Char, _fFontHeight);
}
long EFont::getMultilineClippedCharPos (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning, long nXChar, long nYChar, float fFontHeight){
    return _lpFont->getMultilineClippedCharPos (lpszText, dx1, dy1, dx2, dy2, nStyle, nXOffset, nYOffset, fKerning, nXChar, nYChar, _fFontHeight);
}
