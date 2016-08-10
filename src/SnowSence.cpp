#include "../lib/SnowSence.h"
#include <glm/glm.hpp>
#include <cmath>




SnowSence::SnowSence(number,size,rage,range){
	numberOfFlack = number;
	maxSize = size;
	rageLevel = rage;
	snowRange = range;
	acceleration = 0.009;
	generate();
}


void SnowSence::generate(){
	snowArray.resize(numberOfFlack);
	for (int i = 0; i < numberOfFlack; i++){
		r = ((double) rand() / (RAND_MAX));
		snowArray[i].size = maxSize * ((double) rand() / (RAND_MAX));
		GLfloat theta = 2 * glm::pi<double>() * ((double) rand() / (RAND_MAX)); //0 ~ 2pi
		GLfloat fai = glm::pi<double>() * (((double) rand() / (RAND_MAX))-0.5); // -pi/2 ~ pi/2
		snowArray[i].posX = snowRange * cos(theta) * cos(fai);
		snowArray[i].posY = snowRange * sin(theta) * cos(fai);
		snowArray[i].posZ = snowRnage * sin(fai);

		snowArray[i].speed = sqrt(2*acceleration*snowArray[i].posY);
	}

	

}

void SnowSence::show(){

	Shader snowShader("shader/snow.vs","shader/skybox.frag");

	glGenBuffers(1, &SVBO);
	glGenVertexArrays(1, &VertexArrayID);

	glBindVertexArray(VertexArrayID);

	for (int i = 0; i < numberOfFlack; i++){
		snowArray[i].posY += snowArray[i].speed;
		if (outRange(snowArray[i])){

		}
		snowArray[i].speed += acceleration;
		drawCube(snowArray[i]);

	}
}

void drawCube(snowflack flack){
	GLfloat vertex_buffer[] = {
	    -1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,
	     1.0f,-1.0f,-1.0f,
	     1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f,
	     1.0f, 1.0f,-1.0f,
	     1.0f,-1.0f,-1.0f,
	     1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f,
	     1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f
		};
	for (int i=0;i<36;i++){
		vertex_buffer[i*3]   = vertex_buffer[i*3]  /(2*size) + flack.posX;
		vertex_buffer[i*3+1] = vertex_buffer[i*3+1]/(2*size) + flack.posY;
		vertex_buffer[i*3+2] = vertex_buffer[i*3+2]/(2*size) + flack.posZ;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, SVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), &vertex_buffer, GL_DYNAMIC_DRAW);

    glBindVertexArray(VertexArrayID);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    glDrawArrays(GL_TRIANGLES, 0, 12*3);



}



