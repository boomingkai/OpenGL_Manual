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
	glusProgramBuildFromSource(&g_programe, (const GLchar**)&vertexSource.text, 0, 0, 0, (const GLchar**)&fragmentSource.text);
	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);

	g_model_view_mat_location = glGetUniformLocation(g_programe.program, "u_modelViewProjectionMatrix");
	g_texture_location = glGetUniformLocation(g_programe.program, "u_texture");
	g_vertex_location = glGetUniformLocation(g_programe.program, "a_vertex");
	g_text_coord_location = glGetUniformLocation(g_programe.program, "a_texCoord");

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
	glBindBuffer(GL_ARRAY_BUFFER, g_text_coord_vbo);
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices* 2*sizeof(GLfloat), (GLfloat*)plane.texCoords, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &g_indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_vbo);
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
	glVertexAttribPointer(g_text_coord_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(g_text_coord_location);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_vbo);


	glBindTexture(GL_TEXTURE_2D, g_texture);
	glUniform1i(g_texture_location, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return GLUS_TRUE;

}

GLUSvoid reshape(GLUSint width, GLUSint height)
{
	GLfloat viewMatrix[16];
	GLfloat modelViewProjectionMatrix[16];

	glViewport(0, 0, width, height);

	// Create the view matrix.
	glusMatrix4x4LookAtf(viewMatrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Create a orthogonal projection matrix.
	glusMatrix4x4Orthof(modelViewProjectionMatrix, -(GLfloat)width / 2.0f, (GLfloat)width / 2.0f, -(GLfloat)height / 2.0f, (GLfloat)height / 2.0f, 1.0f, 100.0f);

	// MVP = P * V * M (Note: Here we do not have model matrix).
	glusMatrix4x4Multiplyf(modelViewProjectionMatrix, modelViewProjectionMatrix, viewMatrix);

	// Pass the model view projection matrix to the current active program.
	glUniformMatrix4fv(g_model_view_mat_location, 1, GL_FALSE, modelViewProjectionMatrix);
}


GLUSboolean update(GLUSfloat time)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, g_number_indices_plane, GL_UNSIGNED_INT, 0);
	return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (g_vertex_vbo)
	{
		glDeleteBuffers(1, &g_vertex_vbo);
	}

	if (g_text_coord_vbo)
	{
		glDeleteBuffers(1, &g_text_coord_vbo);
		g_text_coord_vbo = 0;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (g_indices_vbo)
	{
		glDeleteBuffers(1, &g_indices_vbo);
		g_indices_vbo = 0;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	if (g_texture)
	{
		glDeleteTextures(1, &g_texture);
		g_texture = 0;
	}

	glBindVertexArray(0);

	if (g_vao)
	{
		glDeleteVertexArrays(1, &g_vao);
		g_vao = 0;
	}

	glUseProgram(0);
	glusProgramDestroy(&g_programe);
}


int main()
{
	EGLint eglConfigAttributes[] = {
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_DEPTH_SIZE, 0,
			EGL_STENCIL_SIZE, 0,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
			EGL_NONE
	};

	EGLint eglContextAttributes[] = {
			EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 2,
			EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_TRUE,
			EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
			EGL_NONE
	};

	glusWindowSetInitFunc(init);
	glusWindowSetReshapeFunc(reshape);
	glusWindowSetUpdateFunc(update);
	glusWindowSetTerminateFunc(terminate);
	if (!glusWindowCreate("GLUS Example Window", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes, 0))
	{
		printf("Could not create window!\n");
		return -1;
	}

	glusWindowRun();
	return 0;
}