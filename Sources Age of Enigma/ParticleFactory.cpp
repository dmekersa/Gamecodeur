#include "ParticleFactory.h"
#include "ImageBank.h"
#include "global.h"

/*********************************************/
/********** ParticleEmitter section **********/
/*********************************************/
ParticleEmitter::ParticleEmitter(void){
   fMaxEmitter = 0; /* to be adapted to the length of segments/polygon : longer => higher */
	fProgressPerFrame = 0;
	fDisplayEveryLogic = 0;
	lpProgress = NULL;
	lpBatch = NULL;
}
ParticleEmitter::ParticleEmitter(float x, float y){
	fX = x;
	fY = y;

	bStart = false;
	bKilled = true;

	fDisplayEveryLogic = 5;

	fMaxEmitter = 80; /* to be adapted to the length of segments/polygon : longer => higher */
   lpProgress = new KCounter ();

	/* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (128);
}
ParticleEmitter::~ParticleEmitter(void){
   VfxParticle *o;
   VfxParticle *otodelete;
   bool bGotNext = g_lParticles.getHead(o);
   while (bGotNext) {
      otodelete = o;
      bGotNext = g_lParticles.getNext(o);
      g_lParticles.remove(otodelete);
      XDELETE(otodelete);
   }

   Segment *os;
   Segment *ostodelete;
   bGotNext = Polygon.getHead(os);
   while (bGotNext) {
      ostodelete = os;
      bGotNext = Polygon.getNext(os);
      Polygon.remove(ostodelete);
      XDELETE(ostodelete);
   }   

   //	delete lpParticleGraphic;
	delete lpProgress;
	delete lpBatch;
}
void ParticleEmitter::SetEmitRate(long x){
	lRate = x;
}

void ParticleEmitter::SetParticleAge(float x){
	fAge = x;
}

void ParticleEmitter::SetHeightEffect(long x){
	this->SetParticleAge( (float)x/100);
}

void ParticleEmitter::SetPosition(float x, float y) {
	fX = x;
	fY = y;
}

void ParticleEmitter::Move(float x, float y) {
	bool bGotNextSegment;
	Segment *s;

	fX = x; // move emitter source
	fY = y;
	// TODO not sure if this calculation is relevant
	if (bGotNextSegment = Polygon.getHead (s)) {
		while (bGotNextSegment) { // move emitter segments if any
			s->fX1 += x; 
			s->fX2 += x;
			s->fY1 += y;
			s->fY2 += y;
			bGotNextSegment = Polygon.getNext (s);
		}
	}
}

bool ParticleEmitter::SetParticleBitmap(const char *lpszBitmapName){

   // Load particle
   lpParticleGraphic = EImageBank::getImage(lpszBitmapName);

   return lpParticleGraphic != NULL;
 }

KGraphic * ParticleEmitter::GetParticleBitmap(void){
	return lpParticleGraphic;
}

void ParticleEmitter::AddSegment(float x1, float y1, float x2, float y2, float increment) {
	Segment *s = new Segment (x1, y1, x2, y2, increment);
	Polygon.addToTail (s);
}

void ParticleEmitter::AddSegment(Segment *s) {
	Polygon.addToTail (s);
}
void ParticleEmitter::Render(double fElapsed) {
		bool bGotNextParticle;
		VfxParticle *p;
		float w,h;

		// Count for progress
	    lpProgress->move(fElapsed);
	    fProgressPerFrame = lpProgress->getCurrentValue(); /* current number of 2000ms increments */

		this->Init();

		if (!IsKilled()) {
			lpBatch->beginBatch (this->GetParticleBitmap());
         
			w = this->GetParticleBitmap()->getWidth();
			h = this->GetParticleBitmap()->getHeight();

			if ((lTypeOfEffect != VFX_FOCUS) && (lTypeOfEffect != VFX_FLAKE))
				this->GetParticleBitmap()->setAlphaMode (0);  // Additive alpha (for fade out) except for VFX_FOCUS particle

			bGotNextParticle = g_lParticles.getHead (p);
			while (bGotNextParticle) {
				//this->GetParticleBitmap()->setBlitColor (p->fColor[0], p->fColor[1], p->fColor[2], p->fColor[3]); 
            float fAngle = 0.0f;
            if (lTypeOfEffect == VFX_FOCUS) {
               if (p->fAge > 1)
               {
                  fAngle = 360.0f * p->fColor[3];
               }
               else {
                  fAngle = 360.0f * (1.0f-p->fColor[3]);
               }
            }
				lpBatch->blitAlphaRectFx(0, 0, w, h, p->x-w/2, p->y-h/2, fAngle, p->fScale, p->fColor[3]);
				bGotNextParticle = g_lParticles.getNext (p);
			}
			lpBatch->endBatch ();
		}
}

