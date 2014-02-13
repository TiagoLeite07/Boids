//
// Lighthouse3D.com VS*L OpenGL Sample
//
// Loading and displaying a Textured Model
//
// Uses:
//  Assimp 3.0 library for model loading
//		http://assimp.sourceforge.net/
//  Devil for image loading
//		http://openil.sourceforge.net/
//  GLEW for OpenGL post 1.1 functions
//		http://glew.sourceforge.net/
//	TinyXML for font definition parsing
//		http://sourceforge.net/projects/tinyxml/
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//
// If you do use it I would love to hear about it. 
// Just post a comment at Lighthouse3D.com

// Have Fun :-)


#include <math.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>


// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "vslibs.h"

// Our Classes
#include "Boid.h"
#include "Swarm.h"
#include "BWorld.h"

//#include "TinyXML\tinyxml.h"

std::ostream *f;
VSMathLib *vsml;
VSShaderLib shader,shaderTess;
VSFontLib vsfl2, vsfl;
VSResModelLib mark,
			spaceMen,
			woodBox,
			tower,
			friendsModel,
			bird,
			dragon,
			wall;

unsigned int aSentence, profileSentence;

// Query to track the number of primitives
// issued by the tesselation shaders
GLuint counterQ;
unsigned int primitiveCounter = 0;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
long lastFrame = 0, dT;




BWorld grupos("exemploBoids3.xml");
//BWorld grupos("example01.xml");


int w = 800,h = 800;
/* Camera */

// The camera's position
Vec3 camPos = Vec3(10.0,20,5.0);

// The camera's view
Vec3 camView = Vec3(0.0,0.0,-1.0);

// The camera's up vector
Vec3 camUp = Vec3(0.0,1.0,0.0);	
	

// The camera's speed
float walkSpeed	= 1.3f;
float strafeSpeed = 0.7f;	


float angle=0.0,deltaAngle = 0.0;	// camera angle
float deltaMove = 0, deltaStrafe = 0, deltaUp=0;

float lastX = 0.0f,lastY = 0.0f;	// mouse pointer coordinates
bool fpsCam = true;
#define FPSCAM 0
#define GODSCAM 1

bool fullscreen = false; 

void orientMe(float ang) {
	camView.x = sin(ang);
	camView.z = -cos(ang);
}


void moveMeFlat(float i) {
	float px = camPos.x + i*(camView.x);
	float pz = camPos.z + i*(camView.z);
	camPos.x = px;
	camPos.z = pz;
}

void moveMeUp(float i) {
	float py = camPos.y + i*(camView.y);
	camPos.y = py;
}

void strafeMeFlat(float i) {
	float px = camPos.x + i*(cos(angle));
	float pz = camPos.z + i*(sin(angle));
	camPos.x = px;
	camPos.z = pz;
}

void Lookup(float i) {
	camView.y +=i;
}

void initBoids()
{
	//grupos.loadXML("testFriends.xml");
	//grupos.AddSwarm(5,10.0,1);
	//grupos.AddSwarm(5,3.0,2);
}





// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	ratio = (1.0f * w) / h;
	vsml->loadIdentity(VSMathLib::PROJECTION);
	vsml->perspective(53.13f, ratio, 0.1f, 10000.0f);
}


