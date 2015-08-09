
#include <UnitTest.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Math/Math.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CaffComp = ::Caffeine::Components;
namespace CaffMath = ::Caffeine::Math;


TEST(Position)
{
	CaffComp::TransformComponent trans;

	ASSERT_IS_EQUAL(trans.getPosition().x, 0)
	ASSERT_IS_EQUAL(trans.getPosition().y, 0)
	ASSERT_IS_EQUAL(trans.getPosition().z, 0)

	glm::vec3 updatePos(1,2,3);
	trans.setPosition(updatePos);

	ASSERT_IS_EQUAL(trans.getPosition().x, 1)
	ASSERT_IS_EQUAL(trans.getPosition().y, 2)
	ASSERT_IS_EQUAL(trans.getPosition().z, 3)

	trans.addDeltaPosition(1,2,3);

	ASSERT_IS_EQUAL(trans.getPosition().x, 2)
	ASSERT_IS_EQUAL(trans.getPosition().y, 4)
	ASSERT_IS_EQUAL(trans.getPosition().z, 6)

	glm::mat4 id;
	id = glm::translate(id, glm::vec3(7,8,9));
	trans.setWorldMatrix(id);

	ASSERT_IS_EQUAL(trans.getPosition().x, 7)
	ASSERT_IS_EQUAL(trans.getPosition().y, 8)
	ASSERT_IS_EQUAL(trans.getPosition().z, 9)
}


TEST(Rotation)
{
	CaffComp::TransformComponent trans;

	ASSERT_IS_EQUAL(trans.getRotation().x, 0)
	ASSERT_IS_EQUAL(trans.getRotation().y, 0)
	ASSERT_IS_EQUAL(trans.getRotation().z, 0)

	glm::vec3 updateRot(10, 20, 30);
	trans.setRotation(updateRot);

	ASSERT_IS_EQUAL(trans.getRotation().x, 10)
	ASSERT_IS_EQUAL(trans.getRotation().y, 20)
	ASSERT_IS_EQUAL(trans.getRotation().z, 30)

	trans.addDeltaRotation(10, 20, 30);

	ASSERT_IS_EQUAL(trans.getRotation().x, 20)
	ASSERT_IS_EQUAL(trans.getRotation().y, 40)
	ASSERT_IS_EQUAL(trans.getRotation().z, 60)

	// Set matrix
	glm::mat4 id;

	float pitch(-2.6337f);
	float yaw(-0.47158f);
	float roll(-1.2795f);

	//float pitch(CaffMath::DegToRad(27));
	//float yaw(CaffMath::DegToRad(27));
	//float roll(CaffMath::DegToRad(27));

	id = glm::yawPitchRoll(yaw, pitch, roll);
	trans.setWorldMatrix(id);

	std::cout << std::endl;
	std::cout << id[0][0] << " " << id[0][1] << " " << id[0][2] << " " << id[0][3] << std::endl;
	std::cout << id[1][0] << " " << id[1][1] << " " << id[1][2] << " " << id[1][3] << std::endl;
	std::cout << id[2][0] << " " << id[2][1] << " " << id[2][2] << " " << id[2][3] << std::endl;
	std::cout << id[3][0] << " " << id[3][1] << " " << id[3][2] << " " << id[3][3] << std::endl;
	

	ASSERT_IS_EQUAL(trans.getRotation().x, pitch)
	ASSERT_IS_EQUAL(trans.getRotation().y, yaw)
	ASSERT_IS_EQUAL(trans.getRotation().z, roll)

	// Test matrix gets correct pitch yaw roll.
	// http://stackoverflow.com/questions/11514063/extract-yaw-pitch-and-roll-from-a-rotationmatrix
}



TEST(Scale)
{
	CaffComp::TransformComponent trans;

	ASSERT_IS_EQUAL(trans.getScale().x, 1)
	ASSERT_IS_EQUAL(trans.getScale().y, 1)
	ASSERT_IS_EQUAL(trans.getScale().z, 1)

	glm::vec3 updateScale(0.1f, 0.2f, 0.3f);
	trans.setScale(updateScale);

	ASSERT_IS_NEAR(trans.getScale().x, 0.1f, 0.01f)
	ASSERT_IS_NEAR(trans.getScale().y, 0.2f, 0.01f)
	ASSERT_IS_NEAR(trans.getScale().z, 0.3f, 0.01f)

	trans.addDeltaScale(0.1f, 0.2f, 0.3f);

	ASSERT_IS_EQUAL(trans.getScale().x, 0.2f)
	ASSERT_IS_EQUAL(trans.getScale().y, 0.4f)
	ASSERT_IS_EQUAL(trans.getScale().z, 0.6f)

	glm::mat4 id;
	id = glm::scale(id, glm::vec3(10.0f,8.0f,9.0f));
	glm::mat4 rot = glm::yawPitchRoll(20.0f, 4.0f, 1.0f);
	id = id * rot;

	glm::vec3 forward(1, 0, 0);

	glm::vec4 scaleFwd = glm::vec4(forward, 1.0) * id;

	trans.setWorldMatrix(id);

	std::cout << std::endl;
	std::cout << id[0][0] << " " << id[0][1] << " " << id[0][2] << " " << id[0][3] << std::endl;
	std::cout << id[1][0] << " " << id[1][1] << " " << id[1][2] << " " << id[1][3] << std::endl;
	std::cout << id[2][0] << " " << id[2][1] << " " << id[2][2] << " " << id[2][3] << std::endl;
	std::cout << id[3][0] << " " << id[3][1] << " " << id[3][2] << " " << id[3][3] << std::endl;

	std::cout << std::endl;
	glm::vec3 scale(scaleFwd.x, scaleFwd.y, scaleFwd.z);
	std::cout << scaleFwd[0] << " " << scaleFwd[1] << " " << scaleFwd[2] << std::endl;
	std::cout << glm::length(scale) << std::endl;

	ASSERT_IS_EQUAL(trans.getScale().x, 10)
	ASSERT_IS_EQUAL(trans.getScale().y, 8)
	ASSERT_IS_EQUAL(trans.getScale().z, 9)
}


TEST(DirectionVectors)
{
	CaffComp::TransformComponent trans;

	// Really need to be able to test vectors!
}


TEST(WorldMatrix)
{

}


TEST(Everything)
{

}

int main()
{
	Test::RunTests();

	return 0;
}