void ParticleEmitter::Kill(void){
	bool bGotNext;
	VfxParticle *p;
		 
	bKilled = true;
	bStart = false;

	// clean list of remaining alive particles, if any
	bGotNext = g_lParticles.getHead (p);
	while (bGotNext) {
		VfxParticle *lpNextParticle = p;
            
		bGotNext = g_lParticles.getNext (lpNextParticle);
        // remove and clean this particle
		g_lParticles.remove (p);
    	delete p;
		// try with next one
		p = lpNextParticle;
		}
}

void ParticleEmitter::Stop(void){
	bStart = false;
}

void ParticleEmitter::Start(void){
	float fSpeed; // influes on progression speed : value higher => speed slower 
	bStart = true;
	bKilled = false;

   fProgressPerFrame = 1;
	if (lTypeOfEffect == VFX_FIRE)
		fSpeed = 2000; // fire progression must be faster
	else
		fSpeed = 4000;
   if (lTypeOfEffect == VFX_FOCUS) {
      fMaxEmitter = GetRandom(1000.0f, 15000.0f);
   }

   lpProgress->startCounter(0, fMaxEmitter, 0, fSpeed, K_COUNTER_EASEIN);   
}

bool ParticleEmitter::IsKilled(void){
	return bKilled;
}
bool ParticleEmitter::IsStarted(void){
	return bStart;
}

