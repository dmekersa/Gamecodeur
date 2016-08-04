#ifndef EMINIJEUVIDEODISPLAY_H
#define EMINIJEUVIDEODISPLAY_H

#include <list>
#include "global.h"
#include "KPTK.h"
#include "EMiniJeuHelper.h"
#include "eminijeubase.h"
#include "KVideo.h"

#include "CreepingSmoke.h"


class EMiniJeuVideoDisplay: public EMiniJeuBase {
public:
	EMiniJeuVideoDisplay (void);
	virtual ~EMiniJeuVideoDisplay (void);
	
	static std::vector< ESmartImage * > _vVideo;
   
   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();
	
	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset() {;}
	virtual void GameSetHint(int nHintLevel);

	CreepingSmokeEmitter *_lpEmitter;


	int _nVideo;
	bool pause;
	bool _bEmit;
};
#endif
