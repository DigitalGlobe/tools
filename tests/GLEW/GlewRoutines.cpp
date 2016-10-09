#include "GlewRoutines.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <glew.h>
#include <wglew.h>
#include <glut.h>


int GlewRoutines::GlewTest(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutCreateWindow("GLEW Test");

	glewExperimental = TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	if (GLEW_VERSION_1_5)
	{
		fprintf(stdout, "Core extensions of OpenGL 1.1 to 1.5 are available!\n");
	}

	if (glewIsSupported("GL_VERSION_1_4 GL_ARB_point_sprite"))
	{
		/* Great, we have OpenGL 1.4 + point sprites. */
		fprintf(stdout, "OpenGL 1.4 + point sprites are available!\n");
	}


	if (!GLEW_EXT_framebuffer_object)
	{
		fprintf(stderr, "GLEW_EXT_framebuffer_object not available!\n");
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	if (WGLEW_ARB_pbuffer) {
		printf("WGLEW_ARB_pbuffers are available on this platform.\n");		
	}
	else{
		printf("Sorry, WGLEW_ARB_pbuffers will not work on this platform.\n");
	}

	if (GL_ARB_copy_image) {
		printf("We can use the GL_ARB_copy_image extension.\n");
	}
	else {
		printf("The GL_ARB_copy_image extension is not available.\n");
	}

	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
	{
		printf("No OpenGL shading language support\n");
	}
	else {
		printf("OpenGL shading language is supported\n");
	}
	
	return 0;
}