void ParticleEmitter::Init() {
	long i;
	bool bGotNextSegment, bGotNextVector2;
	Segment *s;
	Vertice *CurrentVector2;

	if (IsStarted()){
		switch (lTypeOfEffect) {
			case VFX_TRAIL:
				for (i = 0; i < lRate; i++) {
					VfxParticle *p = new VfxParticle;
					p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 30; // instead of *3
					p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 30;
					p->vx = (KRandom::getRandomFloat() - 0.5f) ; // instead of *4, to slow down 
					p->vy = (KRandom::getRandomFloat() - 0.5f) ; 
					p->fScale = GetRandom(1.0f, 2.0f);
					p->fColor[0] = 1.0f; //0.9f + KRandom::getRandomFloat() * 0.25f;;  /* Red */
					p->fColor[1] = 1.0f; //0.9f + KRandom::getRandomFloat() * 0.25f;  // green instead of 0.5 
					p->fColor[2] = 1.0f; //0.9f + KRandom::getRandomFloat() * 0.25f;  // blue instead of 0.15
					p->fColor[3] = 0.75f + KRandom::getRandomFloat() * 0.25f;  /* alpha transparency */ 
					p->fAge = 0;
					p->fAgeIncr = 0.005f + KRandom::getRandomFloat() * 0.01f; // instead of 0.01 : looks OK

					g_lParticles.addToTail (p);
            }
				break;

			/* fire effect */ 
			case VFX_FIRE :
				/* check if there are segments */ 
				if (bGotNextSegment = Polygon.getHead (s)) {

					long lCumulatedIndex = 0; /* addition of all points so far */

					while(bGotNextSegment && (lCumulatedIndex < fProgressPerFrame)){

						/* for current segment, follow the sampled points aka Vector2 of it */
						bGotNextVector2 = s->SampledSegment.getHead(CurrentVector2);
					
						while (bGotNextVector2 && (lCumulatedIndex < fProgressPerFrame)) { 
						
							this->SetPosition (CurrentVector2->Getx(), CurrentVector2->Gety()); /* set new emitter position */

							for (i = 0; i < lRate; i++) {
								VfxParticle *p = new VfxParticle;
								p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 4.0f;
								p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 2.0f;

								p->vx = (KRandom::getRandomFloat() - 0.5f) * 0.1f;
								p->vy = (KRandom::getRandomFloat() - 1.0f) * 0.5f; 

								p->fScale = KRandom::getRandomFloat() * 0.35f;

								p->fColor[0] = 1.0f;  /* Red */
								p->fColor[1] = 0.3f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
								p->fColor[2] = KRandom::getRandomFloat() * 0.1f;  /* Blue */
								p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* alpha transparency */ 
							
								p->fAge = 0;
								p->fAgeIncr = 0.005f + KRandom::getRandomFloat() * 0.001f;

								g_lParticles.addToTail (p);
							} /* end for */

							lCumulatedIndex++;
							bGotNextVector2 = s->SampledSegment.getNext(CurrentVector2); /* get next sample position, if any */

						} /* end while NextVector2 */ 

						bGotNextSegment = Polygon.getNext (s); /* get next segment, if any */ 
					} /* end while NextSegment */
				
				}  	/* end if Polygon */
				else {
					/* no segment: only one point */
					for (i = 0; i < lRate; i++) {
						VfxParticle *p = new VfxParticle;
		
						p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 3;
						p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 3;
						p->vx = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
						p->vy = (KRandom::getRandomFloat() - 0.5f) * 4.0f; 
						p->fScale = KRandom::getRandomFloat() + 0.1f;
						p->fColor[0] = 1;  /* Red */
						p->fColor[1] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
						p->fColor[2] = 0.15f + KRandom::getRandomFloat() * 0.1f;  /* Blue */
						p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* alpha transparency */ 
						p->fAge = 0;
						p->fAgeIncr = 0.01f + KRandom::getRandomFloat() * 0.1f;
						g_lParticles.addToTail (p);
					}
				}
				break;
		/* firework / spray with gravity : first raise then fall */
		case VFX_SPRAY:
			if ((KRandom::getRandomFloat() - 0.96f) > 0) { // emit randomly
				for (i = 0; i < lRate; i++) {
					VfxParticle *p = new VfxParticle;
				
					p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 5;
					p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 3;
					p->vx = (KRandom::getRandomFloat() - 0.5f) *7.0f; // instead of *4
					if (p->y > fY){ // particle already below emitter
						p->fAgeIncr = 0.07f + KRandom::getRandomFloat() * 0.01f;
						p->vy = KRandom::getRandomFloat(); // do not go above
					}
					else { // particle above emitter
						p->vy = KRandom::getRandomFloat() * -4.0f; // to go up first 
						p->fAgeIncr = 0.03f + KRandom::getRandomFloat() * 0.01f; // longer life
					}
					p->fScale = KRandom::getRandomFloat() * 0.4f + 0.1f;
					p->fColor[0] = 0.9f;  /* Red */
					p->fColor[1] = 0.9f;// + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
					p->fColor[2] = 0.9f;// + KRandom::getRandomFloat() * 0.1f;  /* Blue */
					p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  // alpha transparency 
					p->fAge = 0;
					
					g_lParticles.addToTail (p);
					}
				}
			break;

		/* particles will raise only */ 
		case VFX_SMOKE :
			for (i = 0; i < lRate; i++) {
			    VfxParticle *p = new VfxParticle;

				p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 3;
	            p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 3;
		        p->vx = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
			    p->vy = (KRandom::getRandomFloat() - 0.5f) * 4.0f; 
				p->fScale = KRandom::getRandomFloat() * 0.4f + 0.25f;
	            p->fColor[0] = 1;  /* Red */
				p->fColor[1] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
			    p->fColor[2] = 0.15f + KRandom::getRandomFloat() * 0.1f;  /* Blue */
				p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* alpha tranpsarency */ 
	            p->fAge = 0;
		        p->fAgeIncr = 0.01f + KRandom::getRandomFloat() * 0.01f;

			    g_lParticles.addToTail (p);
			}
			break;
		/* initial algo from sample : explosion in any direction */
		case VFX_EXPLOSION :
			for (i = 0; i < lRate; i++) {
				VfxParticle *p = new VfxParticle;
				p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 3;
				p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 3;
				p->vx = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
				p->vy = (KRandom::getRandomFloat() - 0.5f) * 4.0f; 
				p->fScale = KRandom::getRandomFloat() * 0.4f + 0.25f;
				p->fColor[0] = 1;  /* Red */
				p->fColor[1] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
				p->fColor[2] = 0.15f + KRandom::getRandomFloat() * 0.1f;  /* Blue */
				p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* alpha tranpsarency */ 
				p->fAge = 0;
				p->fAgeIncr = 0.01f + KRandom::getRandomFloat() * 0.01f;

				g_lParticles.addToTail (p);
			}
			break;
		/* fire candle effect following a polygon */ 
		case VFX_MOVING :
			/* check if there are segments */ 
			if (bGotNextSegment = Polygon.getHead (s)) {

				long lCumulatedIndex = 0; /* addition of all points so far */

				while(bGotNextSegment && (lCumulatedIndex < fProgressPerFrame)){

					/* for current segment, follow the sampled points aka Vector2 of it */
					bGotNextVector2 = s->SampledSegment.getHead(CurrentVector2);
					/* find the right sampled point matching the current fProgressPerFrame value */
					while (bGotNextVector2 && (lCumulatedIndex < fProgressPerFrame)) { 
						lCumulatedIndex++;
						bGotNextVector2 = s->SampledSegment.getNext(CurrentVector2); /* get next sample position, if any */
					} /* end while NextVector2 */ 

					bGotNextSegment = Polygon.getNext (s); /* get next segment, if any */ 
				} /* end while NextSegment */

				this->SetPosition (CurrentVector2->Getx(), CurrentVector2->Gety()); /* set new emitter position */

				for (i = 0; i < lRate; i++) {
					VfxParticle *p = new VfxParticle;
					p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 3;
					p->y = fY + (KRandom::getRandomFloat() - 0.5f) * 3;
					p->vx = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
					p->vy = (KRandom::getRandomFloat() - 0.5f) * 4.0f; 
					p->fScale = KRandom::getRandomFloat() + 0.1f;
					p->fColor[0] = 1;  /* Red */
					p->fColor[1] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
					p->fColor[2] = 0.15f + KRandom::getRandomFloat() * 0.1f;  /* Blue */
					p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;  /* alpha transparency */ 
					p->fAge = 0;
					p->fAgeIncr = 0.01f + KRandom::getRandomFloat() * 0.1f;
					g_lParticles.addToTail (p);
				} /* end for */
			}
			break;
		/* effect of slowly raising particles */ 
		case VFX_FLAKE :
			/* check if there are segments */ 
			if (bGotNextSegment = Polygon.getHead (s)) {
				while(bGotNextSegment){
					/* for current segment, follow the sampled points aka Vector2 of it */
					bGotNextVector2 = s->SampledSegment.getHead(CurrentVector2);
					while (bGotNextVector2) { 
						this->SetPosition (CurrentVector2->Getx(), CurrentVector2->Gety()); /* set new emitter position */
						/* decide if emit or not, likely not. Decrease 0.99 to give more chance and density */
						if ((KRandom::getRandomFloat() - 0.985f) > 0) {
							for (i = 0; i < lRate; i++) {
								VfxParticle *p = new VfxParticle;
								p->x = fX + (KRandom::getRandomFloat() - 0.5f) * 3;
								p->y = fY + GetRandom(-20.0f, 1.0f); //(KRandom::getRandomFloat() - 1.0f) * 10;
								p->vx = (KRandom::getRandomFloat() - 0.5f) *0.2f;
								p->vy = ((KRandom::getRandomFloat()) - 1.0f) * 0.01f; 
								p->fScale = KRandom::getRandomFloat() * 0.8f + 0.25f;
								// p->fScale = 0.5f;
								p->fColor[0] = 0.8f;  /* Red */
								p->fColor[1] = 0.8f + KRandom::getRandomFloat() * 0.25f;  /* Green */ 
								p->fColor[2] = 0.8f + KRandom::getRandomFloat() * 0.1f;  /* Blue */
								p->fColor[3] = 0.0f;
								p->fAge = 0;
								p->fAgeIncr = 0.01f;
								g_lParticles.addToTail (p);
							} /* end for */
						} /* end if */
						bGotNextVector2 = s->SampledSegment.getNext(CurrentVector2); /* get next sample position, if any */
					} /* end while NextVector2 */ 
					bGotNextSegment = Polygon.getNext (s); /* get next segment, if any */ 
				} /* end while NextSegment */
			
			}  	/* end if Polygon */
			break;

		case VFX_FOCUS :
			// only emit one particle so create one and this is it						
			VfxParticle *p;

		   if (g_lParticles.isEmpty() && lpProgress->isCompleted())
			{
				// no particle yet : create one... or not
				if ((KRandom::getRandomFloat()) > 0.5) { // 0.99 is too often, 0.999 is rare
               fMaxEmitter = GetRandom(5000.0f, 15000.0f);
               lpProgress->startCounter(0.0f, 1.0f, 0, fMaxEmitter, K_COUNTER_LINEAR);
					p = new VfxParticle();
					p->x = fX;
					p->y = fY;
					p->vx = 0; // useless
					p->vy = 0; // useless
					p->fScale = 0.5f; 
					p->fColor[0] = 1.0f;  /* Red */	
					p->fColor[1] = 1.0f;  /* Green */ 
					p->fColor[2] = 1.0f;  /* Blue */
					p->fColor[3] = 0.0f; // start from full transparent
					p->fAge = 0; 
					p->fAgeIncr = 0.005f; 
					g_lParticles.addToTail (p);
				}
			}
			break;

		default : 
			break;
		} /* end of switch (lTypeOfEffect) */

	} /* end of if (IsStarted()) */

} /* end of ParticleEmitter::Init */ 

