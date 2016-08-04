#include "MyGame.h"
#include "KPTK.h"
#include "FontBank.h"
#include "ImageBank.h"
#include "MusicBank.h"
#include "ESceneDirector.h"

#include "UIOptions.h"

UIOptions::UIOptions(KUIElement *lpUIContainer, bool bWindowed, float fMusicVolume, float fSfxVolume) {
   _bOptionOpened = false;

   _bWindowed = bWindowed;
   _fMusicVolume = fMusicVolume;
   _fSfxVolume = fSfxVolume;

   lpFont = EFontBank::getFont("NITECLUB.TTF",35);
   lpFontButtons = EFontBank::getFont("NITECLUB.TTF",60);
   lpButtonGraphic = EImageBank::getImage("options_button.png");
   lpWindowGraphic = EImageBank::getImage("options_box.png");
     
   lpUIOptionsWindow = new KUIImage (lpUIContainer);
   lpUIOptionsWindow->setPosition (floorf ((800 - lpWindowGraphic->getWidth()) / 2), floorf ((600 - lpWindowGraphic->getHeight()) / 2));
   lpUIOptionsWindow->setSize (lpWindowGraphic->getWidth(), lpWindowGraphic->getHeight ());
   lpUIOptionsWindow->setGraphic (lpWindowGraphic, 0, 0, lpWindowGraphic->getWidth(), lpWindowGraphic->getHeight ());
   lpUIOptionsWindow->setBlend (0); /* Start out as hidden */
   lpUIOptionsWindow->setName("OptionsWindow");

   // Label
   lpButtonGraphic = EImageBank::getImage("options_button_white.png");
   KUIImage *lpUIBgText = new KUIImage(lpUIOptionsWindow);
   lpUIBgText->setSize(lpButtonGraphic->getWidth(),lpButtonGraphic->getHeight());
   lpUIBgText->setGraphic(lpButtonGraphic,0,0,lpUIBgText->getWidth(),lpUIBgText->getHeight());
   lpUIBgText->disableMessages(true);
   lpUIBgText->setPosition(264,24);

   KUIText *lpUIText = new KUIText (lpUIBgText);
   lpUIText->setSize (lpFont->getStringWidth (KStr ("UI_MUSICVOL"))+4, 55);
   lpUIText->setTextFont (lpFont);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setText (KStr ("UI_MUSICVOL"));
   lpUIText->disableMessages(true);
   lpUIText->setPosition(
      (lpUIBgText->getWidth() - lpUIText->getWidth())/2,
      (lpUIBgText->getHeight() -  lpUIText->getHeight())/2);

   // Label
   lpUIBgText = new KUIImage(lpUIOptionsWindow);
   lpUIBgText->setSize(lpButtonGraphic->getWidth(),lpButtonGraphic->getHeight());
   lpUIBgText->setGraphic(lpButtonGraphic,0,0,lpUIBgText->getWidth(),lpUIBgText->getHeight());
   lpUIBgText->disableMessages(true);
   lpUIBgText->setPosition(264,138);

   lpUIText = new KUIText (lpUIBgText);
   lpUIText->setSize (lpFont->getStringWidth (KStr ("UI_SFXVOL"))+4, 55);
   lpUIText->setTextFont (lpFont);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setText (KStr ("UI_SFXVOL"));
   lpUIBgText->disableMessages(true);
   lpUIText->setPosition(
      (lpUIBgText->getWidth() - lpUIText->getWidth())/2,
      (lpUIBgText->getHeight() -  lpUIText->getHeight())/2);

   // Label
   lpUIBgText = new KUIImage(lpUIOptionsWindow);
   lpUIBgText->setSize(lpButtonGraphic->getWidth(),lpButtonGraphic->getHeight());
   lpUIBgText->setGraphic(lpButtonGraphic,0,0,lpUIBgText->getWidth(),lpUIBgText->getHeight());
   lpUIBgText->disableMessages(true);
   lpUIBgText->setPosition(61,246);

   lpUIText = new KUIText (lpUIBgText);
   lpUIText->setSize ((float) lpFont->getMultilineWidth (KStr ("UI_FULLSCREEN"), 0, 800, 0, TEXTSTYLE_LEFT)+4 + 40, 55);
   lpUIText->setTextFont (lpFont, (float) lpFont->getHeightPix() + 4);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setText (KStr ("UI_FULLSCREEN"));
   lpUIText->setTextFlow (K_UITEXT_LEFT);
   lpUIText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   lpUIBgText->disableMessages(true);
   lpUIText->setPosition(
      (lpUIBgText->getWidth() - lpUIText->getWidth())/2,
      (lpUIBgText->getHeight() -  lpUIText->getHeight())/2);

   // Label 
   lpUIBgText = new KUIImage(lpUIOptionsWindow);
   lpUIBgText->setSize(lpButtonGraphic->getWidth(),lpButtonGraphic->getHeight());
   lpUIBgText->setGraphic(lpButtonGraphic,0,0,lpUIBgText->getWidth(),lpUIBgText->getHeight());
   lpUIBgText->disableMessages(true);
   lpUIBgText->setPosition(356,246);

   lpUIText = new KUIText (lpUIBgText);
   lpUIText->setSize ((float) lpFont->getMultilineWidth (KStr ("UI_WINDOWED"), 0, 800, 0, TEXTSTYLE_LEFT)+4, 55);
   lpUIText->setTextFont (lpFont, (float) lpFont->getHeightPix() + 4);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setTextFlow (K_UITEXT_LEFT);
   lpUIText->setText (KStr ("UI_WINDOWED"));
   lpUIText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   lpUIBgText->disableMessages(true);
   lpUIText->setPosition(
      (lpUIBgText->getWidth() - lpUIText->getWidth())/2,
      (lpUIBgText->getHeight() -  lpUIText->getHeight())/2);

   // Label 
   lpUIBgText = new KUIImage(lpUIOptionsWindow);
   lpUIBgText->setSize(lpButtonGraphic->getWidth(),lpButtonGraphic->getHeight());
   lpUIBgText->setGraphic(lpButtonGraphic,0,0,lpUIBgText->getWidth(),lpUIBgText->getHeight());
   lpUIBgText->disableMessages(true);
   lpUIBgText->setPosition(204,320);

   lpUIText = new KUIText (lpUIBgText);
   lpUIText->setSize ((float) lpFont->getMultilineWidth (KStr ("UI_SYSTEMCURSOR"), 0, 800, 0, TEXTSTYLE_LEFT)+4, 55);
   lpUIText->setTextFont (lpFont, (float) lpFont->getHeightPix() + 4);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setTextFlow (K_UITEXT_LEFT);
   lpUIText->setText (KStr ("UI_SYSTEMCURSOR"));
   lpUIText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   lpUIBgText->disableMessages(true);
   lpUIText->setPosition(
      (lpUIBgText->getWidth() - lpUIText->getWidth())/2,
      (lpUIBgText->getHeight() -  lpUIText->getHeight())/2);


   KUILayout *lpUIOptionsLayout = new KUILayout(lpUIOptionsWindow);
   lpUIOptionsLayout->setPosition (0, 70);
   lpUIOptionsLayout->setSize (lpUIOptionsWindow->getWidth(), lpUIOptionsWindow->getHeight () - 70 - 76);
   lpUIOptionsLayout->setLayoutType (K_UILAYOUT_VERTICAL);
   lpUIOptionsLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   lpUIOptionsLayout->setVSpacing (0);
   
   KUILayout *lpUILayout = new KUILayout (lpUIOptionsLayout);
   lpUILayout->setSize (lpUIOptionsWindow->getWidth(), (float) lpFont->getHeightPix() + 2 + 60);
   lpUILayout->setLayoutType (K_UILAYOUT_VERTICAL);
   lpUILayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   
   lpButtonGraphic = EImageBank::getImage("options_slider.png");
   lpUIMusicSlider = new KUISlider (lpUIOptionsWindow);
   lpUIMusicSlider->setSize (377, 59);
   lpUIMusicSlider->setActiveRect (true, 5, 0, 376, 59);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 0, 0, 377, 59);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 0, 377, 59);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 0, 377, 59);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 0, 377, 59);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 377, 0, 437, 59);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 377, 0, 437, 59);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 377, 0, 437, 59);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 377, 0, 437, 59);
   lpUIMusicSlider->setValue (_fMusicVolume);
   (void) lpUIMusicSlider->hasValueChanged ();
   lpUIMusicSlider->setPosition(244,112);
   lpUIMusicSlider->setName ("music_slider");   /* Just for the message handler to know not to play a sound when clicked */
   
   
   lpUISfxSlider = new KUISlider (lpUIOptionsWindow);
   lpUISfxSlider->copyFromTemplate (lpUIMusicSlider);
   lpUISfxSlider->setValue (_fSfxVolume);
   lpUISfxSlider->hasValueChanged ();
   lpUISfxSlider->setPosition(244,226);
   lpUISfxSlider->setName("sound_slider");

   lpButtonGraphic = EImageBank::getImage("options_checkbox.png");
   lpUISelector = new KUISelector (lpUIOptionsWindow);
   lpUISelector->setLayoutType (K_UILAYOUT_HORIZONTAL);
   lpUISelector->setHorizAlignMode(K_UIELEMENT_HCENTER);
   lpUISelector->setVertAlignMode(K_UIELEMENT_VCENTER);
   lpUISelector->setPosition(342,287);
   lpUISelector->setSize(360,60);
   lpUISelector->setName("option_selector");

   lpUIFsButton = new KUIButton (lpUISelector);
   lpUIFsButton->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 0, 0, 60, 55);
   lpUIFsButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 0, 60, 55);
   lpUIFsButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 0, 60, 55);
   lpUIFsButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 55, 60, 110);
   lpUIFsButton->setBackgroundGraphic (K_UIELEMENT_SELECTED, lpButtonGraphic, 0, 55, 60, 110);
   lpUIFsButton->setSize (65, 55);
   lpUIFsButton->setName("option_full_screen");
   
   lpUIWindowedButton = new KUIButton (lpUISelector);
   lpUIWindowedButton->copyFromTemplate (lpUIFsButton);
   lpUIWindowedButton->setSize (65, 55);
   lpUIWindowedButton->setName("option_windowed_screen");

   lpUICursorButton = new KUIButton (lpUIOptionsWindow);
   lpUICursorButton->copyFromTemplate (lpUIFsButton);
   lpUICursorButton->setSize (65, 55);
   lpUICursorButton->setName("option_cursor_screen");
   lpUICursorButton->setPosition(493,358);
   
   lpButtonGraphic = EImageBank::getImage("options_button.png");
   lpUIOkButton = new KUIButton (lpUIOptionsWindow);
   lpUIOkButton->setSize (260, 69);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 0, 0, 260, 69);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 0, 260, 69);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 0, 260, 69);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 0, 260, 69);
   lpUIOkButton->setTextFont (lpFontButtons);
   lpUIOkButton->setTextOutlineThickness (2);
   lpUIOkButton->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIOkButton->setText (KStr ("UI_OK"));
   lpUIOkButton->setName("OkButton");
   lpUIOkButton->setPosition(298,429);

   if (_bWindowed)
      lpUISelector->setSelectedElement (lpUIWindowedButton);
   else
      lpUISelector->setSelectedElement (lpUIFsButton);
   lpUISelector->hasSelectionChanged ();

   lpButtonGraphic = EImageBank::getImage("options_checkbox.png");
   if (SCENEDIRECTOR->_bSystemCursor) {
      lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 55, 60, 110); 
      lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 55, 60, 110);
      lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 0, 60, 55);
   }
}
UIOptions::~UIOptions() {
   XDELETE(lpFont);
   XDELETE(lpFontButtons);
}
void UIOptions::open() {
   if (IsVisible() || KUIElement::getElementByName("playerlist")->getBlend() > 0) {
      SCENEDIRECTOR->_bOptionsRequest = false;
      return;
   }
   SCENEDIRECTOR->_bOptionsRequest = false;
   _bOptionOpened = true;
   lpUIOptionsWindow->animateScale (0.1f, 1.0f, 0, 250, K_COUNTER_EASEOUT);
   lpUIOptionsWindow->animateZAngle (20, 0, 0, 250, K_COUNTER_EASEOUT);
   lpUIOptionsWindow->animatePosition (420, 320, floorf ((1024 - lpWindowGraphic->getWidth()) / 2), floorf ((768 - lpWindowGraphic->getHeight()) / 2), 0, 250, K_COUNTER_EASEOUT);
   lpUIOptionsWindow->animateBlend (0, 1, 0, 250);
}
void UIOptions::close() {
   if (!_bOptionOpened)
      return;
   lpUIOptionsWindow->animateBlend (1, 0, 0, 250);
   lpUIOptionsWindow->animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
   lpUIOptionsWindow->animatePosition (floorf ((1024 - lpWindowGraphic->getWidth()) / 2), floorf ((768 - lpWindowGraphic->getHeight()) / 2), 420, 320, 0, 250, K_COUNTER_EASEIN);
   lpUIOptionsWindow->animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);
   _bOptionOpened = false;
   SCENEDIRECTOR->_bOptionsRequest = false;
   ((MyGame *)MyGame::getGame())->saveSettings();
}

