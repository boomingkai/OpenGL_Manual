#include "GL/glus.h"
#include <stdio.h>

static GLUSprogram g_programe;
static GLint g_model_view_mat_location;
static GLint g_vertex_location;
static GLint g_text_coord_location;
static GLint g_texture_location;
static GLuint g_vertex_vbo;
static GLuint g_text_coord_vbo;
static GLuint g_indices_vbo;
static GLuint g_vao;
static GLuint g_texture;
static GLint g_number_indices_plane;

GLUSboolean init(GLUSvoid)
{
	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;
	GLUStgaimage image;
	GLUSshape plane;


	glusFileLoadText("../Example03/shader/texture.vert.glsl", &vertexSource);
	glusFileLoadText("../Example03/shader/grey.frag.glsl", &fragmentSource);
	glusProgramBuildFromSource(&g_programe, (const GLchar**)vertexSource.text, 0, 0, 0, (const GLchar**)fragmentSource.text);
	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);

	g_model_view_mat_location = glGetUniformLocation(g_programe.program, "u_modelViewProjectionMatrix");
	g_texture_location = glGetUniformLocation(g_programe.program, "u_texture");
	g_vertex_location = glGetUniformLocation(g_programe.program, "a_vertex");
	g_texture_location = glGetUniformLocation(g_programe.program, "a_texCoord");

	glusImageLoadTga("desert.tga", &image);
	glGenTextures(1, &g_texture);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);


	glusShapeCreateRectangularPlanef(&plane, (GLfloat)image.width / 2.0, (GLfloat)image.height / 2.0);
	glusImageDestroyTga(&image);

	g_number_indices_plane = plane.numberIndices;

	glGenBuffers(1, &g_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, plane.numberIndices * 4 * sizeof(GLfloat), (GLfloat*)plane.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &g_text_coord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, &g_text_coord_vbo);
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices* 2*sizeof(GLfloat), (GLfloat*)plane.texCoords, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &g_indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.numberIndices* sizeof(GLint), (GLint*)plane.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glusShapeDestroyf(&plane);

	glUseProgram(g_programe.program);
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_vbo);
	glVertexAttribPointer(g_vertex_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_vertex_location);
	glBindBuffer(GL_ARRAY_BUFFER, g_text_coord_vbo);
	glVertexAttribPointer(g_texture_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_texture_location);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_vbo);


	glBindTexture(GL_TEXTURE_2D, g_texture);
	glUniform1i(g_texture_location, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return GLUS_TRUE;

}

int main()
{

}