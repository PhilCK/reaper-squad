
#include <Application/Components/BulletControllerComponent.hpp>

namespace App {

BulletControllerComponent::BulletControllerComponent()
: CaffComp::ControllerComponent()
{
}

BulletControllerComponent::~BulletControllerComponent()
{
}

void BulletControllerComponent::onAttach()
{
}

void BulletControllerComponent::onDisattach()
{
}

void BulletControllerComponent::onThink(const float dt)
{
}

void BulletControllerComponent::eventController(const CaffSys::Event *eventData)
{
}

COMPONENT_FACTORY_SOURCE(BulletControllerComponent)

} // namespace
