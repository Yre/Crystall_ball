#include "../lib/SnowSence.h"

#include <cmath>


SnowSence::SnowSence(){}
SnowSence::SnowSence(GLuint number,GLfloat size,GLfloat rage,GLfloat range){
	numberOfFlack = number;
	maxSize = size;
	rageLevel = rage;
	snowRange = range;
	acceleration = 0.0000005;
	generate();
}


void SnowSence::generate(){
	snowArray.resize(numberOfFlack);
	for (int i = 0; i < numberOfFlack; i++){
		snowArray[i].size = maxSize * ((double) rand() / (RAND_MAX));
		GLfloat theta = 2 * glm::pi<double>() * ((double) rand() / (RAND_MAX)); //0 ~ 2pi
		GLfloat fai = glm::pi<double>() * (((double) rand() / (RAND_MAX))-0.5); // -pi/2 ~ pi/2
		snowArray[i].posX = snowRange * cos(theta) * cos(fai) * 0.9;
		snowArray[i].posY = snowRange * sin(theta) * cos(fai) * 0.9;
		snowArray[i].posZ = snowRange * sin(fai) * 0.9;

		snowArray[i].speed = 0;//sqrt(2 * acceleration * snowArray[i].posY)*0.0000001;
	}
	glGenBuffers(1, &SVBO);
	glGenVertexArrays(1, &VertexArrayID);


}

void SnowSence::show(GLuint center){

	glBindVertexArray(VertexArrayID);

	for (int i = 0; i < numberOfFlack; i++){
		drawCube(snowArray[i],center);
		snowArray[i].posY += snowArray[i].speed;
		// if (outRange(snowArray[i])){
		// 	snowArray[i].speed = -snowArray[i].speed;
		// }
		snowArray[i].speed += acceleration;
		

	}
}

void SnowSence::drawCube(snowflack flack,GLuint center){
	GLfloat vertex_buffer[] = {
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};
	
	for (int i=0;i<36;i++){
		vertex_buffer[i*6]   = vertex_buffer[i*6]  *flack.size + flack.posX;
		vertex_buffer[i*6+1] = vertex_buffer[i*6+1]*flack.size + flack.posY;
		vertex_buffer[i*6+2] = vertex_buffer[i*6+2]*flack.size + flack.posZ;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, SVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);

    glBindVertexArray(VertexArrayID);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(1);
    

    
    glUniform3f(center, flack.posX, flack.posY,flack.posZ);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisable(GL_BLEND);
    glBindVertexArray(0);


}



