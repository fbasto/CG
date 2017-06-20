#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <string.h>
#include <map>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <map>
#include <list>
#include "RgbImage.h"

//#include <GL/glut.h>*/

//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI 3.14159
#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.93, 0.93, 0.93, 1.0
//================================================================================
#define TIMER	750
#define GL_CLAMP_TO_EDGE 0x812F // não existe nas versões mais recentes do openGL
//===========================================================Variaveis e constantes

// Cores para o cubo rubik
static double white[3] = { 1, 1, 1 },
red[3] = { 1 ,0, 0 },
blue[3] = { 0, 0, 1 },
orange[3] = { 1, 0.5, 0 },
green[3] = { 0, 1, 0 },
yellow[3] = { 1, 1, 0 },
black[3] = { 0, 0, 0 },
purple[3] = { 0.5, 0, 0.5 },
blank[3] = { 0, 0, 0 };

int cubeCoordinates[27][3];

static char* cubeRandomized[6][9] = {
	{ "red", "white", "white", "white", "white", "white", "red", "green", "blue" },
	{ "green", "red", "white", "white", "green", "yellow", "blue", "blue", "green" },
	{ "orange", "red", "yellow", "yellow", "orange", "orange", "yellow", "red", "yellow" },
	{ "red", "orange", "orange", "green", "red", "red", "white", "orange", "white" },
	{ "green", "orange", "red", "blue", "blue", "blue", "orange", "yellow", "green" },
	{ "blue", "green", "orange", "blue", "yellow", "green", "yellow", "yellow", "blue" } };

static char* cubeSolved[6][9] = {
	{ "red",  "red",  "red",  "red",  "red",  "red",  "red",  "red",  "red" },
	{ "green", "green" ,"green" ,"green" ,"green" ,"green" ,"green" ,"green" ,"green" },
	{ "orange", "orange" ,"orange" ,"orange" ,"orange" ,"orange" ,"orange" ,"orange" ,"orange" },
	{ "blue", "blue" ,"blue" ,"blue" ,"blue" ,"blue" ,"blue" ,"blue" ,"blue" },
	{ "yellow", "yellow","yellow","yellow","yellow","yellow","yellow","yellow","yellow" },
	{ "white", "white","white","white","white","white","white","white","white" } };
char* cubeColors[6][9];

static std::list<int> lista_rotacoes;

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   xC = 10.0, yC = 10.0, zC = 20.0;
GLint     wScreen = 700, hScreen = 600;
GLfloat   bule = 0.5;				// dimensao da chaleira
GLfloat   mesaP[] = { 3, 0, 3 };	// posicao da mesa
GLfloat   buleP[] = { 1, bule, 1 };		//posicao do bule
GLfloat  rVisao = 3.0, aVisao = 0.5*PI, incVisao = 0.1;
GLfloat  obsPini[] = { 0, 0, 0.5*xC };
GLfloat  obsPfin[] = { obsPini[0] - rVisao*cos(aVisao), obsPini[1], obsPini[2] - rVisao*sin(aVisao) };

GLfloat   rcube_size = 0.5;				// dimensao da rcube_size
static GLfloat   rcubeP[] = { 2, rcube_size/2, 2 };	// posicao do rcube
double rotate_y = 0;
double rotate_x = 0;
static int rotation_value;
static bool randomSolved = true; // Random = true || Solved = falsed
static bool transparentZ = false; // Transparente se for true

//------------------------------------------------------------ Texturas e Rotacao
GLfloat   quadS = 4.0;
GLfloat   quadP[] = { -2, 0, -8 };

//------------------------------------------------------------ Nevoeiro
GLfloat nevoeiroCor[] = { 0.75, 0.75, 0.75, 1.0 };


/*skybox texturas*/
GLuint textures[6];
float brightness = 0.8f;
RgbImage imag;


