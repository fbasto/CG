#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <string.h>
#include <map>
#include <list>

/*#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>*/

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

char* cubeColors[6][9] = {
	{ "red", "white", "white", "white", "white", "white", "red", "green", "blue" },
	{ "green", "red", "white", "white", "green", "yellow", "blue", "blue", "green" },
	{ "orange", "red", "yellow", "yellow", "orange", "orange", "yellow", "red", "yellow" },
	{ "red", "orange", "orange", "green", "red", "red", "white", "orange", "white" },
	{ "green", "orange", "red", "blue", "blue", "blue", "orange", "yellow", "green" },
	{ "blue", "green", "orange", "blue", "yellow", "green", "yellow", "yellow", "blue" } };

static std::list<int> lista_rotacoes;

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   xC = 10.0, yC = 10.0, zC = 20.0;
GLint     wScreen = 700, hScreen = 600;
GLfloat   mesa = 1.5;				// dimensao da mesa
GLfloat   bule = 0.5;				// dimensao da chaleira
GLfloat   quad = 3.0;				// dimensao do quadro
GLfloat   mesaP[] = { 3, 0, 3 };	// posicao da mesa
GLfloat   buleP[] = { 3,0,3 };		//posicao do bule
GLfloat  rVisao = 3.0, aVisao = 0.5*PI, incVisao = 0.1;
GLfloat  obsPini[] = { 0, 0, 0.5*xC };
GLfloat  obsPfin[] = { obsPini[0] - rVisao*cos(aVisao), obsPini[1], obsPini[2] - rVisao*sin(aVisao) };

GLfloat   rcube_size = 0.75;				// dimensao da rcube_size
GLfloat   rcubeP[] = { 10, 0, 10 };	// posicao da rcube_size
double rotate_y = 0;
double rotate_x = 0;

//------------------------------------------------------------ Texturas e Rotacao
GLfloat   quadS = 4.0;
GLfloat   quadP[] = { -2, 0, -8 };

//------------------------------------------------------------ NOVO - Nevoeiro
GLfloat nevoeiroCor[] = { 0.75, 0.75, 0.75, 1.0 }; //definicao da cor do nevoeiro
												   /*skybox texturas*/
GLuint textures[6];
float brightness = 0.8f;
//------------------------------------------------------------ Iluminacao
//------------------------------------------------------------ Global (ambiente)
GLint   noite = 0;
GLfloat luzGlobalCor[4] = { 1.0,1.0,1.0,1.0 };   //
												 //Lampada tecto (ambiente local)

GLint   ligaLuz = 0;
GLfloat localCor[4] = { 0.1,0.1,0.1,1.0 };
GLfloat localCorDif[4] = { 1, 1, 1, 1.0 };
GLfloat localPos[4] = { xC / 2, 10.0, xC / 2, 1.0 };
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
void init(void);
void initLights(void);


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

static void cbMainMenu(int value)
{
	if (value == 98) {

	}
	if (value == 99) {
		exit(0);
	}
}

static void cbRotationsMenu(int value)
{
	if (value == 90) {
		lista_rotacoes.push_front(0);
		glutPostRedisplay();
	}
	if (value == 91) {
		lista_rotacoes.push_front(1);
		glutPostRedisplay();
	}
	if (value == 92) {
		lista_rotacoes.push_front(2);
		glutPostRedisplay();
	}
	if (value == 93) {
		lista_rotacoes.push_front(3);
		glutPostRedisplay();
	}
	if (value == 94) {
		lista_rotacoes.push_front(4);
		glutPostRedisplay();
	}
	if (value == 95) {
		lista_rotacoes.push_front(5);
		glutPostRedisplay();
	}
	if (value == 96) {
		lista_rotacoes.push_front(6);
		glutPostRedisplay();
	}
	if (value == 97) {
		lista_rotacoes.push_front(7);
		glutPostRedisplay();
	}
	if (value == 98) {
		lista_rotacoes.push_front(8);
		glutPostRedisplay();
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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

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
	glutAddMenuEntry("Quit", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void glColor4fCustom(double clr[]) {
	if (clr == blank) {
		glColor4f(0, 0, 0, 0);
	}
	else {
		glColor4f(clr[0], clr[1], clr[2], 1);
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

	// Face 0
	glBegin(GL_POLYGON);
	if (faceColor.find(0) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[0]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glEnd();

	// Face 2
	glBegin(GL_POLYGON);
	if (faceColor.find(2) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[2]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glEnd();

	// Face 1
	glBegin(GL_POLYGON);
	if (faceColor.find(1) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[1]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glEnd();

	// Face 3
	glBegin(GL_POLYGON);
	if (faceColor.find(3) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[3]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glEnd();

	// Face 5
	glBegin(GL_POLYGON);
	if (faceColor.find(5) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[5]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX - v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX - v_const, pY + v_const, pZ - v_const);
	glEnd();

	// Face 4
	glBegin(GL_POLYGON);
	if (faceColor.find(4) != faceColor.end()) {
		glColor4fCustom(stringToColor(faceColor[4]));
	}
	else {
		glColor4fCustom(stringToColor("blank"));
	}
	glVertex3f(pX + v_const, pY - v_const, pZ + v_const);
	glVertex3f(pX + v_const, pY - v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ - v_const);
	glVertex3f(pX + v_const, pY + v_const, pZ + v_const);
	glEnd();

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
	printf("Init Cube #%d at(%d,%d,%d)\n", cubeNumber, cubeCoordinates[cubeNumber][0], cubeCoordinates[cubeNumber][1], cubeCoordinates[cubeNumber][2]);
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
	printf("Final Cube #%d at(%d,%d,%d)\n", cubeNumber, cubeCoordinates[cubeNumber][0], cubeCoordinates[cubeNumber][1], cubeCoordinates[cubeNumber][2]);
	//faceColor = aplicarRotacoesCores(posX, posY, posZ,faceColor);
	createCube(posX, posY, posZ, faceColor);
	glTranslatef(rcubeP[0], rcubeP[1], rcubeP[2]);
	glPopMatrix();
}

void drawScene() {
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chaleira
	glPushMatrix();
	glColor4f(LARANJA);
	glTranslatef(buleP[0], buleP[1], buleP[2]);
	glutSolidTeapot(bule);
	glPopMatrix();

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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixos
	glColor4f(BLACK);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
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
	glViewport(wScreen / 4, hScreen / 4, 0.75*wScreen, 0.75*hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(99.0, wScreen / hScreen, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsPini[0], obsPini[1] + rcube_size, obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
	iluminacao();
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
		if (linear)
			glFogi(GL_FOG_MODE, GL_LINEAR);
		else
			glFogi(GL_FOG_MODE, GL_EXP2);
		glutPostRedisplay();
		break;

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
