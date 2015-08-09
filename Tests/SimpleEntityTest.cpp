
#include <UnitTest.hpp>
#include <Caffeine/Entity/SimpleEntity.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <memory>
#include <iostream>

namespace CaffEntity = ::Caffeine::Entity;

// Test components.
class TestComponentOne : public CaffEntity::Component {};
class TestComponentTwo : public CaffEntity::Component {};

// Create an enity and check state.
TEST(CreateEntity)
{
	CaffEntity::SimpleEntity *entity = new CaffEntity::SimpleEntity();

	ASSERT_IS_TRUE(entity)
	ASSERT_IS_EQUAL(entity->getNumberOfComponents(), 0)

	delete entity;
}

// Create entity add components and check state.
TEST(TestComponents)
{
	CaffEntity::SimpleEntity *entity 	 = new CaffEntity::SimpleEntity();
	TestComponentOne 		 *component1 = new TestComponentOne();
	TestComponentTwo 		 *component2 = new TestComponentTwo();

	entity->addComponent("test1", component1);

	ASSERT_IS_EQUAL(entity->getNumberOfComponents(), 1)
	ASSERT_IS_EQUAL(component1->getOwner(), entity)

	entity->addComponent("test2", component2);

	ASSERT_IS_EQUAL(entity->getNumberOfComponents(), 2)
	ASSERT_IS_EQUAL(component2->getOwner(), entity)

	delete entity;
}

// Destroy entity and components and check state.
TEST(DeleteEntity)
{
	/*
	CaffEntity::SimpleEntity *entity 	 = new CaffEntity::SimpleEntity();
	TestComponentOne 		 *component1 = new TestComponentOne();
	TestComponentTwo 		 *component2 = new TestComponentTwo();
	*/

	// We need a better way of checking to see if it has been deleted.
	ASSERT_IS_TRUE(false)
}

int main()
{
	Test::RunTests();

	return 0;
}