//------------------------------------------------------------ Iluminacao
//------------------------------------------------------------ Global (ambiente)
GLint   noite = 0;
GLfloat luzGlobalCor[4] = { 1.0,1.0,1.0,1.0 };   //
												 //Lampada tecto (ambiente local)

GLint   ligaLuz = 0;
GLfloat localCor[4] = { 0.0,0.0,0.0,1.0 };
GLfloat localCorDif[4] = { 1, 1, 1, 1.0 };
GLfloat localPos[4] = { xC / 2, 25.0, xC / 2, 1.0 };
GLfloat localAttCon = 1.0;
GLfloat localAttLin = 0.05;
GLfloat localAttQua = 0.0;

//Lanterna (local)
GLint   ligaFoco = 0;
GLfloat rFoco = 1.1, aFoco = aVisao;
GLfloat incH = 0.0, incV = 0.0;
GLfloat incMaxH = 0.5, incMaxV = 0.35;
GLfloat focoPini[] = { obsPini[0], obsPini[1], obsPini[2], 1.0 };
GLfloat focoPfin[] = { obsPini[0] - rFoco*cos(aFoco), obsPini[1], obsPini[2] - rFoco*sin(aFoco), 1.0 };
GLfloat focoDir[] = { focoPfin[0] - focoPini[0], 0, focoPfin[2] - focoPini[2] };
GLfloat focoExp = 10.0;
GLfloat focoCut = 15.0;

GLfloat focoCorEsp[4] = { 1.0 ,  1.0, 1.0, 1.0 };
GLfloat focoCorDif[4] = { 0.85, 0.85,0.85, 1.0 };


//Nevoeiro
GLint linear = 1;


GLfloat x = 0;
GLfloat y = 0;

GLfloat width = 20;
GLfloat height = 20;

GLint dim = 64;

void updateVisao();
void init(void);
void drawScene();
void display(void);
void iluminacao();
void initNevoeiro(void);
void initLights(void);
void criarsky();
void sideTextures(char path[50], int i);

void criarsky() {
	int i;
	//char sides[6][20]= {"craterlake_ft.bmp","craterlake_rt.bmp","craterlake_bk.bmp","craterlake_lf.bmp","craterlake_up.bmp","craterlake_dn.bmp"};
	char sides[6][20] = { "craterlake_lf.bmp","craterlake_bk.bmp","craterlake_up.bmp","craterlake_rt.bmp","craterlake_ft.bmp","craterlake_dn.bmp" };
	char tpath[50];

	for (i = 0; i<6; i++) {
		//sprintf(tpath, "skybox/%s", sides[i]); usa-se o sprint_s pq esta funçao está deprecated
		sprintf_s(tpath, "skybox/%s", sides[i]);
		sideTextures(tpath, i);
	}
}
void sideTextures(char path[50], int i) {
	glGenTextures(1, &textures[i]);
	glBindTexture(GL_TEXTURE_2D, textures[i]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile(path);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

//--------------------- NOVO - Definicoes do nevoeiro
void initNevoeiro(void) {
	glFogfv(GL_FOG_COLOR, nevoeiroCor); //Cor do nevoeiro
	glFogi(GL_FOG_MODE, GL_LINEAR); //Equa�cao do nevoeiro - linear
	glFogf(GL_FOG_START, 1.0); // Distancia a que tera' inicio o nevoeiro
	glFogf(GL_FOG_END, 5.0); // Distancia a que o nevoeiro terminara'
	glFogf(GL_FOG_DENSITY, 0.75); //Densidade do nevoeiro - nao se especifica quando temos "nevoeiro linear"
}

void initLights(void) {
	//Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
	//Tecto
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, localAttQua);
	//Foco
	glLightfv(GL_LIGHT1, GL_POSITION, focoPini);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, focoExp);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, focoCut);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, focoCorDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, focoCorEsp);
}

