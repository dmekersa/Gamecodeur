#include "EGeometry.h"
#include "math.h"
#include "MyGame.h"

const double Pi = 3.141592654;

namespace EGeometry
{
	double Distance(float Ax, float Ay, float Bx, float By) {
		return sqrt(pow(Bx-Ax,2) + pow(By-Ay,2));
	}

	double Distance(Coord A, Coord B) {
		return (float) Distance(A.x,A.y,B.x,B.y);
	}
	
	bool IsInRange(Coord cCentre, float fRayon, Coord cPoint) {
		return Distance(cCentre, cPoint) <= fRayon;
	}

	Coord Vector(float Ax, float Ay, float Bx, float By) {
		return Coord(Bx - Ax, By - Ay);
	}

	Coord Vector(Coord A, Coord B) {
		return Vector(A.x, A.y, B.x, B.y);
	}

	Coord Center(float Ax, float Ay, float Bx, float By) {
		return Coord ( 
			(float)((double)Ax + ((double)(Bx - Ax) /2)),
			(float)((double)Ay + ((double)(By - Ay)/2))
			);
	}

	Coord Center(Coord A, Coord B) {
		return Center(A.x, A.y, B.x, B.y);
	}

   bool IsLineParallele(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy) {
		double A1 = By - Ay;
		double B1 = Ax - Bx;
      double C1 = round(A1/B1);

		double A2 = Dy - Cy;
		double B2 = Cx - Dx;
		double C2 = round(A2/B2);

		if(C1 == C2){
			return true;
		}
		return false;
	}

   bool IsLineParallele(Coord A, Coord B, Coord C, Coord D) {
		return IsLineParallele(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
	}

	bool IsLineIntersect(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy) {
		double A1 = By - Ay;
		double B1 = Ax - Bx;
		//double C1 = A1*Ax + B1*Ay;

		double A2 = Dy - Cy;
		double B2 = Cx - Dx;
		//double C2 = A2*Cx + B2*Cy;

		double det = A1*B2 - A2*B1;

		if(det == 0){
			return false;
		}
		return true;
	}

	bool IsLineIntersect(Coord A, Coord B, Coord C, Coord D) {
		return IsLineIntersect(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
	}

	Coord LineIntersection(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy) {
		double A1 = By - Ay;
		double B1 = Ax - Bx;
		double C1 = A1*Ax + B1*Ay;

		double A2 = Dy - Cy;
		double B2 = Cx - Dx;
		double C2 = A2*Cx + B2*Cy;

		double det = A1*B2 - A2*B1;
		KAssert(det!=0);

		double x = (B2*C1 - B1*C2)/det;
		double y = (A1*C2 - A2*C1)/det;

		return Coord((float)x,(float)y);
	}

	Coord LineIntersection(Coord A, Coord B, Coord C, Coord D) {
		return LineIntersection(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
	}

   bool IsSegmentIntersect(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy, bool TestSegEnd) {
      if (!IsLineIntersect(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)) {
         return false;
      }


      Coord pos = LineIntersection(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy);
      pos = Coord(round(pos.x),round(pos.y));

      bool res = false;
      if (Bx >= Ax) {
         res= (pos.x <= Bx) && (pos.x >= Ax);
      } else {
         res = (pos.x <= Ax) && (pos.x >= Bx);
      }
      if (By >= Ay) {
         res= res && (pos.y <= By) && (pos.y >= Ay);
      } else {
         res = res && (pos.y <= Ay) && (pos.y >= By);
      }

      if (Dx >= Cx) {
         res = res && (pos.x <= Dx) && (pos.x >= Cx);
      } else {
         res = res && (pos.x <= Cx) && (pos.x >= Dx);
      }
      if (Dy >= Cy) {
         res = res && (pos.y <= Dy) && (pos.y >= Cy);
      } else {
         res = res && (pos.y <= Cy) && (pos.y >= Dy);
      }
      if (!TestSegEnd) { // Prise de marge pour absorber les arrondis
         if (Distance(pos, Coord(Ax,Ay)) <= 1 || 
            Distance(pos, Coord(Bx,By)) <= 1 || 
            Distance(pos, Coord(Cx,Cy)) <= 1 || 
            Distance(pos, Coord(Dx,Dy)) <= 1)
            res = false;
      }
      return res;
   }

	bool IsSegmentIntersect(Coord A, Coord B, Coord C, Coord D, bool TestSegEnd) {
		return IsSegmentIntersect(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y, TestSegEnd);
	}

	Coord GetSegmentEnd(double Longueur, double Angle) {
		Coord c = Coord(0,0);
		double A = (int)(Angle + 360) % 360;
		if ((int) A == 0) {
			c.x = Longueur;
			return c;
		} else if ((int)A == 180) {
			c.x= -Longueur;
			return c;
		} else if ((int)A == 90) {   // Angle direction de plan normal alors que y evolue celons les conventions de coordonnée d'affichage
			c.y = -Longueur;
			return c;
		} else if ((int)A == 270) {
			c.y = Longueur;
			return c;
		}

		c.x = (cos(degtorad(Angle)) * Longueur);
		// Angle direction de plan normal alors que y evolue celons les conventions de coordonnée d'affichage
		c.y = -(sin(degtorad(Angle)) * Longueur) ;
		return c;
	}

	Coord GetSegmentEnd(Coord pos, double Longueur, double Angle) {
		Coord c = GetSegmentEnd(Longueur, Angle);
		return Coord(pos.x+c.x, pos.y+c.y);
	}

	float Angle(float Px, float Py) {
		return 180*atan2f(-Py,Px)/Pi;
	}
	float Angle(Coord P) {
		return Angle(P.x,P.y);
	}
	
	float Angle(float Ox, float Oy, float Px, float Py) {
		return Angle (Px-Ox,Py-Oy);
	}
	float Angle(Coord O, Coord P) {
		return Angle(P-O);
	}
	float Angle(Coord A, Coord O, Coord B) {
		return Angle(O,B) - Angle(O,A);
	}

	double degtorad(float deg) {
		return ((double) deg * (double)Pi) / (double)180;
	}

	short int GetCadrant(float angle) {
		short int cadrant = 0;
		float dir = angle;
		if (dir < 0)
			dir = dir - (((int)(dir/360)-1)*360);
		dir = (float)((int)dir %360) + (dir - (float)((int)dir));

		if (dir < 60 || dir > 300 ) { // Left Right or nothing
			cadrant &= RIGHT;
		} else  if (dir > 120 && dir < 240 ) {
			cadrant &= RIGHT;
		}
		if (dir > 30 || dir < 150 ) { // Left Right or nothing
			cadrant &= TOP;
		} else  if (dir > 220 && dir < 330 ) {
			cadrant &= DOWN;
		}

		return cadrant;
	}

   float round(float x) {
      const float sd = 100; 
      return int(x*sd + (x<0? -0.5 : 0.5))/sd;
   }

   double round(double x) {
      const double sd = 100; 
      return int(x*sd + (x<0? -0.5 : 0.5))/sd;
   }


};