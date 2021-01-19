#include "Torus.h"          
#define	TwoPi  6.28318530717958648
const double eps = 1e-14; 

Torus::Torus(glm::vec3 p, float r1, float r2, string name, ofColor diffuse) {
	position = p;
	R = r1;
	r = r2;
	objName = name;
	diffuseColor = diffuse;
}

// Constructor with angle and axis of ratation parameters
Torus::Torus(glm::vec3 p, float r1, float r2, float a, glm::vec3 rot, string name, ofColor diffuse) {
	position = p;
	R = r1;
	r = r2;
	objName = name;
	diffuseColor = diffuse;
	angle = a;
	axisR = rot;
}

bool Torus::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec2 tor = glm::vec2(R, r);

	//float po = 1.0;
	//float Ra2 = tor.x * tor.x;
	//float ra2 = tor.y * tor.y;

	//float m = dot(ray.p, ray.p);
	//float n = dot(ray.p, ray.d);

	//float k = (m - ra2 - Ra2) / 2.0;
	//float k3 = n;
	//float k2 = n * n + Ra2 * ray.d.z * ray.d.z + k;
	//float k1 = k * n + Ra2 * ray.p.z * ray.d.z;
	//float k0 = k * k + Ra2 * ray.p.z * ray.p.z - Ra2 * ra2;

	//if (abs(k3 * (k3 * k3 - k2) + k1) < 0.01) {
	//	po = -1.0;
	//	float tmp = k1;
	//	k1 = k3;
	//	k3 = tmp;

	//	k0 = 1.0 / k0;
	//	k1 = k1 * k0;
	//	k2 = k2 * k0;
	//	k3 = k3 * k0;
	//}

	//float c2 = 2.0 * k2 - 3.0 * k3 * k3;
	//float c1 = k3 * (k3 * k3 - k2) + k1;
	//float c0 = k3 * (k3 * (-3.0 * k3 * k3 + 4.0 * k2) - 8.0 * k1) + 4.0 * k0;

	//c2 /= 3.0;
	//c1 *= 2.0;
	//c0 /= 3.0;

	//float Q = c2 * c2 + c0;
	//float Ra = 3.0 * c0 * c2 - c2 * c2 * c2 - c1 * c1;

	//float h = Ra * Ra - Q * Q * Q;
	//float z = 0.0;
	//if (h < 0.0) {
	//	// 4 intersections
	//	float sQ = sqrt(Q);
	//	z = 2.0 * sQ * cos(acos(Ra / (sQ * Q)) / 3.0);
	//}
	//else {
	//	// 2 intersections
	//	float sQ = pow(sqrt(h) + abs(Ra), 1.0 / 3.0);
	//	z = glm::sign(Ra) * abs(sQ + Q / sQ);
	//}
	//z = c2 - z;

	//float d1 = z - 3.0 * c2;
	//float d2 = z * z - 3.0 * c0;

	//if (abs(d1) < 1.0e-4) {
	//	if (d2 < 0.0)
	//		return false;
	//	d2 = sqrt(d2);
	//}
	//else {
	//	if (d1 < 0.0)
	//		return false;
	//	d1 = sqrt(d1 / 2.0);
	//	d2 = c1 / d1;
	//}

	//float result = 1e20;

	//h = d1 * d1 - z + d2;
	//if (h > 0.0) {
	//	h = sqrt(h);
	//	float t1 = -d1 - h - k3;
	//	t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = -d1 + k - k3;
	//	t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	
	//	if (t1 > 0.0) {
	//		result = t1;
	//	}
	//	if (t2 > 0.0) {
	//		result = min(result, t2);
	//	}
	//}

	//h = d1 * d1 - z - d2;
	//if (h > 0.0) {
	//	h = sqrt(h);
	//	float t1 = d1 - h - k3;
	//	t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = d1 + h - k3;
	//	t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	
	//	if (t1 > 0.0)
	//		result = min(result, t1);
	//	if (t2 > 0.0)
	//		result = min(result, t2);
	//}

	//float t = result;
	float mm = dot(ray.d, ray.d);
	float nn = dot(ray.p, ray.d);
	float ee = dot(ray.p, ray.p) - (R * R + r * r);
	float four_a_sqrd = 4.0 * R * R;

	float cc4 = mm * mm;
	float cc3 = 4.0 * mm * nn;
	float cc2 = 2.0 * mm * ee + 4.0 * nn * nn + four_a_sqrd * ray.d.z * ray.d.z;
	float cc1 = 4.0 * nn * ee + 2.0 * four_a_sqrd * ray.p.z * ray.d.z;
	float cc0 = ee * ee - four_a_sqrd * (r * r - ray.p.z * ray.p.z);

	// Quartic variables
	cc4 /= cc4;
	cc3 /= cc4;
	cc2 /= cc4;
	cc1 /= cc4;
	cc0 /= cc4;

	double roots[4];
	int realRoots = SolveP4(roots, cc3, cc2, cc1, cc0);

	if (realRoots == 0)
		return false;

	float root = FLT_MAX;
	for (int i = 0; i < realRoots; i++) {
		if(root > static_cast<float>(roots[i]))
			root = static_cast<float>(roots[i]);
	}
	if (root < 0.0001) return false;

	Ray temp = Ray(ray.p, ray.d);
	point = temp.evalPoint(root);
	//cout << "[" << imageY << ", " << imageX << "]	" << point << endl;
	glm::vec3 pPrime = glm::vec3(point.x, point.y, 0.0);
	float Qp = R / sqrt(point.x * point.x + point.y * point.y);

	normal = glm::normalize(point * (dot(point, point) - tor.y * tor.y - tor.x * tor.x * glm::vec3(1.0, 1.0, -1.0)));
	//double an = 1.0 - (R / sqrt(point.x * point.x + point.y * point.y));
	//normal = glm::normalize(glm::vec3(an * point.x, an * point.y, point.z));
	//normal = glm::vec3(
	//	point.x * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R),
	//	point.y * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R),
	//	point.z * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R * -1)
	//);

	//float paramSquared = R * R + r * r;
	//float px = point.x;
	//float py = point.y;
	//float pz = point.z;
	//float sumSquared = px * px + py * py + pz * pz;

	//normal = glm::vec3(
	//	4.0 * px * (sumSquared - paramSquared),
	//	4.0 * py * (sumSquared - paramSquared),
	//	4.0 * pz * (sumSquared - paramSquared + 2.0 * R * R)
	//);

	//normal = glm::normalize(normal);
	//if (shading) {
		//points.push_back(point);
		//normals.push_back((point + normal/2));
	//}
	//cout << "Num: " << points.size() << " \\ " << Fpoints.size() << endl;
	return true;
}