void attributeCubeColors() {
	if (randomSolved == true) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 9; j++) {
				cubeColors[i][j] = cubeRandomized[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 9; j++) {
				cubeColors[i][j] = cubeSolved[i][j];
			}
		}
	}
}
static void timerFly(int value) {
	if (value < 10) {
		rcubeP[1] += 0.2;
		glutPostRedisplay();
		value++;
		glutTimerFunc(TIMER / 7, timerFly, value);
	}
	if (value >= 10 && value < 20) {
		rcubeP[1] -= 0.2;
		glutPostRedisplay();
		value++;
		glutTimerFunc(TIMER / 7, timerFly, value);
	}
}

static void cbMainMenu(int value)
{
	if (value == 87) {
		if (transparentZ == false) {
			transparentZ = true;
		}
		else {
			transparentZ = false;
		}
		drawScene();
		glutPostRedisplay();
	}
	if (value == 88) {
		glutTimerFunc(TIMER, timerFly, 0);
		glutPostRedisplay();
	}
	if (value == 89) {
		if (randomSolved) {
			randomSolved = false;
		}
		else {
			randomSolved = true;
		}
		attributeCubeColors();
		glutPostRedisplay();
	}
	if (value == 99) {
		exit(0);
	}
}

static void timerRotations(int value)
{
	if (value < 4) {
		lista_rotacoes.push_front(rotation_value);
		glutPostRedisplay();
		value++;
		glutTimerFunc(TIMER, timerRotations, value);
	}
}