void ParticleEmitter::Logic(void) {
	     bool bGotNext;
		 VfxParticle *p;
          
		 if (!IsKilled()) {
			switch (lTypeOfEffect) {
				case VFX_TRAIL:
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						p->x += p->vx;
						p->y += p->vy;

						p->fColor[3] -= p->fAgeIncr; //0.01f;
						p->fAge += p->fAgeIncr;

						if (p->fAge >= 1) {
							g_lParticles.remove (p);
							delete p;
						}

						p = lpNextParticle;
					}
					break;
				case VFX_FIRE : 
					/* Update particles */
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						if (p->fAge >= (fAge/3)) // at third of the lifetime
							p->x -= p->vx; // make particles closer to centre of flame */
						else 
							p->x += p->vx;

						p->y += p->vy;

						p->fAge += p->fAgeIncr;

						if (p->fAge >= fAge) {  
							g_lParticles.remove (p);
							delete p;
						}
						p = lpNextParticle;
					}
					break;

				case VFX_SMOKE :
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						p->x += p->vx;
						p->y += p->vy;
						p->vy -= 0.5f; /* rajout de gravité, pour faire aller vers le bas sur les y */

						p->fAge += p->fAgeIncr;

						if (p->fAge >= 1) {
							g_lParticles.remove (p);
							delete p;
						}
						p = lpNextParticle;
					}
					break;

				case VFX_EXPLOSION :
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						p->x += p->vx;
						p->y += p->vy;

						p->fAge += p->fAgeIncr;

						if (p->fAge >= 1) {
							g_lParticles.remove (p);
							delete p;
						}

						p = lpNextParticle;
					}
					break;

				case VFX_SPRAY :
					fDisplayEveryLogic++;
					if (fDisplayEveryLogic > 6) {
						fDisplayEveryLogic = 0;

					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);            
						p->x += p->vx;
						p->y += p->vy;
					//	if (p->vy <= 4)
							p->vy += 0.6f; // rajout de gravité pour refaire tomber, au lieu de 0.6
						p->fAge += p->fAgeIncr;
						if (p->fAge >= 0.5) {
							g_lParticles.remove (p);
							delete p;
						}
						p = lpNextParticle;
					}
					}
					break;

				case VFX_MOVING : 
					/* Update particles */
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						if (p->fAge >= 0.1)
							p->x -= p->vx; /* rapprocher la particule du centre si en fin de vie */
						else 
							p->x += p->vx;

						p->y += p->vy;
						p->vy -= 0.5f; /* rajout de gravité, pour faire aller vers le bas sur les y */

						p->fAge += p->fAgeIncr;

						if (p->fAge >= 0.2) { /* âge diminué pour avoir une flamme courte */  
							g_lParticles.remove (p);
							delete p;
						}
						p = lpNextParticle;
					}
					break;

				case VFX_FLAKE :
					bGotNext = g_lParticles.getHead (p);
					while (bGotNext) {
						VfxParticle *lpNextParticle = p;
            
						bGotNext = g_lParticles.getNext (lpNextParticle);
            
						p->y += p->vy;
						p->vy -= 0.001f; /* this is to go up and slowly. To go down, add instead substract */

						if (p->fAge < 2){
							p->x += p->vx/2;
							p->fColor[3] += 0.005f; // fade in until mid-life
						}
						else {
							p->x -= p->vx/2;
							p->fColor[3] -= 0.01f; // post mid-life, start fading out
						}
						p->fAge += p->fAgeIncr;

						if (p->fAge >= 4) {
							g_lParticles.remove (p);
							delete p;
						}
						p = lpNextParticle;
					}
					break;

				case VFX_FOCUS :
					// Only one particle, which is static so no position change
					if (g_lParticles.getHead (p)){
						if (p->fAge < 1)
							p->fColor[3] += 0.005f; // fade in until mid-life
						else
							p->fColor[3] -= 0.01f; // post mid-life, start fading out

						p->fAge += p->fAgeIncr;
						if (p->fAge >= 2) {
							g_lParticles.remove (p);
							delete p;
						}
	
					}
					break;

				default: 
					break;
			 }
		}

}

