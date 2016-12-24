#include "GLRenderer.h"
#include <assert.h>
#include <sstream>
#include "Logger.h"


GLRenderer::GLRenderer()
{
}


bool GLRenderer::Initialize(GLFWwindow * Window)
{
	assert(Window);

	Context = Window;

	int width, height = 0;

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	GLint majorVersionNumber = 0;
	GLint minorVersionNumber = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersionNumber);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersionNumber);

	std::stringstream versionLog;

	versionLog << "OpenGL version number is : " << majorVersionNumber << "." << minorVersionNumber << std::ends;

	Logger::GetLogger().LogString(versionLog.str(), LogType::LOG);

	glfwGetFramebufferSize(Context, &width, &height);
	glViewport(0, 0, width, height);


	return true;

}

void GLRenderer::Clear()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::Present()
{
	glfwSwapBuffers(Context);
}

void GLRenderer::DrawMesh(const Mesh & mesh)
{
	//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertices().size());

	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void GLRenderer::Draw()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glfwSwapBuffers(Context);
}

GLRenderer::~GLRenderer()
{
}
