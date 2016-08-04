#ifndef EPOINTER_H
#define EPOINTER_H

#include "EMiniJeuHelper.h"

class EPointer: public Coord {
public:
	EPointer();
	~EPointer() {;}

	void init() const;
	
	void exec(float x, float y, void (*fnc)()) const {
		float tmpx = _relX;
		float tmpy = _relY;
		
		SetRelativePos(x,y);
		fnc();

		SetRelativePos(tmpx,tmpy);
	}

	void GetPos();
	void SetRelativePos(float x, float y) const;
	void SetRelativePos(struct Coord c) const;

	void UnsetRelativePos() const;

	bool LeftEvent() const;
	bool RightEvent() const;
	bool MiddleEvent() const;
	bool PressEvent() const;
	
	bool LeftReleaseEvent() const;
	bool RightReleaseEvent() const;
	bool MiddleReleaseEvent() const;
	bool ReleaseEvent() const;

	bool Event() const;


	void LeftEventClose() const;
	void RightEventClose() const;
	void MiddleEventClose() const;
	
	void EventClose() const;

	bool LeftState() const;
	bool RightState() const;
	bool MiddleState() const;

	//static void ExecuteInTranslatedPos(void &function(void)) const;

private:
	static  bool _bMouseLeftState;
	static  bool _bMouseRightState;
	static  bool _bMouseMiddleState;

	static  bool _bMouseLeftDown;
	static  bool _bMouseRightDown;
	static  bool _bMouseMiddleDown;

	static float _relX;
	static float _relY;

	static EPointer *pointer; 
};

extern const EPointer Mouse;
#endif