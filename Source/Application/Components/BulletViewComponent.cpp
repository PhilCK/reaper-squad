
#include <Application/Components/BulletViewComponent.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Application/Components/BulletModelComponent.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {

	CaffApp::VertexFormatID	vertFmt(CaffApp::NO_VERTEX_FORMAT);
	CaffApp::VertexID		vertBuffer(CaffApp::NO_VERTEX_BUFFER);
	CaffApp::ShaderID		shader(CaffApp::NO_SHADER);
	
	const std::array<CaffApp::FormatDesc, 1> vertFmtDesc = {{
		{"inPosition", CaffApp::AttrFormat::FLOAT, 3},
	}};
	
	std::array<float, 3> lineMesh = {{
		0,0,0,
	}};

}

namespace App {

BulletViewComponent::BulletViewComponent()
: CaffComp::RenderableComponent()
{
}


BulletViewComponent::~BulletViewComponent()
{
}


void BulletViewComponent::onAttach()
{
	CaffApp::Renderer &renderer = CaffServ::RendererManager();
	
	if(!shader)
	shader		= renderer.addShader(CaffUtil::GetPathDir() + "Assets/Shaders/Bullet.shd");
	
	if(!vertFmt)
	vertFmt		= renderer.addVertexFormat(&vertFmtDesc[0], vertFmtDesc.size());
	
	if(!vertBuffer)
	vertBuffer	= renderer.addVertexBuffer(&lineMesh[0], sizeof(lineMesh), CaffApp::BufferAccess::STATIC);
}


void BulletViewComponent::onDisattach()
{
}


void BulletViewComponent::render()
{
	BulletModelComponent *model = getOwner()->getComponent<BulletModelComponent>();
	assert(model);
	
	//glm::vec3 start = model->getStartPos();
	//glm::vec3 end = start + (model->getDirection() * 10.0f);
	
	//CaffSys::DebugRenderer::DrawLine(start, end, glm::vec3(1,0,1));
	
	// Render the line
	CaffApp::Renderer &renderer = CaffServ::RendererManager();
	CaffSys::Camera   &camera   = CaffServ::CameraManager().getCurrentCamera();
	{
		glm::mat4 startPos;
		startPos = glm::translate(startPos, model->getCurrentPos());
		
		const glm::mat4 v = camera.getViewMat();
		const glm::mat4 p = camera.getProjMat();
		const glm::mat4 wvp = p * v * startPos;
		
		const glm::vec3 color(1, 0, 0);
		const float		length(100.0f);
		
		renderer.reset();
		renderer.setShader(shader);
		renderer.setVertexFormat(vertFmt);
		renderer.setVertexBuffer(vertBuffer);
		renderer.setShaderMatrix44f("wvpMat", &wvp[0][0]);
		renderer.setShader4f("color", &color[0]);
		renderer.setShader3f("direction", &model->getDirection()[0]);
		renderer.setShader1f("tracerLength", &length);
		renderer.apply();
		
		glDrawArrays(GL_POINTS, 0, 1);
	}
}


void BulletViewComponent::onThink(const float dt)
{
}


COMPONENT_FACTORY_SOURCE(BulletViewComponent)


} // namespace
