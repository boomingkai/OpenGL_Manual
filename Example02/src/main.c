#include<stdio.h>
#include<GL/glus.h>

GLUSboolean init(GLUSvoid)
{
	glClearColor(0.0, 1.0, 0.0, 1.0);
	return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height)
{
	glViewport(0, 0, width, height);
}

GLUSboolean update(GLUSvoid)
{
	glClear(GL_COLOR_BUFFER_BIT);
	return GLUS_TRUE;
}

GLUSvoid terminate()
{
	return;
}

int main()
{
	EGLint eglConfigAttibutes[] = {
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
	if (!glusWindowCreate("GLUS Example Window", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttibutes, eglContextAttributes, 0))
	{
		printf("create window error");
		return -1;
	}

	glusWindowRun();
	return 1;
}