/*********************************************/
/************** Vertice section **************/
/*********************************************/
Vertice::Vertice (float x, float y) {
	Point2D = new KVector2;
	Point2D->x = x;
	Point2D->y = y;
}; 

Vertice::Vertice () {
	Point2D = new KVector2;
}; 

Vertice::~Vertice () {
	delete Point2D;
}; 
float Vertice::Getx(void){
	return Point2D->x;
};

float Vertice::Gety(void){
	return Point2D->y;
};

long Vertice::GetIndex(void){
	return fIndex;
};
void Vertice::SetIndex(long i){
	fIndex = i;
};
/*********************************************/
/************** Segment section **************/
/*********************************************/
Segment::Segment(float x1, float y1, float x2, float y2, float fIncrement) {
	fX1 = x1;
	fX2 = x2;
	fY1 = y1;
	fY2 = y2;
	this->Sampling (fIncrement);
};
Segment::~Segment(void) {
   Vertice *o;
   Vertice *otodelete;
   bool bGotNext = SampledSegment.getHead(o);
   
   while (bGotNext) {
      otodelete = o;
      bGotNext = SampledSegment.getNext(o);
      SampledSegment.remove(otodelete);
      XDELETE(otodelete);
   }

}

float Segment::length(void) {
	return sqrtf (K_SQUARE(fX2 - fX1) + K_SQUARE(fY2 - fY1));
};

