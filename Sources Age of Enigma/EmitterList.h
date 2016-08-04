/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * You may freely use or modify this code
 *
 * Support: pyro-support@fenomen-games.com
 *
 */

#ifndef _PYRO_SAMPLES_COMMON_EMITTER_LIST_H_INCLUDED
#define _PYRO_SAMPLES_COMMON_EMITTER_LIST_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include "Pyro.h"
#include <assert.h>
#include <stdlib.h>

namespace PyroParticles
{

	class CEmitterList
	{

	public:

		struct CEmitter
		{
			float x, y, z;
			float m_ActivityTime;
			float m_StartTime;
			PyroParticles::IPyroParticleEmitter *m_pEmitter;
		};
		CEmitter *m_pEmitterList;

		int m_nEmitters;
	private:	


		int m_nMaxEmitters;
		int m_nMaxParticles;
		PyroBool m_bUniformDistribution;
	    
	public:

		CEmitterList(int nMaxParticles = -1, PyroBool bUniformDistribution = PyroFalse)
		{
			m_nMaxParticles = nMaxParticles;
			m_bUniformDistribution = bUniformDistribution;

			m_pEmitterList = 0;
			m_nEmitters = 0;
			m_nMaxEmitters = 0;
		}

		~CEmitterList()
		{
			if (m_pEmitterList)
			{
				for (int i = 0; i < m_nEmitters; i ++)
					delete m_pEmitterList[i].m_pEmitter;

				free(m_pEmitterList);
			}	
		}

		CEmitter &operator[](int nEmitter) const
		{
			KAssert(nEmitter >= 0 && nEmitter < m_nEmitters);
			return m_pEmitterList[nEmitter];
		}
		
		PyroParticles::IPyroParticleEmitter *Append(PyroParticles::IPyroFile *pPyroFile, const char *pEmitterName, float ActivityTime, float x, float y, float z = 0.0f)
		{
			PyroParticles::IPyroParticleEmitter *pEmitter = pPyroFile->CreateEmitter(pEmitterName);
	        
			if (m_nEmitters == m_nMaxEmitters)
			{
		 		if (m_nMaxEmitters == 0)
		 			m_nMaxEmitters = 4;
		 		m_nMaxEmitters *= 2;
		 		m_pEmitterList = (CEmitter *) realloc(m_pEmitterList, sizeof(CEmitter) * m_nMaxEmitters);
			}

			m_pEmitterList[m_nEmitters].x = x;
			m_pEmitterList[m_nEmitters].y = y;
			m_pEmitterList[m_nEmitters].z = z;
			m_pEmitterList[m_nEmitters].m_pEmitter     = pEmitter;
			m_pEmitterList[m_nEmitters].m_ActivityTime = ActivityTime;
			m_pEmitterList[m_nEmitters].m_StartTime    = ActivityTime;
			m_nEmitters ++;

			return pEmitter;
		}

		void RemoveInactive(float Time, float MaxDelta)
		{
			if (m_nEmitters == 0)
				return;

			int nActiveEmitters = 0;
			CEmitter *pSrc  = m_pEmitterList;
			CEmitter *pDest = m_pEmitterList;
	                
			for (int i = 0; i < m_nEmitters; i ++, pSrc ++)
			{
				if (Time - pSrc->m_ActivityTime < MaxDelta)
				{
					*pDest ++ = *pSrc;
					nActiveEmitters ++;				
				}
				else
				{
					delete pSrc->m_pEmitter;
				}
			}

			m_nEmitters = nActiveEmitters;
		}
			
		void Render(float Time)
		{
			if (m_nMaxParticles == 0)
				return;

			if (m_nMaxParticles < 0)
			{
				for (int i = 0; i < m_nEmitters; i ++)
					m_pEmitterList[i].m_pEmitter->Render();
			}
			else
			{
				int i;

				int nTotalParticles = 0;
				for (i = 0; i < m_nEmitters; i ++)
					nTotalParticles += m_pEmitterList[i].m_pEmitter->GetNumParticles();

				if (nTotalParticles == 0)
					return;

				for (i = 0; i < m_nEmitters; i ++)
				{
					int nEmitterParticles = m_pEmitterList[i].m_pEmitter->GetNumParticles();
					int nEmitterMaxParticles = m_nMaxParticles * nEmitterParticles / nTotalParticles;

					m_pEmitterList[i].m_pEmitter->Render(nEmitterMaxParticles, m_bUniformDistribution);
				}
			}
		}

		void Prepare(float Time, float TimeDelta)
		{
			KAssert(TimeDelta >= 0);

			for (int i = 0; i < m_nEmitters; i ++)
			{
				m_pEmitterList[i].m_pEmitter->Prepare(
					Time - m_pEmitterList[i].m_StartTime, TimeDelta, 
					m_pEmitterList[i].x,
					m_pEmitterList[i].y,
					m_pEmitterList[i].z,
					PyroTrue);
				if (m_pEmitterList[i].m_pEmitter->HasParticles())
					m_pEmitterList[i].m_ActivityTime = Time;

			}
		}
		
		void Move(float Time, float TimeDelta)
		{
			for (int i = 0; i < m_nEmitters; i ++)
				m_pEmitterList[i].m_pEmitter->Move(Time - m_pEmitterList[i].m_StartTime, TimeDelta);
		}
		
		int GetCount() const { return m_nEmitters; }

	};

}

#endif /* _PYRO_SAMPLES_COMMON_EMITTER_LIST_H_INCLUDED */
