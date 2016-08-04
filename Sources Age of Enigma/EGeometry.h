#ifndef EGEOMETRY_H
#define EGEOMETRY_H
#include "EMinijeuHelper.h"

namespace EGeometry
{
	double Distance(float Ax, float Ay, float Bx, float By);
	double Distance(Coord A, Coord B);

	Coord Vector(float Ax, float Ay, float Bx, float By);
	Coord Vector(Coord A, Coord B);

	bool IsInRange(Coord cCentre, float fRayon, Coord cPoint);

	Coord Center(float Ax, float Ay, float Bx, float By);
	Coord Center(Coord A, Coord B);

	// line
	bool IsLineParallele(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy);
	bool IsLineParallele(Coord A, Coord B, Coord C, Coord D);
	bool IsLineIntersect(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy);
	bool IsLineIntersect(Coord A, Coord B, Coord C, Coord D);

	Coord LineIntersection(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy);
	Coord LineIntersection(Coord A, Coord B, Coord C, Coord D);

	// segment
	bool IsSegmentIntersect(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy, bool testSegEnd = true);
	bool IsSegmentIntersect(Coord A, Coord B, Coord C, Coord D, bool testSegEnd=true);

	// projection orthogonal sur segment

	Coord GetSegmentEnd(double Longueur, double Angle);
	Coord GetSegmentEnd(Coord pos, double Longueur, double Angle);

	float Angle(float Px, float Py); // Depuit l'origine (0,0)
	float Angle(Coord A);

	float Angle(float Ox, float Oy, float Px, float Py);
	float Angle(Coord O, Coord P);
	
	float Angle(Coord A, Coord O, Coord B);

	double degtorad(float deg);

	enum Cadrant {RIGHT,TOP,LEFT,DOWN};	
	short int GetCadrant(float angle);

	float round(float x);
	double round(double x);
};

#endif