static void cbRotationsMenu(int value)
{
	if (value == 90) {
		rotation_value = 0;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 91) {
		rotation_value = 1;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 92) {
		rotation_value = 2;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 93) {
		rotation_value = 3;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 94) {
		rotation_value = 4;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 95) {
		rotation_value = 5;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 96) {
		rotation_value = 6;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 97) {
		rotation_value = 7;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
	if (value == 98) {
		rotation_value = 8;
		glutTimerFunc(TIMER, timerRotations, 0);
	}
}

void init(void)
{
	int rotationsMenu, mainMenu;
	glClearColor(WHITE);
	glShadeModel(GL_SMOOTH);

	//NOVO - Activa o nevoeiro
	glEnable(GL_FOG);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	initLights();
	initNevoeiro();
	criarsky();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.



	rotationsMenu = glutCreateMenu(cbRotationsMenu);
	glutAddMenuEntry("Front", 90);
	glutAddMenuEntry("Back", 91);
	glutAddMenuEntry("Up", 92);
	glutAddMenuEntry("Down", 93);
	glutAddMenuEntry("Right", 94);
	glutAddMenuEntry("Left", 95);
	glutAddMenuEntry("Middle", 96);
	glutAddMenuEntry("Equator", 97);
	glutAddMenuEntry("Standing", 98);
	mainMenu = glutCreateMenu(cbMainMenu);
	glutAddSubMenu("Rotate Cube", rotationsMenu);
	glutAddMenuEntry("Fly Cube", 88);
	glutAddMenuEntry("Solve Cube", 89);
	glutAddMenuEntry("Change wall transparency", 87);
	glutAddMenuEntry("Quit", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	attributeCubeColors();

}

void glColor4fCustom(int transparency, double clr[]) {
	if (clr == blank) {
		glColor4f(0, 0, 0, 0);
	}else{
		glColor4f(clr[0], clr[1], clr[2], transparency);
	}
}

double* stringToColor(char clr[]) {
	if (strcmp("white", clr) == 0) {
		return white;
	}
	if (strcmp("red", clr) == 0) {
		return red;
	}
	if (strcmp("blue", clr) == 0) {
		return blue;
	}
	if (strcmp("orange", clr) == 0) {
		return orange;
	}
	if (strcmp("green", clr) == 0) {
		return green;
	}
	if (strcmp("yellow", clr) == 0) {
		return yellow;
	}
	if (strcmp("black", clr) == 0) {
		return black;
	}
	if (strcmp("purple", clr) == 0) {
		return purple;
	}
	if (strcmp("blank", clr) == 0) {
		return blank;
	}
}

void createCube(int posX, int posY, int posZ, std::map<int, char*> faceColor) {
	double v_const = rcube_size / 2;
	double pX = posX * rcube_size;
	double pY = posY * rcube_size;
	double pZ = posZ * rcube_size;

	glPushMatrix();
	glTranslatef(rcubeP[0], rcubeP[1], rcubeP[2]);

	// Face 0
	glBegin(GL_POLYGON);
	if (faceColor.find(0) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[0]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glEnd();

	// Face 2
	glBegin(GL_POLYGON);
	if (faceColor.find(2) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[2]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glEnd();

	// Face 1
	glBegin(GL_POLYGON);
	if (faceColor.find(1) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[1]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glEnd();

	// Face 3
	glBegin(GL_POLYGON);
	if (faceColor.find(3) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[3]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glEnd();

	// Face 5
	glBegin(GL_POLYGON);
	if (faceColor.find(5) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[5]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glEnd();

	// Face 4
	glBegin(GL_POLYGON);
	if (faceColor.find(4) != faceColor.end()) {
		glColor4fCustom(1,stringToColor(faceColor[4]));
	}
	else {
		glColor4fCustom(1,stringToColor("blank"));
	}
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glEnd();
	glPopMatrix();
	glFlush();
}

void spawnCube(int cubeNumber, int posX, int posY, int posZ, int face1, char* clr1, int face2 = NULL, char* clr2 = NULL, int face3 = NULL, char* clr3 = NULL) {
	std::map<int, char*> faceColor;
	faceColor[face1] = clr1;
	if (face2 != NULL) {
		faceColor[face2] = clr2;
	}
	if (face3 != NULL) {
		faceColor[face3] = clr3;
	}
	cubeCoordinates[cubeNumber][0] = posX;
	cubeCoordinates[cubeNumber][1] = posY;
	cubeCoordinates[cubeNumber][2] = posZ;
	glPushMatrix();
	//printf("Init Cube #%d at(%d,%d,%d)\n", cubeNumber, cubeCoordinates[cubeNumber][0], cubeCoordinates[cubeNumber][1], cubeCoordinates[cubeNumber][2]);
	for (std::list<int>::iterator it = lista_rotacoes.begin(); it != lista_rotacoes.end(); it++) {
		if (*it == 0 && cubeCoordinates[cubeNumber][2] == 2) {
			glTranslatef(rcube_size * 2, 0, rcube_size * 2);
			glRotatef(90, 0, 0, 1);
			glTranslatef(0, 0, -rcube_size * 2);
		}
		else if (*it == 1 && cubeCoordinates[cubeNumber][2] == 0) {
			glTranslatef(rcube_size * 2, 0, rcube_size * 2);
			glRotatef(90, 0, 0, 1);
			glTranslatef(0, 0, -rcube_size * 2);
		}
		else if (*it == 2 && cubeCoordinates[cubeNumber][1] == 2) {
			glTranslatef(2 * rcube_size, 2 * rcube_size, 2 * rcube_size);
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, -2 * rcube_size, -2 * rcube_size);
		}
		else if (*it == 3 && cubeCoordinates[cubeNumber][1] == 0) {
			glTranslatef(2 * rcube_size, 2 * rcube_size, 2 * rcube_size);
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, -2 * rcube_size, -2 * rcube_size);
			//adjustHorizontalCoordinates(cubeNumber);
		}
		else if (*it == 4 && cubeCoordinates[cubeNumber][0] == 2) {
			glTranslatef(rcube_size * 2, 0, 0);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-rcube_size * 2, 0, -rcube_size * 2);
			//adjustVerticalCoordinates2(cubeNumber);
		}
		else if (*it == 5 && cubeCoordinates[cubeNumber][0] == 0) {
			glTranslatef(rcube_size * 2, 0, 0);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-rcube_size * 2, 0, -rcube_size * 2);
		}
		else if (*it == 6 && cubeCoordinates[cubeNumber][0] == 1) {
			glTranslatef(rcube_size * 2, 0, 0);
			glRotatef(90, 1, 0, 0);
			glTranslatef(-rcube_size * 2, 0, -rcube_size * 2);
		}
		else if (*it == 7 && cubeCoordinates[cubeNumber][1] == 1) {
			glTranslatef(2 * rcube_size, 2 * rcube_size, 2 * rcube_size);
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, -2 * rcube_size, -2 * rcube_size);
		}
		else if (*it == 8 && cubeCoordinates[cubeNumber][2] == 1) {
			glTranslatef(rcube_size * 2, 0, rcube_size * 2);
			glRotatef(90, 0, 0, 1);
			glTranslatef(0, 0, -rcube_size * 2);
		}
	}
	//printf("Final Cube #%d at(%d,%d,%d)\n", cubeNumber, cubeCoordinates[cubeNumber][0], cubeCoordinates[cubeNumber][1], cubeCoordinates[cubeNumber][2]);
	//faceColor = aplicarRotacoesCores(posX, posY, posZ,faceColor);
	createCube(posX, posY, posZ, faceColor);
	glPopMatrix();
}

void drawSkybox(int d) {
	d /= 2;
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, textures[1]); //right
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -d, -d);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(d, -d, -d);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(d, d, -d);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, d, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[3]); //front
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(d, -d, -d);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(d, -d, d);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(d, d, d);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(d, d, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[4]); //left
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(d, -d, d);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -d, d);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-d, d, d);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(d, d, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[0]); //back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-d, -d, d);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-d, -d, -d);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-d, d, -d);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-d, d, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[2]); //up
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(d, d, -d);
	glTexCoord2f(0.9, 0.0); glVertex3f(d, d, d);
	glTexCoord2f(1.0, 1.0); glVertex3f(-d, d, d);
	glTexCoord2f(0.0, 1.0); glVertex3f(-d, d, -d);


	glTexCoord2f(0.0, 0.0); glVertex3f(d, d, -d);
	glTexCoord2f(1.0, 0.0); glVertex3f(-d, d, d);
	glTexCoord2f(1.0, 1.0); glVertex3f(-d, d, d);
	glTexCoord2f(0.0, 1.0); glVertex3f(-d, d, -d);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Down
	glBindTexture(GL_TEXTURE_2D, textures[5]); //Down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(d, -d, -d);
	glTexCoord2f(1.0, 0.0); glVertex3f(d, -d, d);
	glTexCoord2f(1.0, 1.0); glVertex3f(-d, -d, d);
	glTexCoord2f(0.0, 1.0); glVertex3f(-d, -d, -d);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawScene() {
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Cubo
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ y = 0
	spawnCube(0, 0, 0, 0, 0, cubeColors[0][0], 1, cubeColors[1][6], 2, cubeColors[2][2]);
	spawnCube(1, 0, 0, 1, 0, cubeColors[0][1], 1, cubeColors[1][7]);
	spawnCube(2, 0, 0, 2, 0, cubeColors[0][2], 1, cubeColors[1][8], 3, cubeColors[3][0]);

	spawnCube(3, 1, 0, 0, 0, cubeColors[0][3], 2, cubeColors[2][5]);
	spawnCube(4, 1, 0, 1, 0, cubeColors[0][4]);
	spawnCube(5, 1, 0, 2, 0, cubeColors[0][5], 3, cubeColors[3][3]);

	spawnCube(6, 2, 0, 0, 0, cubeColors[0][6], 2, cubeColors[2][8], 4, cubeColors[4][0]);
	spawnCube(7, 2, 0, 1, 0, cubeColors[0][7], 4, cubeColors[4][1]);
	spawnCube(8, 2, 0, 2, 0, cubeColors[0][8], 3, cubeColors[3][6], 4, cubeColors[4][2]);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ y = 1
	spawnCube(9, 0, 1, 0, 1, cubeColors[1][3], 2, cubeColors[2][1]);
	spawnCube(10, 0, 1, 1, 1, cubeColors[1][4]);
	spawnCube(11, 0, 1, 2, 1, cubeColors[1][5], 3, cubeColors[3][1]);

	spawnCube(12, 1, 1, 0, 2, cubeColors[2][5]);
	// Cubo vazio
	spawnCube(14, 1, 1, 2, 3, cubeColors[3][4]);

	spawnCube(15, 2, 1, 0, 2, cubeColors[2][7], 4, cubeColors[4][3]);
	spawnCube(16, 2, 1, 1, 4, cubeColors[4][4]);
	spawnCube(17, 2, 1, 2, 3, cubeColors[3][7], 4, cubeColors[4][5]);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ y = 2
	spawnCube(18, 0, 2, 0, 1, cubeColors[1][0], 2, cubeColors[2][2], 5, cubeColors[5][6]);
	spawnCube(19, 0, 2, 1, 1, cubeColors[1][1], 5, cubeColors[5][7]);
	spawnCube(20, 0, 2, 2, 1, cubeColors[1][2], 3, cubeColors[3][2], 5, cubeColors[5][8]);

	spawnCube(21, 1, 2, 0, 2, cubeColors[2][3], 5, cubeColors[5][3]);
	spawnCube(22, 1, 2, 1, 5, cubeColors[5][4]);
	spawnCube(23, 1, 2, 2, 3, cubeColors[3][5], 5, cubeColors[5][5]);

	spawnCube(24, 2, 2, 0, 2, cubeColors[2][6], 4, cubeColors[4][6], 5, cubeColors[5][0]);
	spawnCube(25, 2, 2, 1, 4, cubeColors[4][7], 5, cubeColors[5][1]);
	spawnCube(26, 2, 2, 2, 3, cubeColors[3][8], 4, cubeColors[4][8], 5, cubeColors[5][2]);

	//~~~~~~~~~~~~~~Plano X = 0
	glColor4fCustom(1, stringToColor("purple"));
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glVertex3f(0, 2, 4);
	glVertex3f(0, 2, 0);
	glEnd();
	//~~~~~~~~~~~~~~Plano Z = 0
	if (transparentZ == true) {
		glColor4fCustom(0.5, stringToColor("purple"));
		glEnable(GL_BLEND); //Enable blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	}
	else {
		glColor4fCustom(1, stringToColor("purple"));
	}
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);
	glVertex3f(4, 2, 0);
	glVertex3f(0, 2, 0);
	glEnd();
	glDisable(GL_BLEND);
	//~~~~~~~~~~~~~~Plano Y = 0
	glColor4fCustom(1, stringToColor("purple"));
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);
	glVertex3f(4, 0, 4);
	glVertex3f(0, 0, 4);
	glEnd();
	//~~~~~~~~~~~~~~Plano X = 4
	glColor4fCustom(1, stringToColor("purple"));
	glBegin(GL_POLYGON);
	glVertex3f(4, 0, 0);
	glVertex3f(4, 0, 4);
	glVertex3f(4, 2, 4);
	glVertex3f(4, 2, 0);
	glEnd();
}

