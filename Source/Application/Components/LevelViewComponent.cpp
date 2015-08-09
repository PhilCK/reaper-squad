
#include <Application/Components/LevelViewComponent.hpp>

#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/TextureManager.hpp>

#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Entity/Entity.hpp>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <LinearMath/btAabbUtil2.h>

namespace
{
	CaffApp::ShaderID		shaderID;
	CaffApp::ShaderID		lightShaderID;
	CaffApp::VertexFormatID	vertexFormat;
}


namespace App {

LevelViewComponent::LevelViewComponent()
: Caffeine::Components::RenderableComponent()
{
}

LevelViewComponent::~LevelViewComponent()
{
}

void LevelViewComponent::render()
{
	Caffeine::Components::TransformComponent *transform = getOwner()->getTransform();
	
		  glm::mat4 m = transform->getWorldMatrix();
	const glm::mat4 v = CaffServ::CameraManager().getCurrentCamera().getViewMat();
	const glm::mat4 p = CaffServ::CameraManager().getCurrentCamera().getProjMat();
	
	CaffApp::Renderer  & renderer  = CaffServ::RendererManager();

	// Render the object
	{
		const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("plane");
		const CaffSys::TextureData & texData = CaffServ::TextureManager().getTextureData("dev_grid");
	
		renderer.reset();
		renderer.setShader(shaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelData.vertBufferIDs[0]);
		
		renderer.setShaderMatrix44f("worldMat", &m[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setTexture("diffuseTex", texData.textureID);
		
		const glm::vec4 color(1, 1, 1, 0);
		renderer.setShader4f("color", &color[0]);
		
		const glm::vec3 scale(100, 1, 100);
		renderer.setShader3f("vertScale", &scale[0]);
		
		const glm::vec2 texScale(50, 50);
		renderer.setShader2f("texScale", &texScale[0]);
		renderer.apply();
		
		//glDrawArrays(GL_TRIANGLES, 0, (int)modelData.model->getMesh(0).getGLFaces());
	}
	
	
	// Render the object
	for(int i = 0; i < 5; ++i)
	{
		const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("ring");
		const glm::vec3			   camPos	 = CaffServ::CameraManager().getCurrentCamera().getPosition();
		
		m = glm::translate(m, glm::vec3(0, 0, 4));
		
		renderer.reset();
		renderer.setShader(lightShaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelData.vertBufferIDs[0]);
		
		renderer.setShaderMatrix44f("worldMat", &m[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setShader3f("eye", &camPos[0]);
		//renderer.setTexture("diffuseTex", texData.textureID);
		
		renderer.apply();
		
		glDrawArrays(GL_TRIANGLES, 0, (int)modelData.model->getMesh(0).getGLFaces());
	}
	
	// Droids
	const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("droids");
	for(int i = 0; i < modelData.model->getNumberOfMeshes(); ++i)
	{
		const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("droids");
		const glm::vec3			   camPos	 = CaffServ::CameraManager().getCurrentCamera().getPosition();
		
		glm::mat4 droids;
		droids = glm::scale(droids, glm::vec3(8,8,8));
		
		renderer.reset();
		renderer.setShader(lightShaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelData.vertBufferIDs[i]);
		
		renderer.setShaderMatrix44f("worldMat", &droids[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setShader3f("eye", &camPos[0]);
		//renderer.setTexture("diffuseTex", texData.textureID);
		
		renderer.apply();
		
		glDrawArrays(GL_TRIANGLES, 0, (int)modelData.model->getMesh(i).getGLFaces());
	}
	
	
	// Ships
	const CaffSys::ModelData & modelDatas = CaffServ::ModelManager().getModelData("ships");
	for(int i = 0; i < modelDatas.model->getNumberOfMeshes(); ++i)
	{
		const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("droids");
		const glm::vec3			   camPos	 = CaffServ::CameraManager().getCurrentCamera().getPosition();
		
		glm::mat4 droids;
		droids = glm::scale(droids, glm::vec3(8,8,8));
		// TODO: Transform scale is fooked.
		
		renderer.reset();
		renderer.setShader(lightShaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelDatas.vertBufferIDs[i]);
		
		renderer.setShaderMatrix44f("worldMat", &droids[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setShader3f("eye", &camPos[0]);
		//renderer.setTexture("diffuseTex", texData.textureID);
		
		renderer.apply();
		
		//glDrawArrays(GL_TRIANGLES, 0, (int)modelDatas.model->getMesh(i).getGLFaces());
	}
}


void LevelViewComponent::onAttach()
{
	// -- Physics
	
	CaffComp::PhysicsComponent *phys = getOwner()->getRigidBody();
	
	phys->setMass(0);
	
	std::unique_ptr<CaffComp::Collider> collider(new CaffComp::StaticPlaneCollider(0.f, 1.f, 0.f, 0.05f));
	phys->setColliderShape(std::move(collider));
	
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


void LevelViewComponent::onDisattach()
{
	Caffeine::Components::RenderableComponent::onDisattach();
}


void LevelViewComponent::onEarlyThink(const float dt)
{
}


void LevelViewComponent::onThink(const float dt)
{
}


void LevelViewComponent::onLateThink(const float dt)
{
}



COMPONENT_FACTORY_SOURCE(LevelViewComponent)

} // namespace
