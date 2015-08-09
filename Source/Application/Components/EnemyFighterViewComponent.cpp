
#include <Application/Components/EnemyFighterViewComponent.hpp>

#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/TextureManager.hpp>

#include <Application/Components/PlayerViewComponent.hpp>
#include <glm/glm.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <LinearMath/btAabbUtil2.h>


namespace
{
	// The id's for the renderer;
	// This will duplicate stuff, reqruies a 3rd layer between low level renderer and interface.
	//CaffApp::Model			model;		// Mesh/Shader can go into a material file instead.
	CaffApp::ShaderID		shaderID;
	CaffApp::VertexFormatID	vertexFormat;
}


namespace App {


EnemyFighterViewComponent::EnemyFighterViewComponent()
{
}


EnemyFighterViewComponent::~EnemyFighterViewComponent()
{
}


void EnemyFighterViewComponent::onAttach()
{
	CaffComp::RenderableComponent::onAttach();

	shaderID = CaffServ::RendererManager().addShader(CaffUtil::GetPathDir() + "Assets/Shaders/ForwardLighting.shd");
	
	std::array<Caffeine::Application::FormatDesc, 3> vertFmt = {{
		{"inPosition",	CaffApp::AttrFormat::FLOAT, 3},
		{"inTexC",		CaffApp::AttrFormat::FLOAT, 2},
		{"inNormal",	CaffApp::AttrFormat::FLOAT, 3},
	}};
	
	vertexFormat = CaffServ::RendererManager().addVertexFormat(&vertFmt[0], vertFmt.size());
	
	CaffComp::TransformComponent *transform = getOwner()->getTransform();
	
	const glm::vec3 scale(0.1f, 0.1f, 0.1f);
	
	//transform->setScale(scale);
}


void EnemyFighterViewComponent::onDisattach()
{
}


void EnemyFighterViewComponent::render()
{
	CaffComp::TransformComponent *transform = getOwner()->getTransform();

	const glm::mat4 m = transform->getWorldMatrix();
	const glm::mat4 v = CaffServ::CameraManager().getCurrentCamera().getViewMat();
	const glm::mat4 p = CaffServ::CameraManager().getCurrentCamera().getProjMat();
	const glm::vec3 camPos = CaffServ::CameraManager().getCurrentCamera().getPosition();
	
		  CaffApp::Renderer  & renderer  = CaffServ::RendererManager();
	const CaffSys::ModelData & modelData = CaffServ::ModelManager().getModelData("spitfire");
	
	const CaffSys::TextureData & texData = CaffServ::TextureManager().getTextureData("jet_trainer");
	
	for(int i = 0; i < modelData.model->getNumberOfMeshes(); ++i)
	{
		renderer.reset();
		renderer.setShader(shaderID);
		renderer.setVertexFormat(vertexFormat);
		renderer.setVertexBuffer(modelData.vertBufferIDs[i]);
		
		renderer.setShaderMatrix44f("worldMat", &m[0][0]);
		renderer.setShaderMatrix44f("viewMat",  &v[0][0]);
		renderer.setShaderMatrix44f("projMat",  &p[0][0]);
		renderer.setShader3f("eye", &camPos[0]);
		renderer.setTexture("diffuseTex", texData.textureID);
		
		renderer.apply();
		
		glDrawArrays(GL_TRIANGLES, 0, (int)modelData.model->getMesh(i).getGLFaces());
	}
}


void EnemyFighterViewComponent::onThink(const float dt)
{
}


COMPONENT_FACTORY_SOURCE(EnemyFighterViewComponent)

} // namespace
