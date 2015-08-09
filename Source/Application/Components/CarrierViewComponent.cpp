
#include <Application/Components/CarrierViewComponent.hpp>

#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/TextureManager.hpp>

#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Entity/Entity.hpp>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	CaffApp::ShaderID		shaderID;
	CaffApp::ShaderID		lightShaderID;
	CaffApp::VertexFormatID	vertexFormat;
}


namespace App {

CarrierViewComponent::CarrierViewComponent()
: CaffComp::RenderableComponent()
{
}

CarrierViewComponent::~CarrierViewComponent()
{
}

void CarrierViewComponent::render()
{
	CaffComp::TransformComponent *transform   = getOwner()->getTransform();
	
		  glm::mat4 m = transform->getWorldMatrix();
	const glm::mat4 v = CaffServ::CameraManager().getCurrentCamera().getViewMat();
	const glm::mat4 p = CaffServ::CameraManager().getCurrentCamera().getProjMat();
	
	CaffApp::Renderer  & renderer  = CaffServ::RendererManager();
	
	// Runway
	const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("runway");
	const glm::vec3			   camPos	 = CaffServ::CameraManager().getCurrentCamera().getPosition();
		
	for(int i = 0; i < modelData.model->getNumberOfMeshes(); ++i)
	{
		renderer.reset();
		renderer.setShader(lightShaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelData.vertBufferIDs[i]);
		
		renderer.setShaderMatrix44f("worldMat", &m[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setShader3f("eye", &camPos[0]);
		//renderer.setTexture("diffuseTex", texData.textureID);
		
		renderer.apply();
		
		glDrawArrays(GL_TRIANGLES, 0, (int)modelData.model->getMesh(i).getGLFaces());
	}
}


void CarrierViewComponent::onAttach()
{
	// -- Renderer

	Caffeine::Components::RenderableComponent::onAttach();
	
	shaderID = Caffeine::Services::RendererManager().addShader(CaffUtil::GetPathDir() + "Assets/Shaders/FullBright.shd");
	
	std::array<Caffeine::Application::FormatDesc, 3> vertFmt = {{
		{"inPosition",	CaffApp::AttrFormat::FLOAT, 3},
		{"inTexC",		CaffApp::AttrFormat::FLOAT, 2},
		{"inNormal",	CaffApp::AttrFormat::FLOAT, 3},
	}};
	
	vertexFormat = Caffeine::Services::RendererManager().addVertexFormat(&vertFmt[0], vertFmt.size());
	
	lightShaderID = CaffServ::RendererManager().addShader(CaffUtil::GetPathDir() + "Assets/Shaders/ForwardLighting.shd");
}


void CarrierViewComponent::onDisattach()
{
	CaffComp::RenderableComponent::onDisattach();
}


void CarrierViewComponent::onEarlyThink(const float dt)
{
}


void CarrierViewComponent::onThink(const float dt)
{
}


void CarrierViewComponent::onLateThink(const float dt)
{
}


COMPONENT_FACTORY_SOURCE(CarrierViewComponent)


} // namespace
