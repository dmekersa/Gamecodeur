#include "assert.h"
#include "KRandom.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"

#include "EPointer.h"
#include "EGeometry.h"
#include "KVideo.h"
#include "ESmartImage.h"

#include "EMiniJeuVideoDisplay.h"

std::vector< ESmartImage * > EMiniJeuVideoDisplay::_vVideo;
namespace {
bool LoadDirectoryVideos(char *filename, bool isFolder)
{
	if (isFolder)
		return true;

	std::string sfilename = filename;
	if (sfilename.find(".ogv") > sfilename.length())
		return true;

	if (sfilename.find("_mask.ogv") < sfilename.length())
		return true;

	if (sfilename.find("_Q1.ogv") < sfilename.length())
		return true;

	if (sfilename.find("_Q2.ogv") < sfilename.length())
		return true;

	KVideo *video = EImageBank::getVideo(filename,true);
	video->readFrame();
	ESmartImage *simg = new ESmartImage(video,0,0,true,true,1000);
	simg->setFixedBlend(1);
	simg->SetVisible(true,true);
	simg->StartVideo();
	EMiniJeuVideoDisplay::_vVideo.push_back(simg);

	return true;
}
}
EMiniJeuVideoDisplay::EMiniJeuVideoDisplay(void)
{
	SetTitle("Connect the colored wires");
	SetRule("Connect the 3 pairs of colored wires by placing the appropriate tiles on the grid. Right clic to rotate a tile.\n\nClick on HELP to lower the difficulty.");
	SetGameType(MINIJEU_DIRECTGAME);
	SetBackGround(EImageBank::getImage("mg_poltergeist_bg.jpg"));
	SetGameNumber(40);

	for (int i=0; i<_vVideo.size(); i++) {
		XDELETE(_vVideo[i]);
	}
	_vVideo.clear();
	KMiscTools::enumerateFolder(KMiscTools::makeFilePath("videos"), (enumProc) LoadDirectoryVideos , NULL  );
	
	_nVideo = -2;
	pause = false;
	Reset();

	_lpEmitter = new CreepingSmokeEmitter();
	_lpEmitter->SetWindow(MyGame::getGame()->getKWindow());
	_lpEmitter->ReadPreset("particles/pirateflame.cse");
	_lpEmitter->ReadAnimation("particles/livingroom_yumanco_flame.csa");
	_lpEmitter->Stop();

	_bEmit = false;
}

EMiniJeuVideoDisplay::~EMiniJeuVideoDisplay(void)
{
	for (int i=0; i<_vVideo.size(); i++) {
		XDELETE(_vVideo[i]);
	}
	_vVideo.clear();
	XDELETE(_lpEmitter);
}

/** 3 steps rendering **/
void EMiniJeuVideoDisplay::GameCheck ()
{

	if (!Mouse.LeftEvent())
		return;

	ESmartImage *tmp;

	if (_nVideo == -1) {
		for (int i=0; i< _vVideo.size(); i++) {
			_vVideo[i]->StopVideo();
		}
	}
	_nVideo++;
	if (_vVideo.size() == 0)
		return;

	_nVideo = _nVideo % _vVideo.size();
	
	tmp = _vVideo[_nVideo];
	tmp->ResumeVideo();
	tmp->getVideo()->rewind();
	tmp->getVideo()->readFrame();
	tmp->Move((_nW - tmp->getWidth())/2,(_nH - tmp->getHeight())/2);	
}

void EMiniJeuVideoDisplay::GameLogic ()
{	
	if (_nVideo < 0 || _vVideo.size() == 0)
		return;

	_vVideo[_nVideo]->Logic();
	if (KInput::isPressed(K_VK_E))
	{
		if (_bEmit) {
			_lpEmitter->Stop();
		} else {
			_lpEmitter->Start();		
		}
		_bEmit = ! _bEmit;
	}

}

void EMiniJeuVideoDisplay::GameDrawSet ()
{
	if (_vVideo.size() == 0)
		return;

if (_nVideo < 0) {
		for (int i=0; i<_vVideo.size(); i++) {
			_vVideo[i]->Draw();
		}
		_nVideo++;
		if (_nVideo == 0) {
			for (int i=0; i<_vVideo.size(); i++) {
				_vVideo[i]->StopVideo();
			}
			ESmartImage *tmp;
			tmp = _vVideo[_nVideo];
			tmp->ResumeVideo();
			tmp->getVideo()->rewind();
			tmp->getVideo()->readFrame();
			tmp->Move((_nW - tmp->getWidth())/2,(_nH - tmp->getHeight())/2);	
		}
		return;
	}

	if (_bEmit) {
		_lpEmitter->Render();
	}
	_vVideo[_nVideo]->Draw();
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuVideoDisplay::GameIsResolve()
{
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuVideoDisplay::GameResolve()
{
}

// Active un niveau d'indices (_nGameStatus = 1, 2 ou 3)
void EMiniJeuVideoDisplay::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	case MINIJEU_HINT1:
		Reset();
		break;
	case MINIJEU_HINT2:
		Reset();
		break;
	case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}

// Preload images et sons
void EMiniJeuVideoDisplay::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_poltergeist_bg.jpg","",true,true);

}