void Torus::draw() {
	ofDisableLighting();
	ofSetColor(ofColor::red);
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::blue);
		//ofDrawSphere(points[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}
	for (int i = 0; i < Fpoints.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(Fpoints[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}
	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofTranslate(position);
			ofRotate(angle, axisR.x, axisR.y, axisR.z);
			ofDrawAxis(R * 1.5);
			drawTorus();
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);

	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofTranslate(position);
		ofRotate(angle, axisR.x, axisR.y, axisR.z);
		drawTorus();
	ofPopMatrix();
	material.end();
}

void Torus::drawTorus() {
	double s, x, y, z;

	for (int i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= numt; j++) {
			for (int k = 1; k >= 0; k--) {
				s = (i + k) % numc;
				x = (R + r * cos(s * 2 * PI / numc))*cos(j * 2 * PI / numt);
				y = (R + r * cos(s * 2 * PI / numc))*sin(j * 2 * PI / numt);
				z = r * sin(s * 2 * PI / numc);
				glVertex3f(x, y, z);

			}
		}
		glEnd();
	}
}

// Calculate the sdf of the torus scene object while applying transformations
float Torus::sdf(glm::vec3 p1) {
	// Transformation matrices(translate and rotate)
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.y)) - R, p.z);
	return glm::length(q) - r;
}

