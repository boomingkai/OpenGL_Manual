#include "GL/glus.h"
#include <stdio.h>

static GLUSprogram g_programe;
static GLint g_vertex_location;
static GLint g_vertex_buffer_object;
static GLint g_vao;

GLUSboolean init(GLUSvoid)
{

	GLfloat points[] = {-0.5,0.0,0.0,1.0, 0.5,0.0,0.0,1.0, 0.0,0.5,0.0,1.0};
	GLUStextfile vertexSource;
	GLUStextfile fragmentSource;
	glusFileLoadText("../Example02/shader/simple.vert.glsl", &vertexSource);
	glusFileLoadText("../Example02/shader/red.frag.glsl", &fragmentSource);

	glusProgramBuildFromSource(&g_programe.program, (const GLUSchar**)&vertexSource.text, 0, 0, 0, (const GLUSchar**)&fragmentSource.text);
	glusFileDestroyText(&vertexSource);
	glusFileDestroyText(&fragmentSource);
	g_vertex_location = glGetAttribLocation(g_programe.program, "a_vertex");
	glGenBuffers(1, &g_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLUSfloat), (GLUSfloat*)points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(g_programe.program);
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_object);
	glVertexAttribPointer(g_vertex_location, 4, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(g_vertex_location);
	glClearColor(.0f, .0f, .0f, .0f);

	return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height)
{
	glViewport(0,0,width, height);
}

GLUSboolean update(GLUSfloat time)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (g_vertex_buffer_object)
	{
		glDeleteBuffers(1, &g_vertex_buffer_object);
		g_vertex_buffer_object = 0;
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
	glusWindowSetTerminateFunc(terminate);
	glusWindowSetUpdateFunc(update);
	if (!glusWindowCreate("GLUS Example Window", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes, 0))
	{
		printf("Could not create window!\n");
		return -1;
	}

	glusWindowRun();
	return 0;

}