void iluminacao() {

	if (ligaLuz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if (ligaFoco)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_POSITION, focoPini);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDir);
}


void display(void) {
	if (noite)
		glClearColor(GRAY1);
	else
		glClearColor(GRAY2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Janela Visualizacao ]
	glViewport(0, 0, wScreen, hScreen);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Projeccao]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xC, xC, -yC, yC, -zC, zC);

	glEnable(GL_LIGHTING);
	//glViewport(wScreen / 4, hScreen / 4, 0.75*wScreen, 0.75*hScreen);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(99.0, wScreen / hScreen, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
	iluminacao();
	drawSkybox(100);
	drawScene();
	iluminacao();


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Objectos ]
	drawScene();
	//drawOrientacao();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


GLvoid resize(GLsizei width, GLsizei height)
{
	wScreen = width;
	hScreen = height;
	drawScene();
}


void updateVisao() {
	obsPfin[0] = obsPini[0] + rVisao*cos(aVisao);
	obsPfin[2] = obsPini[2] - rVisao*sin(aVisao);
	focoPini[0] = obsPini[0];
	focoPini[2] = obsPini[2];
	focoPfin[0] = focoPini[0] - rFoco*cos(aFoco + incH);
	focoPfin[2] = focoPini[2] - rFoco*sin(aFoco + incH);
	focoPini[1] = obsPini[1];
	focoPini[2] = obsPini[2];
	focoDir[0] = focoPfin[0] - focoPini[0];
	focoDir[1] = incV;
	focoDir[2] = focoPfin[2] - focoPini[2];
	glutPostRedisplay();

}
//======================================================= EVENTOS
//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//--------------------------- Direccao da Lanterna
	case 's':
	case 'S':
		incH = incH - 0.05;
		if (incH<-incMaxH) incH = -incMaxH;
		updateVisao();
		break;
	case 'd':
	case 'D':
		incH = incH + 0.05;
		if (incH>incMaxH) incH = incMaxH;
		updateVisao();
		break;
	case 'e':
	case 'E':
		incV = incV + 0.05;
		if (incV>incMaxV) incV = incMaxV;
		updateVisao();
		break;
	case 'c':
	case 'C':
		incV = incV - 0.05;
		if (incV<-incMaxV) incV = -incMaxV;
		updateVisao();
		break;
		//--------------------------- Dia/noite
	case 'n':
	case 'N':
		noite = !noite;
		if (noite)
		{
			luzGlobalCor[0] = 0.6;   luzGlobalCor[1] = 0.6;   luzGlobalCor[2] = 0.6;
		}
		else
		{
			luzGlobalCor[0] = 0.3;   luzGlobalCor[1] = 0.3;   luzGlobalCor[2] = 0.3;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
		glutPostRedisplay();
		break;
		//--------------------------- Iluminacaoda sala
	case 't':
	case 'T':
		ligaLuz = !ligaLuz;
		glutPostRedisplay();
		break;
		//--------------------------- Observador
	case 'f':
	case 'F':
		ligaFoco = !ligaFoco;
		glutPostRedisplay();
		break;
	case 'l':
	case 'L':
		linear = !linear;
		if (linear) {
			glFogi(GL_FOG_MODE, GL_LINEAR);
		}
		else
			glFogi(GL_FOG_MODE, GL_EXP2);
		glutPostRedisplay();
		break;
	case 'x':
	case 'X':
		rcubeP[1] += 0.5;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;

	}
}

void teclasNotAscii(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		obsPini[0] = obsPini[0] + incVisao*cos(aVisao);
		obsPini[2] = obsPini[2] - incVisao*sin(aVisao);
	}
	if (key == GLUT_KEY_DOWN) {
		obsPini[0] = obsPini[0] - incVisao*cos(aVisao);
		obsPini[2] = obsPini[2] + incVisao*sin(aVisao);
	}
	if (key == GLUT_KEY_LEFT) {
		aVisao = (aVisao + 0.1);
		aFoco = aFoco - 0.1;
	}
	if (key == GLUT_KEY_RIGHT) {
		aVisao = (aVisao - 0.1);
		aFoco = aFoco + 0.1;
	}
	if (key == GLUT_KEY_F1) {
		obsPini[1] += 0.1;
	}
	if (key == GLUT_KEY_F2) {
		obsPini[1] -= 0.1;
	}
	updateVisao();
}

//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow(" :: CG 2017 - Cubo de Rubik :: ");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
