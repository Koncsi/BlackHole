//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization f√üggv√änyt kiv√äve, a lefoglalt adat korrekt felszabad√≠t√•sa n√älk√ül 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Koncser B·lint
// Neptun : ------
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...
//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
	float x, y, z;

	Vector() {
		x = y = z = 0;
	}
	Vector(float x0, float y0, float z0 = 0) {
		x = x0; y = y0; z = z0;
	}
	Vector operator*(float a)const {
		return Vector(x * a, y * a, z * a);
	}
	Vector operator+(const Vector& v)const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v)const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { 	// dot product
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%(const Vector& v) { 	// cross product
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() { return sqrt(x * x + y * y + z * z); }

	Vector normalize(){
		Vector vissza;
		if (this->Length() > 0.0f)
			vissza = *this *(1.0f / fabs(this->Length()));

		return vissza;
	}
};

float dot(Vector a, Vector b){
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
	float r, g, b;

	Color() {
		r = g = b = 0;
	}
	Color(float r0, float g0, float b0) {
		r = r0; g = g0; b = b0;
	}
	Color operator*(float a) {
		return Color(r * a, g * a, b * a);
	}
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
	Color operator+=(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
	Color operator-(const Color& c) {
		return Color(r - c.r, g - c.g, b - c.b);
	}
	Color operator/(const Color& c) {
		return Color(r / c.r, g / c.g, b / c.b);
	}
};

const int screenWidth = 600;	
const int screenHeight = 600;
const int recursionDepth = 3;				
const float eplsilon = 0.001;	

Color image[screenWidth*screenHeight];

class Material{
	Color F0;			
	Color  kd, ks;		
	float shininess;	

public:

	Color ka;			
	bool reflective;	

	Material(Color _kd, float _shine){	
		reflective = false;
		kd = _kd;
		ks = Color(0.15, 0.15, 0.3);
		shininess = _shine;
		ka = Color(4, 4, 4);
	}

	Material(float _shine = 5){			
		reflective = true;
		shininess = _shine;
		ka = Color(4, 4, 4);

		Color nd(0.17f, 0.35f, 1.5f), nk(3.1f, 2.7f, 1.9f);
		
		Color fup = (nd - Color(1, 1, 1))*(nd - Color(1, 1, 1));
		fup = fup + nk*nk;

		Color fdw = (nd + Color(1, 1, 1))*(nd + Color(1, 1, 1));
		fdw = fdw + nk*nk;

		F0 = fup / fdw;
	}

	Color shade(Vector normal, Vector viewDir,
		Vector lightDir, Color inRad)
	{											
		Color reflRad = Color(0,0,0);
		float cosTheta = dot(normal, lightDir);
		if (cosTheta < 0) return reflRad;
		reflRad = inRad * kd * cosTheta;
		Vector halfway = (viewDir + lightDir).normalize();
		float cosDelta = dot(normal, halfway);
		if (cosDelta < 0) return reflRad;
		return reflRad + inRad * ks * powf(cosDelta, shininess);
	}

	Vector reflect(Vector inDir, Vector normal) {		
		return inDir - normal * dot(normal, inDir) * 2.0f;
	}

	Color Fresnel(Vector inDir, Vector normal) {		
		float cosa = fabs(dot(normal, inDir));
		Color vissza = F0 + (Color(1, 1, 1) - F0) * pow(1 - cosa, 5);
		return vissza;
	}
};

struct Ray 
{
	Vector pos, dir;

	Ray(Vector _pos, Vector _dir){
		pos = _pos;
		dir = _dir.normalize();
	}
};

struct Hit {		
	bool valid, blackHole;
	float t;
	Vector position;
	Vector normal;
	Material* material;
	Hit() {			
		t = -1;
		valid = false;
		blackHole = false;
	}
};

class Camera{
	Vector eye, lookat, normal, right, up;
public:
	Camera(){}

	Vector getEye(){
		return eye;
	}
	void initCamera(Vector _eye, Vector _lookat, Vector _up){
		eye = _eye;
		lookat = _lookat;
		Vector w = eye - lookat;
		float f = w.Length();
		right = (_up % w).normalize() * (-30);
		up = (w % right).normalize() * (-30);
	}

	Ray getRay(int x, int y){
		Vector dir = lookat + right*(2.0f*x / screenWidth - 1) + up*(2.0f*y / screenHeight - 1) - eye;
		return Ray(eye, dir.normalize());
	}
};

class Intersectable{ 
protected:
	Material * material;
public:
	virtual Hit intersect(const Ray& ray, float t) = 0;

};

// MetszÈspont sz·mÌt·shoz haszn·latos kÈpletek: Forr·s:
// http://www.ccs.neu.edu/home/fell/CSU540/programs/RayTracingFormulas.htm
class Sphere :public Intersectable{
public:

	Vector center;
	float radius;

	Sphere(Vector _pont, float _radius){
		center = _pont;
		radius = _radius;
	}

	bool in(Ray ray){
		float length = (ray.pos - center).Length();
		if (length < radius) return true;
		else return false;
	}

	Hit intersect(const Ray& ray, float t){
		Hit theHit;
		float t0;

		float a = dot(ray.dir, ray.dir);
		float b = 2.0f*dot(ray.dir, ray.pos - center);
		float c = dot((ray.pos - center), (ray.pos - center)) - radius*radius;
		float disc = (b*b) - (4.0f*a*c);

		if (disc < 0) return theHit;
		t0 = ((-b - sqrtf(disc)) / 2.0f*a);

		if (t0 < eplsilon)
			return theHit;

		theHit.blackHole = true;
		theHit.material = material;
		theHit.t = t0;
		theHit.position = ray.pos + ray.dir*t0;
		theHit.normal = (theHit.position - center).normalize();

		theHit.valid = true;

		return theHit;
	}
};

const int rTes = 4;
const int RTes = 5;

class Torus : public Intersectable{
	Vector center;
	float R, r;
	
	Vector quadList[20][4];

	Vector rv(float u, float v){
		float x = (R + r*cos(u))*cos(v);
		float y = r*sin(u);
		float z = (R + r*cos(u))*sin(v);

		return Vector(center.x + y, center.y + x, center.z + z);
	}

	void initQuadList(){
		int i = 0;
		for (int u = 0; u < rTes; u++){
			for (int v = 0; v < RTes; v++){
				Vector p0 = rv(2 * u * M_PI / rTes, 2 * v * M_PI / RTes);
				Vector p1 = rv(2 * (u + 1) * M_PI / rTes, 2 * v * M_PI / RTes);
				Vector p2 = rv(2 * (u + 1) * M_PI / rTes, 2 * (v + 1) * M_PI / RTes);
				Vector p3 = rv(2 * u * M_PI / rTes, 2 * (v + 1) * M_PI / RTes);
		
				if (i < rTes*RTes){
					quadList[i][0] = p0;
					quadList[i][1] = p1;
					quadList[i][2] = p2;
					quadList[i][3] = p3;
					i++;
				}
			}
		}
	}

public:
	Torus(Vector _center, float _radius, float _tRadius){
		center = _center;
		R = _radius;
		r = _tRadius;
		material = new Material();
		initQuadList();
	}

	Hit intersect(const Ray& ray, float step){
		Hit theHit;
		
		Sphere around = Sphere(center, (R+r+step));
		if (!around.in(ray)) return theHit;

		for (int i = 0; i < 20; i++){
			
			Vector p0 = quadList[i][0];
			Vector p1 = quadList[i][1];
			Vector p2 = quadList[i][2];
			Vector p3 = quadList[i][3];

			Vector normal = ((p1 - p0) % (p3 - p0)).normalize();

			float fDir = dot(normal, ray.dir);

			if (fabs(fDir) < eplsilon)  continue;

			float t = dot(normal, p0 - ray.pos) / fDir;

			if (t>0 &&(!theHit.valid || t < theHit.t)){
				Vector p = ray.pos + ray.dir*t;

				if (dot(((p1 - p0) % (p - p0)), normal) > 0 && dot(((p2 - p1) % (p - p1)), normal) > 0 && dot(((p3 - p2) % (p - p2)), normal) > 0 && dot(((p0 - p3) % (p - p3)), normal) > 0){
					theHit.material = material;
					theHit.position = p;
					theHit.t = t;
					theHit.valid = true;
					theHit.normal = normal*(-1.0f);
				}
			}
		}
		return theHit;
	}
};

class Plane : public Intersectable{
	Vector point, normal;
	Material * texture, * dark;
public:
	Plane(Vector _point, Vector _normal, Color _kd){
		point = _point;
		normal = _normal;
		material = new Material(_kd,280);						
		texture = new Material(_kd - Color(0.2f, 0.2f, 0.2f), 280);	
		dark = new Material(_kd + Color(0.3, 0.02, 0.02),280);
	}


	Hit intersect(const Ray& ray, float t){						
		Hit theHit;
		float d = dot(normal, ray.dir);		
		if (fabs(d) > eplsilon){
			theHit.t = dot(normal, point - ray.pos) / d;		
			if (theHit.t < 0 || theHit.t > 2 * t) return theHit;	
			theHit.normal = normal;
			theHit.position = ray.pos + ray.dir * theHit.t;
			
			if ((int)theHit.position.x % 6 == 0 && theHit.position.x <30 && theHit.position.x>-30){
				theHit.material = dark;
			}
			else if ((int)theHit.position.Length() % 2 == 0){			
				theHit.material = material;
			}
			else{
				theHit.material = texture;
			}
			theHit.valid = true;
			
		}
		return theHit;
	}
};



class Light{
public:
	Vector pos;
	Color Li;

	Light(Vector _pos, Color _Li){
		pos = _pos;
		Li = _Li;
	}

	Vector getDir(Vector p){
		return (pos - p).normalize();
	}

	Color getRad(Vector p){
		return Li * (1.0f / dot(pos - p, pos - p));

	}

};

const long meterKerulet = 40000 * 1000;
const long meterFenySebesseg = 300000 * 1000;
const int gyorsulas = 10;

class Scene{
	
	Intersectable *obj[100];
	Camera camera;
	Light *feny[100];
	Color La;
	int numObj, numL;

	float swaRRrrrr;

public:
	Sphere *blackHole;

	Scene(){
		float Gm = gyorsulas * powf(meterKerulet / (2 * M_PI), 2);
		swaRRrrrr = 2 * Gm / powf(meterFenySebesseg, 2)*1000;

		init();
	}

	void init(){

		camera.initCamera(Vector(9, 11, -50), Vector(0, 0, 0), Vector(0, 1, 0));
		blackHole = new Sphere(Vector(7, 8, -5), swaRRrrrr * 60 / 100);

		obj[0] = new Plane(Vector(0, 0, 30), Vector(0, 0, -1), Color(0.3, 0, 0));
		obj[1] = new Plane(Vector(0, -30, 0), Vector(0, 1, 0), Color(0, 0.3, 0));
		obj[2] = new Plane(Vector(-30, 0, 0), Vector(1, 0, 0), Color(0.2, 0.1, 0.1));
		obj[3] = new Plane(Vector(0, 30, 0), Vector(0, -1, 0), Color(0.15, 0, 0.2));
		obj[4] = new Plane(Vector(30, 0, 0), Vector(-1, 0, 0), Color(0.2, 0.2, 0));
		obj[5] = new Torus(Vector(-26, -5, 10), 15, 4);
		obj[6] = blackHole;
		numObj = 7;

		feny[0] = new Light(Vector(20, 20, 5), Color(18000, 18000, 18000));
		La = Color(0.1, 0.1, 0.1);
		numL = 1;
	}

	Color trace(Ray ray, int depth) {
		if (depth > recursionDepth) return La;
		Hit hit = firstIntersect(ray);
		if (hit.t < 0) return Color(0,0,0);
		Color outRadiance(0, 0, 0);
		if (!hit.blackHole) outRadiance = La;
		Vector x = hit.position, N = hit.normal, V = ray.dir *(-1);
		for (int i = 0; i < numL; i++){
			Vector L = feny[i]->getDir(x);
			Color Lin = feny[i]->getRad(x);
			Ray shadowRay(x + N*eplsilon, L);
			if (firstIntersecShadow(shadowRay, (x - feny[i]->pos).Length()))
				outRadiance = outRadiance + hit.material->shade(N, V, L, Lin);
			else
				outRadiance = outRadiance + hit.material->shade(N, V, L, Lin)*Color(0.1,0.1,0.1);

		}
		if (hit.material->reflective){
			Vector reflectionDir = hit.material->reflect(ray.dir, N);
			Ray reflectedRay(x + N*eplsilon, reflectionDir);
			outRadiance = outRadiance + trace(reflectedRay, depth + 1)*(hit.material->Fresnel(V, N));
		}

		return outRadiance;
	}

	Hit firstIntersect(Ray ray) {
		Hit bestHit;
	
		for (int i = 0; i < 1234; i++){
			Vector holeDir = blackHole->center - ray.pos;
			float rayDist = holeDir.Length();
			float t =  5*sqrtf(rayDist) /( blackHole->radius);

			if (rayDist < blackHole->radius){
				bestHit.blackHole = true;
				bestHit.valid = true;
				return bestHit;
			}

			ray.dir = ray.dir + ray.dir*t + (holeDir*(1/rayDist))*(blackHole->radius/4)*(powf(t/rayDist , 2));
			ray.pos = ray.pos + ray.dir;
			ray.dir = ray.dir.normalize();
			

			for (int i = 0; i < numObj; i++) {
				if (obj[i] == blackHole) continue;

				Hit hit = obj[i]->intersect(ray, t); 
				if (hit.t > eplsilon && hit.t <=  2 * t){
					if (!bestHit.valid || hit.t < bestHit.t) { 
						bestHit = hit;
					}
				}
			}
			if (bestHit.valid) return bestHit;
		}
		return bestHit;
	}


	bool firstIntersecShadow(Ray ray, float tmax){
		for (int i = 0; i < numObj; i++){
			Hit hit = obj[i]->intersect(ray, tmax);
			if (hit.valid && hit.t > eplsilon && hit.t < tmax) return false;
		}
		return true;

	}

	void render(){
		for (int x = 0; x < screenWidth; x++){
			for (int y = 0; y < screenHeight; y++){
				image[y*screenWidth + x] = trace(camera.getRay(x, y), 0);

			}
		}
	}

}scene;


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
	glViewport(0, 0, screenWidth, screenHeight);

	scene.render();

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
	
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle() {

}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
	glutInit(&argc, argv); 				// GLUT inicializalasa
	glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
	glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

	glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

	glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();

	onInitialization();					// Az altalad irt inicializalast lefuttatjuk

	glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);

	glutMainLoop();					// Esemenykezelo hurok

	return 0;
}
