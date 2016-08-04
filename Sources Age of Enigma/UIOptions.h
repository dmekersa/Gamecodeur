#ifndef UIOPTIONS_H
#define UIOPTIONS_H
#include "KPTK.h"

class EFont;

class UIOptions {
public:
   UIOptions(KUIElement *, bool bWindowed, float fMusicVolume, float fSfxVolume);
   ~UIOptions();

   void open();
   void close();

   void Check();
   bool IsVisible();

   // 17/08/11 Passe en public pour pouvoir propager le changement de mode dans myGame
   bool _bWindowed;
   float _fMusicVolume;
   float _fSfxVolume;
   
private:
   bool _bOptionOpened;   

   KUIImage    *lpUIOptionsWindow;
   KUIButton   *lpUIOkButton;
   KUISlider   *lpUIMusicSlider;
   KUISlider   *lpUISfxSlider;
   KUISelector *lpUISelector;
   KUIButton   *lpUIFsButton;
   KUIButton   *lpUIWindowedButton;
   KUIButton   *lpUICursorButton;

   KGraphic *lpButtonGraphic;
   KGraphic *lpWindowGraphic;

   EFont *lpFont;
   EFont *lpFontButtons;
};
#endif
