/*
 *  EnergyParticles.h
 *  enigma
 *
 *  Created by Rockford on 14/01/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef ENERGYPARTICLE_H
#define ENERGYPARTICLE_H

#include "CreepingSmoke.h"
#include "global.h"

class EnergyParticle : public KObjectListable {
public:
   EnergyParticle(float fX, float fY, float fGoalX, float fGoalY, float fDuration, int nGroupNumber);
   ~EnergyParticle()
   {
      XDELETE(_lpEmitter);
      XDELETE(_lpAnimX);
      XDELETE(_lpAnimY);
   }
   CreepingSmokeEmitter *_lpEmitter;
   int _nGroup;
   KCounter *_lpAnimX;
   KCounter *_lpAnimY;
   
   void Render();
};

class EnergyParticlesManager {
public:
   EnergyParticlesManager();
   ~EnergyParticlesManager();
   
   // Liste
   static KList<EnergyParticle> g_ListEnergy;
   
   void AddEnergy(float fX, float fY, float fGoalX, float fGoalY, float fDuration, int nGroupNumber);
   
   void Logic();
   void Render(); // Penser à cleaner les emetteurs vides
   bool isCompleted(int nGroupNumber);
   
   // Destructeur de chaque élement
   static void CleanUp() {
      // Parcours la liste pour en récupérer les valeurs et la vide
      EnergyParticle *o;
      EnergyParticle *otodelete;
      bool bGotNext = g_ListEnergy.getHead (o);
      if (bGotNext) {
         while (bGotNext) {
            g_ListEnergy.remove(o);
            otodelete = o;
            bGotNext = g_ListEnergy.getNext (o);
            delete otodelete;
         }
      }
   }
   
private:
   KBatch *_lpBatch;
};

#endif