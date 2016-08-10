#ifndef SNOWSENCE_H
#define SNOWSENCE_H

#include <OpenGL/gl.h>
#include <vector>
using namespace std;

struct snowflack{
	GLuint size;
	GLfloat posX,posY,posZ;
	GLfloat speed;
};

class SnowSence{
public:
	SnowSence(GLuint numbOfFlack, GLfloat maxSize, GLfloat rageLevel, GLfloat range);
	GLuint numbOfFlack;
	GLfloat maxSize;
	GLfloat rageLevel;
	GLfloat snowRange;
	GLfloat acceleration;



	
	vector<snowflack> snowArray;
	void generate();
	void show();
private:
	GLuint SVBO;
	GLuint VertexArrayID;
};

#endif