#include "KInput.h"
#include "EPointer.h"
#include "MyGame.h"

//void EPointer::init() const {
//	X = recast_const(0.0);
//	Y = recast_const(0.0);
//}

float EPointer::_relX = 0.0;
float EPointer::_relY = 0.0;

bool EPointer::_bMouseLeftDown = false;
bool EPointer::_bMouseRightDown = false;
bool EPointer::_bMouseMiddleDown = false;

bool EPointer::_bMouseLeftState = false;
bool EPointer::_bMouseRightState = false;
bool EPointer::_bMouseMiddleState = false;

EPointer *EPointer::pointer = NULL; 

const EPointer Mouse;

EPointer::EPointer()
{
   KAssert(pointer == NULL);
   pointer = this;
}

void EPointer::init() const{
	EPointer *tmp = const_cast<EPointer *> (this);

	tmp->_bMouseLeftState = KInput::getLeftButtonState();
/*	if (_bMouseLeftDown && ! _bMouseLeftState)
		_bMouseLeftDown = false;
*/

	tmp->_bMouseRightState = KInput::getRightButtonState();
/*	if (_bMouseRightDown && ! _bMouseRightState)
		_bMouseRightDown = false;
*/

	tmp->_bMouseMiddleState = KInput::getMiddleButtonState();
/*	if (_bMouseMiddleDown && ! _bMouseMiddleState)
		_bMouseMiddleDown = false;
*/
	tmp->x = KInput::getMouseX() - _relX;
	tmp->y = KInput::getMouseY() - _relY;
}

void EPointer::SetRelativePos(float _x, float _y) const {
	EPointer *tmp = const_cast<EPointer *> (this);

	if (_relX != 0 || _relY != 0) {
		tmp->x = x + _relX;
		tmp->y = y + _relY;
	}

	tmp->_relX = _x;
	tmp->_relY = _y;
	tmp->x = x - _relX;
	tmp->y = y - _relY;
}
void EPointer::SetRelativePos(struct Coord c) const {
	EPointer::SetRelativePos(c.x, c.y);
}

void EPointer::UnsetRelativePos() const {
	EPointer *tmp = const_cast<EPointer *> (this);
	tmp->x = x + _relX;
	tmp->y = y + _relY;
	tmp->_relX = 0;
 	tmp->_relY = 0;
}

bool EPointer::LeftEvent() const {
	return !_bMouseLeftDown && _bMouseLeftState;
}
bool EPointer::RightEvent() const {
	return !_bMouseRightDown && _bMouseRightState ;
}
bool EPointer::MiddleEvent() const {
	return !_bMouseMiddleDown && _bMouseMiddleState ;
}
bool EPointer::PressEvent() const {
	return LeftEvent() || RightEvent() || MiddleEvent();
}

bool EPointer::LeftReleaseEvent() const {
	return _bMouseLeftDown && ! _bMouseLeftState;
}
bool EPointer::RightReleaseEvent() const {
	return _bMouseRightDown && ! _bMouseRightState;
}
bool EPointer::MiddleReleaseEvent() const {
	return _bMouseMiddleDown && ! _bMouseMiddleState;
}
bool EPointer::ReleaseEvent() const {
	return LeftReleaseEvent() || RightReleaseEvent() || MiddleReleaseEvent();
}

bool EPointer::Event() const {
	return ReleaseEvent() || PressEvent();
}

void EPointer::LeftEventClose() const {
	_bMouseLeftDown = _bMouseLeftState;
}
void EPointer::RightEventClose() const {
	_bMouseRightDown = _bMouseRightState;
}
void EPointer::MiddleEventClose() const {
	_bMouseMiddleDown = _bMouseMiddleState;
}
void EPointer::EventClose() const {
	LeftEventClose();
	RightEventClose();
	MiddleEventClose();
}

bool EPointer::LeftState() const {
	return _bMouseLeftDown;
}
bool EPointer::RightState() const {
	return _bMouseRightDown;
}
bool EPointer::MiddleState() const {
	return _bMouseMiddleDown;
}

//void EPointer::ExecuteInTranslatedPos(void &function(void)) {;}
