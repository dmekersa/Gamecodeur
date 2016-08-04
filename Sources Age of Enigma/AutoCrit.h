#ifndef __AUTOCRIT_INCLUDED__
#define __AUTOCRIT_INCLUDED__

#include "KSysLock.h"

namespace Sexy
{

	/*!***********************************
	// \class AutoCrit
	//
	// \brief		The scope of this object automatically acquires and releases a KSysLock.
	// \details 	<b>Description:</b> Pass a KSysLock object to this method to obtain a lock.
	//************************************/
	class AutoCrit
	{
		KSysLock*		mCritSec;
	public:
		AutoCrit(KSysLock* theCritSec) : 
			mCritSec(theCritSec)
		{ 
			if(mCritSec)
				mCritSec->acquire();
		}

		AutoCrit(const KSysLock& theCritSect) : 
			mCritSec((KSysLock*) &theCritSect)
		{ 
			if(mCritSec)
				mCritSec->acquire();
		}

		~AutoCrit()
		{ 
			if (mCritSec)
				mCritSec->release();
		}
	};

}

#endif //__AUTOCRIT_INCLUDED__