// ------------------------------------------------------------
//
// Render stuff
//
float x =1;
void renderScene(void) {

	if (deltaMove){
		moveMeFlat(deltaMove);
		if(!fpsCam){
			moveMeUp(deltaMove);
		}
	}
	if (deltaStrafe)
		strafeMeFlat(deltaStrafe);
	if (deltaUp)
		Lookup(deltaUp);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	// Debug
	//printf("angle-%f\n",angle);
	deltaUp=0;	// Reset dos angulos
	deltaAngle=0;

	{
		PROFILE("Frame");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set both matrices to the identity matrix
		vsml->loadIdentity(VSMathLib::VIEW);
		vsml->loadIdentity(VSMathLib::MODEL);

		// set camera
		Vec3 viewAux(camPos.x+camView.x,camPos.y + camView.y,camPos.z + camView.z);

		vsml->lookAt(camPos.x,camPos.y,camPos.z,
		viewAux.x,viewAux.y,viewAux.z,
		camUp.x,camUp.y,camUp.z);

		{
			PROFILE_GL("Render models");

			// set the shader to render models
			glUseProgram(shader.getProgramIndex());
			
			// start counting primitives
			glBeginQuery(GL_PRIMITIVES_GENERATED, counterQ);
			// render array of models

					
			glClearColor(0.0f, 0.3f, 0.5f, 1.0f);



			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 100, 0, 0);
			vsml->scale(150,150,150);
			wall.render();
			vsml->popMatrix(VSMathLib::MODEL);

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, -100, 0, 0);
			vsml->scale(150,150,150);
			wall.render();
			vsml->popMatrix(VSMathLib::MODEL);

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 0, 0, 100);
			vsml->rotate(90,0,1,0);
			vsml->scale(150,150,150);
			wall.render();
			vsml->popMatrix(VSMathLib::MODEL);

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 0, 0, -100);
			vsml->rotate(90,0,1,0);
			vsml->scale(150,150,150);
			wall.render();
			vsml->popMatrix(VSMathLib::MODEL);



			// Draw obstacles (like in xml file)
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 0, 0, 0);
			vsml->scale(30, 30, 30);
			tower.render();
			vsml->popMatrix(VSMathLib::MODEL);

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 20, 0, 35);
			vsml->scale(10, 10, 10);
			friendsModel.render();
			vsml->popMatrix(VSMathLib::MODEL);

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 35, 0, 10);
			vsml->scale(10, 10, 10);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			
/*
			// Draw marks
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 35, 0, 35);
			mark.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, -35, 0, -35);
			mark.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, -35, 0, 35);
			mark.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 35, 0, -35);
			mark.render();
			vsml->popMatrix(VSMathLib::MODEL);
*/
			// Draw marks
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 35, -30, 35);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, -35, -30, -35);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, -35, -30, 35);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 35, -30, -35);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			
			
			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(VSMathLib::MODEL, 0, -30, 0);		
			vsml->scale(1,1,1);
			woodBox.render();
			vsml->popMatrix(VSMathLib::MODEL);
			
			// BOIDS	
			int group = 0;
			int boidsCount = 0;

			std::vector<Swarm*> listAux = grupos.getSwarms();

			for each (Swarm* grupo in listAux)
			{
				std::vector<Boid*> listBAux = grupo->getBoids();

				for each (Boid* boid in listBAux)
				{
				
					vsml->pushMatrix(VSMathLib::MODEL);
					vsml->translate(VSMathLib::MODEL, boid->getPosition().x, boid->getPosition().y, boid->getPosition().z);
					vsml->rotate(boid->getAngle(),0,1,0);
				
					if(boid->closeToFriends!=0)
					{
						friendsModel.render();
					}
					else if(grupo->getId().find("mark")!=std::string::npos)
					{
						vsml->scale(2,2,2);
						mark.render();
					}
					else if(grupo->getId().find("people")!=std::string::npos)
					{
						vsml->scale(2,2,2);
						spaceMen.render();
					}
					else if(grupo->getId().find("bird")!=std::string::npos)
					{
						vsml->scale(2,2,2);
						//vsml->rotate(90,1,0,0);
						bird.render();
					}
					else if(grupo->getId().find("dragon")!=std::string::npos)
					{
						vsml->scale(5,5,5);
						vsml->rotate(90,1,0,0);
						dragon.render();
					}
					else
						woodBox.render();

					vsml->popMatrix(VSMathLib::MODEL);
					/*
					for(string value : boid->getProperties())
					{
						printf("prop=%s\n",value.c_str());
					}*/
					//printf("%s\n", boid->getProperty(string("model")).c_str());					
					//Vec3 pos = boid->getPosition();
					//printf("x=%f,y=%f,z=%f\n",pos.x,pos.y,pos.z);
				}

			}

			// pode fazer-se update individualmente dentro do ciclo ao invés de todos os grupos de uma só vez
			grupos.Update();

			// stop counting primitives
			glEndQuery(GL_PRIMITIVES_GENERATED);
		}

		
		// FPS computation and display
		frame++;
		myTime=glutGet(GLUT_ELAPSED_TIME);
		if (myTime - timebase > 1000) {
				sprintf(s,"FPS:%4.2f  Counter: %d",
					frame*1000.0/(myTime-timebase) , primitiveCounter);
			timebase = myTime;
			frame = 0;
			vsfl.prepareSentence(aSentence,s);
			glutSetWindowTitle(s);
		}

		// Display text info
		{
			PROFILE("Dump");
			// prepare sentence with profile info
			std::string s = VSProfileLib::DumpLevels();
			vsfl.prepareSentence(profileSentence, s);
			//set the shader for rendering the sentence
			glUseProgram(shader.getProgramIndex());
			// render sentences
			vsfl.renderSentence(10,10,aSentence);
			vsfl.renderSentence(10, 30, profileSentence);

		}
		 //swap buffers
		{
			PROFILE("Swap");
			glutSwapBuffers();
		}
	} // end PROFILE("Frame")
	{
		PROFILE("Collect GL Queries Time");
		VSProfileLib::CollectQueryResults();
		glGetQueryObjectuiv(counterQ, GL_QUERY_RESULT, &primitiveCounter);
	}
}