void Segment::Sampling(float fIntervalLength) {
	float length = this->length();
	float lIntervalNbr; /* should be the number of points +1 */ 
	float lDeltaX; /* x increment between 2 consecutive sampled points */
	float lDeltaY; /* y increment between 2 consecutive sampled points */
	long i;
	float x, y;

	/* no sampling if length is null : keep original segment */ 
	if (fIntervalLength == 0) {
		Vertice *v1 = new Vertice (fX1, fY1);
		Vertice *v2 = new Vertice (fX2, fY2);
		v1->SetIndex(0); /* set index of current point in the list */ 
		SampledSegment.addToTail(v1); /* add current point to list of sampled points */ 
		v2->SetIndex(1); /* set index of current point in the list */ 
		SampledSegment.addToTail(v2); /* add current point to list of sampled points */ 
	}
	else {
		lIntervalNbr = floorf (length /fIntervalLength); /* should be the number of points +1 */ 
		/* no sampling if length to sample is larger than segment length : keep original segment */ 
		if (lIntervalNbr == 0) {
			Vertice *v1 = new Vertice (fX1, fY1);
			Vertice *v2 = new Vertice (fX2, fY2);
			v1->SetIndex(0); /* set index of current point in the list */ 
			SampledSegment.addToTail(v1); /* add current point to list of sampled points */ 
			v2->SetIndex(1); /* set index of current point in the list */ 
			SampledSegment.addToTail(v2); /* add current point to list of sampled points */ 
		}
		else {
			lDeltaX = (fX2 - fX1) / lIntervalNbr; /* x increment between 2 consecutive sampled points */
			lDeltaY = (fY2 - fY1) / lIntervalNbr; /* y increment between 2 consecutive sampled points */

			x = fX1; /* init first vertice to segment origin */
			y = fY1;

			/* build list of sampled points. There are lIntervalNbr sampled points */
			for (i = 0; i < lIntervalNbr ; i++) {
				Vertice *v = new Vertice (x, y); /* init current point with computed coordinates */ 
				v->SetIndex(i); /* set index of current point in the list */ 
				SampledSegment.addToTail(v); /* add current point to list of sampled points */ 
	
				/* compute next sampled point */ 
				x += lDeltaX;
				y += lDeltaY;
			}
		}
	}
};  
/*********************************************/
/********** ParticleFactory section **********/
/*********************************************/
ParticleFactory::ParticleFactory (void){
};

ParticleFactory::~ParticleFactory (void){
	/* delete all particle emitters from list */
	ParticleEmitter *o;
	ParticleEmitter *otodelete;
   bool bGotNext = lParticlesEmitter.getHead (o);
	while (bGotNext) {
		otodelete = o;
		lParticlesEmitter.remove(o);
		bGotNext = lParticlesEmitter.getNext(o);
		XDELETE(o);
	}
};

void ParticleFactory::Logic(void){
	ParticleEmitter *pe;
	bool bGotNext;

	bGotNext = lParticlesEmitter.getHead (pe);
	while (bGotNext) {
		pe->Logic();
		bGotNext = lParticlesEmitter.getNext (pe);
	}
}

void ParticleFactory::Render(double fElapsed){
	ParticleEmitter *pe;
	bool bGotNextEmitter;

	bGotNextEmitter = lParticlesEmitter.getHead (pe);
	while (bGotNextEmitter) {		
		pe->Render(fElapsed);
		bGotNextEmitter = lParticlesEmitter.getNext (pe);
	}
}

	

void ParticleFactory::AddEmitter(ParticleEmitter *pe){
	lParticlesEmitter.addToTail (pe);
}
