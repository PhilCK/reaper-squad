
#include <UnitTest.hpp>
#include <Caffeine/Entity/SimpleEntity.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <memory>
#include <iostream>

namespace CaffEntity = ::Caffeine::Entity;

// Test components.
class TestComponentOne : public CaffEntity::Component {};
class TestComponentTwo : public CaffEntity::Component {};


// Check to see if components attach
TEST(AttachEntity)
{
	CaffEntity::SimpleEntity *entity  = new CaffEntity::SimpleEntity;
	CaffEntity::Component *component1 = new CaffEntity::Component;
	CaffEntity::Component *component2 = new CaffEntity::Component;

	entity->addComponent("comp1", component1);

	ASSERT_IS_EQUAL(entity, component1->getOwner())

	entity->addComponent("comp2", component2);

	ASSERT_IS_EQUAL(entity, component2->getOwner())

	delete entity; // Should destroy others.
}

// Detach components.
TEST(DetachEntity)
{
	// Currently no support for detaching entities.
}

int main()
{
	Test::RunTests();

	return 0;
}