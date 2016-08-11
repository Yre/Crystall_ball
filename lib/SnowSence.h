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
	GLfloat size;
	//GLfloat posX,posY,posZ;
	glm::vec3 pos;
	glm::vec3 speed;
};

class SnowSence{
public:
	SnowSence();
	SnowSence(GLuint number, GLfloat size, GLfloat rage, GLfloat range);
	GLuint numberOfFlack;
	GLfloat maxSize;
	GLfloat rageLevel;
	GLfloat snowRange;
	glm::vec3 acceleration;

	vector<snowflack> snowArray;
	
	void show(GLuint,GLuint);
	void setMode(GLuint);

private:
	GLuint mode;
	GLuint SVBO;
	GLuint VertexArrayID;
	bool outRange(snowflack);
	void drawCube(snowflack,GLuint,GLuint);
	void generate();
	glm::vec3 windAccelerate(glm::vec3);
};

#endif