// ------------------------------------------------------------
//
// Events from the Keyboard and Mouse
//

void mouseHandler(int x, int y)		// (0,0)->canto superior esquerdo
{
	if(x!=lastX||y!=lastY)
	{
		deltaAngle = (lastX - x)/500; 

		deltaUp = (y - lastY)/500; 

		lastY=y;
		lastX=x;
		glutWarpPointer(h/2, w/2);
		glutPostRedisplay();

	}

}


/** Input **/
void keyHandler(unsigned char tecla, int x, int y) {

	

	switch(tecla){

		// Movement
        case 'w' :  deltaMove = walkSpeed;
                    break;
        case 's' :  deltaMove = -walkSpeed;
                    break;
        case 'd' :  deltaStrafe = strafeSpeed;
                    break;
        case 'a' :	deltaStrafe = -strafeSpeed;
                    break;
		case 'g' :	fpsCam = !fpsCam;
                    break;
		case 'e' :	camPos.y += 0.7;
                    break;
		case 'q' :	camPos.y -= 0.7;
                    break;
		case '+' :  {walkSpeed+=0.1;strafeSpeed+=0.1;}
                    break;
		case '-' :  {walkSpeed-=0.1;strafeSpeed-=0.1;}
                    break;
		case 'f' : if(!fullscreen) { glutFullScreen(); fullscreen = true; }
				   else { glutLeaveFullScreen(); fullscreen = false; }
				   break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
		case 'k': VSProfileLib::Reset(); break;
		case 'p': {std::string s = VSProfileLib::DumpLevels();
			printf("%s\n", s.c_str());}
				break;

		default  :  break;
	}

	if (tecla==27) exit(0);
	if (tecla==32) camPos.y=60;

	glutPostRedisplay();

}


void releaseKeyHandler(unsigned char key, int x, int y) {

	switch (key) {
		case 'a' : 
		case 'd' : deltaStrafe = 0.0f;break;
		case 'w' : 
		case 's' : deltaMove = 0;break;
	}

	glutPostRedisplay();

}


// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for fonts and models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/dirlightdiffambpix.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/dirlightdiffambpix.frag");

	// set semantics for the shader variables
	shader.setProgramOutput(0,"outputF");
	shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
	shader.setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "texCoord");
	shader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");

	shader.prepareProgram();

	VSGLInfoLib::getProgramInfo(shader.getProgramIndex());
	printf("%s\n", shader.getAllInfoLogs().c_str());
	// set sampler uniform
	shader.setUniform("texUnit", 0);

	return(1);
}



// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//




int init()
{

	// load models
	if (friendsModel.load("models/golf_ball.obj")) {

		//printf("%s\n",friendsModel.getInfo().c_str());

		// set the uniform block name for the models uniforms
		friendsModel.setMaterialBlockName("Material");
		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}
	else {
		printf("%s\n",friendsModel.getErrors().c_str());
		return false;
	}
	
	if (bird.load("models/Dove/DOVE.OBJ")) {

		//printf("%s\n",bird.getInfo().c_str());

		// set the uniform block name for the models uniforms
		bird.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}
	else {
		printf("%s\n",bird.getErrors().c_str());
		return false;
	}

	if (dragon.load("models/Dragon/Gringotts Dragon.obj")) {

		//printf("%s\n",dragon.getInfo().c_str());

		// set the uniform block name for the models uniforms
		dragon.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}
	else {
		printf("%s\n",dragon.getErrors().c_str());
		return false;
	}


	if (wall.load("models/wall.obj")) {

		//printf("%s\n",wall.getInfo().c_str());

		// set the uniform block name for the models uniforms
		wall.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}
	else {
		printf("%s\n",wall.getErrors().c_str());
		return false;
	}

	if (spaceMen.load("models/SpaceSuit/Space_suit_male.obj")) {

		//printf("%s\n",spaceMen.getInfo().c_str());

		// set the uniform block name for the models uniforms
		spaceMen.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}

	if (woodBox.load("models/Box/WoodenBox02.obj")) {

		//printf("%s\n",woodBox.getInfo().c_str());

		// set the uniform block name for the models uniforms
		woodBox.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}

	if (tower.load("models/LibertyStatue/LibertStatue.obj")) {

		//printf("%s\n",tower.getInfo().c_str());

		// set the uniform block name for the models uniforms
		tower.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}


	if (mark.load("models/carro.obj")) {

		//printf("%s\n",tower.getInfo().c_str());

		// set the uniform block name for the models uniforms
		mark.setMaterialBlockName("Material");

		// some GL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate a query to count primitives
		glGenQueries(1,&counterQ);

		//return true;
	}

	return true;

}


void initVSL() {

	// Init VSML
	vsml = VSMathLib::getInstance();
	vsml->setUniformBlockName("Matrices");
	vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "projViewModelMatrix");
	vsml->setUniformName(VSMathLib::NORMAL, "normalMatrix");

	// Init VSFL Fonts
	vsfl.load("fonts/couriernew10");
	vsfl.setFixedFont(true);
	vsfl.setMaterialBlockName("Material");
	aSentence = vsfl.genSentence();
	profileSentence = vsfl.genSentence();
}

// ------------------------------------------------------------
//
// Main function
//



int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	// Set context
	glutInitContextVersion (2, 0);
	glutInitContextProfile (GLUT_CORE_PROFILE );

	glutInitWindowPosition(100,100);
	glutInitWindowSize(w,h);
	glutCreateWindow("Animação de Personagens Virtuais - Boids");


//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(keyHandler);
	glutKeyboardUpFunc(releaseKeyHandler);
	glutPassiveMotionFunc(mouseHandler);
	glutSetCursor(GLUT_CURSOR_NONE);

//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
		printf("OpenGL 4.2 not supported\n");
		exit(1);
	}

	VSGLInfoLib::getGeneralInfo();

	setupShaders();
	if (!init())
		printf("Could not Load the Model\n");
	initVSL();


	initBoids();


	//  GLUT main loop
	glutMainLoop();

	return(1);

}