//=============================================================================
// _root3, root3 from http://prografix.narod.ru
//=============================================================================
static double _root3(double x) {
	double s = 1.;
	while (x < 1.) {
		x *= 8.;
		s *= 0.5;
	}
	while (x > 8.) {
		x *= 0.125;
		s *= 2.;
	}
	double r = 1.5;
	r -= 1. / 3. * (r - x / (r * r));
	r -= 1. / 3. * (r - x / (r * r));
	r -= 1. / 3. * (r - x / (r * r));
	r -= 1. / 3. * (r - x / (r * r));
	r -= 1. / 3. * (r - x / (r * r));
	r -= 1. / 3. * (r - x / (r * r));
	return r * s;
}

double root3(double x) {
	if (x > 0) return _root3(x); else
		if (x < 0) return-_root3(-x); else
			return 0.;
}


// x - array of size 2
// return 2: 2 real roots x[0], x[1]
// return 0: pair of complex roots: x[0]켲*x[1]
int   Torus::SolveP2(double *x, double a, double b) {			// solve equation x^2 + a*x + b = 0
	double D = 0.25*a*a - b;
	if (D >= 0) {
		D = sqrt(D);
		x[0] = -0.5*a + D;
		x[1] = -0.5*a - D;
		return 2;
	}
	x[0] = -0.5*a;
	x[1] = sqrt(-D);
	return 0;
}
//---------------------------------------------------------------------------
// x - array of size 3
// In case 3 real roots: => x[0], x[1], x[2], return 3
//         2 real roots: x[0], x[1],          return 2
//         1 real root : x[0], x[1] � i*x[2], return 1
int Torus::SolveP3(double *x, double a, double b, double c) {	// solve cubic equation x^3 + a*x^2 + b*x + c = 0
	double a2 = a * a;
	double q = (a2 - 3 * b) / 9;
	double r = (a*(2 * a2 - 9 * b) + 27 * c) / 54;
	// equation x^3 + q*x + r = 0
	double r2 = r * r;
	double q3 = q * q*q;
	double A, B;
	if (r2 <= (q3 + eps)) {//<<-- FIXED!
		double t = r / sqrt(q3);
		if (t < -1) t = -1;
		if (t > 1) t = 1;
		t = acos(t);
		a /= 3; q = -2 * sqrt(q);
		x[0] = q * cos(t / 3) - a;
		x[1] = q * cos((t + TwoPi) / 3) - a;
		x[2] = q * cos((t - TwoPi) / 3) - a;
		return(3);
	}
	else {
		//A =-pow(fabs(r)+sqrt(r2-q3),1./3); 
		A = -root3(fabs(r) + sqrt(r2 - q3));
		if (r < 0) A = -A;
		B = (A == 0 ? 0 : B = q / A);

		a /= 3;
		x[0] = (A + B) - a;
		x[1] = -0.5*(A + B) - a;
		x[2] = 0.5*sqrt(3.)*(A - B);
		if (fabs(x[2]) < eps) { x[2] = x[1]; return(2); }
		return(1);
	}
}// SolveP3(double *x,double a,double b,double c) {	
//---------------------------------------------------------------------------
// a>=0!
void Torus::CSqrt(double x, double y, double &a, double &b) // returns:  a+i*s = sqrt(x+i*y)
{
	double r = sqrt(x*x + y * y);
	if (y == 0) {
		r = sqrt(r);
		if (x >= 0) { a = r; b = 0; }
		else { a = 0; b = r; }
	}
	else {		// y != 0
		a = sqrt(0.5*(x + r));
		b = 0.5*y / a;
	}
}
//---------------------------------------------------------------------------
int   Torus::SolveP4Bi(double *x, double b, double d)	// solve equation x^4 + b*x^2 + d = 0
{
	double D = b * b - 4 * d;
	if (D >= 0) {
		double sD = sqrt(D);
		double x1 = (-b + sD) / 2;
		double x2 = (-b - sD) / 2;	// x2 <= x1
		if (x2 >= 0)				// 0 <= x2 <= x1, 4 real roots
		{
			double sx1 = sqrt(x1);
			double sx2 = sqrt(x2);
			x[0] = -sx1;
			x[1] = sx1;
			x[2] = -sx2;
			x[3] = sx2;
			return 4;
		}
		if (x1 < 0)				// x2 <= x1 < 0, two pair of imaginary roots
		{
			double sx1 = sqrt(-x1);
			double sx2 = sqrt(-x2);
			x[0] = 0;
			x[1] = sx1;
			x[2] = 0;
			x[3] = sx2;
			return 0;
		}
		// now x2 < 0 <= x1 , two real roots and one pair of imginary root
		double sx1 = sqrt(x1);
		double sx2 = sqrt(-x2);
		x[0] = -sx1;
		x[1] = sx1;
		x[2] = 0;
		x[3] = sx2;
		return 2;
	}
	else { // if( D < 0 ), two pair of compex roots
		double sD2 = 0.5*sqrt(-D);
		CSqrt(-0.5*b, sD2, x[0], x[1]);
		CSqrt(-0.5*b, -sD2, x[2], x[3]);
		return 0;
	} // if( D>=0 ) 
} // SolveP4Bi(double *x, double b, double d)	// solve equation x^4 + b*x^2 d
//---------------------------------------------------------------------------
#define SWAP(a,b) { t=b; b=a; a=t; }
static void  dblSort3(double &a, double &b, double &c) // make: a <= b <= c
{
	double t;
	if (a > b) SWAP(a, b);	// now a<=b
	if (c < b) {
		SWAP(b, c);			// now a<=b, b<=c
		if (a > b) SWAP(a, b);// now a<=b
	}
}
//---------------------------------------------------------------------------
int   Torus::SolveP4De(double *x, double b, double c, double d)	// solve equation x^4 + b*x^2 + c*x + d
{
	//if( c==0 ) return SolveP4Bi(x,b,d); // After that, c!=0
	if (fabs(c) < 1e-14*(fabs(b) + fabs(d))) return SolveP4Bi(x, b, d); // After that, c!=0

	int res3 = SolveP3(x, 2 * b, b*b - 4 * d, -c * c);	// solve resolvent
	// by Viet theorem:  x1*x2*x3=-c*c not equals to 0, so x1!=0, x2!=0, x3!=0
	if (res3 > 1)	// 3 real roots, 
	{
		dblSort3(x[0], x[1], x[2]);	// sort roots to x[0] <= x[1] <= x[2]
		// Note: x[0]*x[1]*x[2]= c*c > 0
		if (x[0] > 0) // all roots are positive
		{
			double sz1 = sqrt(x[0]);
			double sz2 = sqrt(x[1]);
			double sz3 = sqrt(x[2]);
			// Note: sz1*sz2*sz3= -c (and not equal to 0)
			if (c > 0) {
				x[0] = (-sz1 - sz2 - sz3) / 2;
				x[1] = (-sz1 + sz2 + sz3) / 2;
				x[2] = (+sz1 - sz2 + sz3) / 2;
				x[3] = (+sz1 + sz2 - sz3) / 2;
				return 4;
			}
			// now: c<0
			x[0] = (-sz1 - sz2 + sz3) / 2;
			x[1] = (-sz1 + sz2 - sz3) / 2;
			x[2] = (+sz1 - sz2 - sz3) / 2;
			x[3] = (+sz1 + sz2 + sz3) / 2;
			return 4;
		} // if( x[0] > 0) // all roots are positive
		// now x[0] <= x[1] < 0, x[2] > 0
		// two pair of comlex roots
		double sz1 = sqrt(-x[0]);
		double sz2 = sqrt(-x[1]);
		double sz3 = sqrt(x[2]);

		if (c > 0)	// sign = -1
		{
			x[0] = -sz3 / 2;
			x[1] = (sz1 - sz2) / 2;		// x[0]켲*x[1]
			x[2] = sz3 / 2;
			x[3] = (-sz1 - sz2) / 2;		// x[2]켲*x[3]
			return 0;
		}
		// now: c<0 , sign = +1
		x[0] = sz3 / 2;
		x[1] = (-sz1 + sz2) / 2;
		x[2] = -sz3 / 2;
		x[3] = (sz1 + sz2) / 2;
		return 0;
	} // if( res3>1 )	// 3 real roots, 
	// now resoventa have 1 real and pair of compex roots
	// x[0] - real root, and x[0]>0, 
	// x[1]켲*x[2] - complex roots, 
	// x[0] must be >=0. But one times x[0]=~ 1e-17, so:
	if (x[0] < 0) x[0] = 0;
	double sz1 = sqrt(x[0]);
	double szr, szi;
	CSqrt(x[1], x[2], szr, szi);  // (szr+i*szi)^2 = x[1]+i*x[2]
	if (c > 0)	// sign = -1
	{
		x[0] = -sz1 / 2 - szr;			// 1st real root
		x[1] = -sz1 / 2 + szr;			// 2nd real root
		x[2] = sz1 / 2;
		x[3] = szi;
		return 2;
	}
	// now: c<0 , sign = +1
	x[0] = sz1 / 2 - szr;			// 1st real root
	x[1] = sz1 / 2 + szr;			// 2nd real root
	x[2] = -sz1 / 2;
	x[3] = szi;
	return 2;
} // SolveP4De(double *x, double b, double c, double d)	// solve equation x^4 + b*x^2 + c*x + d
//-----------------------------------------------------------------------------
double Torus::N4Step(double x, double a, double b, double c, double d)	// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d
{
	double fxs = ((4 * x + 3 * a)*x + 2 * b)*x + c;	// f'(x)
	if (fxs == 0) return x;	//return 1e99; <<-- FIXED!
	double fx = (((x + a)*x + b)*x + c)*x + d;	// f(x)
	return x - fx / fxs;
}
//-----------------------------------------------------------------------------
// x - array of size 4
// return 4: 4 real roots x[0], x[1], x[2], x[3], possible multiple roots
// return 2: 2 real roots x[0], x[1] and complex x[2]켲*x[3], 
// return 0: two pair of complex roots: x[0]켲*x[1],  x[2]켲*x[3], 
int   Torus::SolveP4(double *x, double a, double b, double c, double d) {	// solve equation x^4 + a*x^3 + b*x^2 + c*x + d by Dekart-Euler method
	// move to a=0:
	double d1 = d + 0.25*a*(0.25*b*a - 3. / 64 * a*a*a - c);
	double c1 = c + 0.5*a*(0.25*a*a - b);
	double b1 = b - 0.375*a*a;
	int res = SolveP4De(x, b1, c1, d1);
	if (res == 4) { x[0] -= a / 4; x[1] -= a / 4; x[2] -= a / 4; x[3] -= a / 4; }
	else if (res == 2) { x[0] -= a / 4; x[1] -= a / 4; x[2] -= a / 4; }
	else { x[0] -= a / 4; x[2] -= a / 4; }
	// one Newton step for each real root:
	if (res > 0) {
		x[0] = N4Step(x[0], a, b, c, d);
		x[1] = N4Step(x[1], a, b, c, d);
	}
	if (res > 2) {
		x[2] = N4Step(x[2], a, b, c, d);
		x[3] = N4Step(x[3], a, b, c, d);
	}
	return res;
}
