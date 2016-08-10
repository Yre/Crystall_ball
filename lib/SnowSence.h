#ifndef SNOWSENCE_H
#define SNOWSENCE_H

#include <vector>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS // force everything in radian
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct snowflack{
	GLuint size;
	GLfloat posX,posY,posZ;
	GLfloat speed;
};

class SnowSence{
public:
	SnowSence();
	SnowSence(GLuint number, GLfloat size, GLfloat rage, GLfloat range);
	GLuint numberOfFlack;
	GLfloat maxSize;
	GLfloat rageLevel;
	GLfloat snowRange;
	GLfloat acceleration;



	
	vector<snowflack> snowArray;
	void generate();
	void show();
	void drawCube(snowflack);
private:
	GLuint SVBO;
	GLuint VertexArrayID;
};

#endif