bool UIOptions::IsVisible() {
   return _bOptionOpened || lpUIOptionsWindow->getBlend() > 0;
}
void UIOptions::Check() {
   if (SCENEDIRECTOR->_bOptionsRequest) {
      open();
   }
   if (lpUIOkButton->isClicked ()) {
      close(); 
   }
   if (!IsVisible())
      return;

   if (lpUICursorButton->isClicked()) {
      if (SCENEDIRECTOR->_bSystemCursor) {
         SCENEDIRECTOR->SetSystemCursor(false);
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 0, 60, 110); 
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 0, 60, 55);
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 55, 60, 110);
      } else {
         SCENEDIRECTOR->SetSystemCursor(true);
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 55, 60, 110); 
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 55, 60, 110);
         lpUICursorButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 0, 60, 55);
      }
   }
   if (lpUIMusicSlider->hasValueChanged () || lpUISfxSlider->hasValueChanged ()) {
      _fMusicVolume = lpUIMusicSlider->getValue ();
      _fSfxVolume = lpUISfxSlider->getValue ();
      KSound::setGlobalVolumes ((long) (100 * _fMusicVolume), (long) (100 * _fSfxVolume));
      SCENEDIRECTOR->SetVolume(_fMusicVolume*100);
   }
   if (lpUISelector->hasSelectionChanged ()) {
      if (lpUISelector->getSelectedElement() == lpUIFsButton && _bWindowed) {
         MyGame::getGame()->getKWindow()->toggleFullScreen (true);
         _bWindowed = false;
      }
      if (lpUISelector->getSelectedElement() == lpUIWindowedButton && !_bWindowed) {
         if (MyGame::getGame()->getKWindow()->getDesktopWidth() >= 1024 && MyGame::getGame()->getKWindow()->getDesktopHeight() >= 768) {
            MyGame::getGame()->getKWindow()->toggleFullScreen (false);
            _bWindowed = true;
         }
         else {
            SCENEDIRECTOR->OpenDialogbox("Unsupported desktop resolution");
         }
      }
   }
}

