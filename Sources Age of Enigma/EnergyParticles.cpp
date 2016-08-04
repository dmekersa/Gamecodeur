/*
 *  EnergyParticles.cpp
 *  enigma
 *
 *  Created by Rockford on 14/01/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EnergyParticles.h"
#include "ImageBank.h"

EnergyParticle::EnergyParticle(float fX, float fY, float fGoalX, float fGoalY, float fDuration, int nGroupNumber)
{
   _lpEmitter = new CreepingSmokeEmitter();
   _lpEmitter->SetWindow(MyGame::getGame()->getKWindow());
   _lpEmitter->ReadPreset(KMiscTools::makeFilePath("particles/pirateflame.cse"));
   _lpEmitter->Move(fX, fY);
   _lpEmitter->_nTrailChilds = 0;
   
   _lpAnimX = new KCounter();
   _lpAnimY = new KCounter();
   
   _lpAnimX->startCounter(fX, fGoalX, 0, fDuration, K_COUNTER_EASEIN);
   _lpAnimY->startCounter(fY, fGoalY, 0, fDuration, K_COUNTER_EASEIN);
   
   _nGroup = nGroupNumber;
   _lpEmitter->Start();
}

void EnergyParticle::Render()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

   _lpAnimX->move(fElapsed);
   _lpAnimY->move(fElapsed);
   
   _lpEmitter->Move(_lpAnimX->getCurrentValue(), _lpAnimY->getCurrentValue());
   
   _lpEmitter->Render();
}

EnergyParticlesManager::EnergyParticlesManager()
{
   _lpBatch = KPTK::createKBatch ();
   _lpBatch->allocateBuffer(500);
   
}

EnergyParticlesManager::~EnergyParticlesManager()
{
   XDELETE(_lpBatch);
}

void EnergyParticlesManager::AddEnergy(float fX, float fY, float fGoalX, float fGoalY, float fDuration, int nGroupNumber)
{
   EnergyParticle *lpE = new EnergyParticle(fX, fY, fGoalX, fGoalY, fDuration, nGroupNumber);
   g_ListEnergy.addToTail(lpE);
}

void EnergyParticlesManager::Logic()
{
	EnergyParticle *pe;
	bool bGotNext;
   
	bGotNext = g_ListEnergy.getHead (pe);
	while (bGotNext) {
      
      pe->_lpEmitter->Logic();
      
		bGotNext = g_ListEnergy.getNext (pe);
	}
}

void EnergyParticlesManager::Render()
{
	EnergyParticle *pe;
   EnergyParticle *oToDelete = NULL;
	bool bGotNext;
   
	bGotNext = g_ListEnergy.getHead (pe);
	while (bGotNext) {
   
      // Déplacement
      pe->Render();
      
      // Fini ?
      if (pe->_lpAnimX->isCompleted() && pe->_lpAnimY->isCompleted()) {
         // Stoppe l'emetteur à son arrivée
         if (pe->_lpEmitter->_bStarted) {
            pe->_lpEmitter->Stop();
         }
         else {
            // Supprime l'emetteur si il n'y a plus de particules
            if (pe->_lpEmitter->_particles.getCount() == 0) {
               g_ListEnergy.remove(pe);
               oToDelete = pe;
            }
         }

      }    
		bGotNext = g_ListEnergy.getNext (pe);
      if (oToDelete) {
         XDELETE(oToDelete);
      }
	}
}

bool EnergyParticlesManager::isCompleted(int nGroupNumber)
{
	EnergyParticle *pe;
	bool bGotNext;
   
	bGotNext = g_ListEnergy.getHead (pe);
	while (bGotNext) {
      if (pe->_nGroup = nGroupNumber) {
         if (!pe->_lpAnimX->isCompleted() || !pe->_lpAnimY->isCompleted()) {
            return false;
         }
      }
		bGotNext = g_ListEnergy.getNext (pe);
	}
   
   return true;
}

KList<EnergyParticle> EnergyParticlesManager::g_ListEnergy;
