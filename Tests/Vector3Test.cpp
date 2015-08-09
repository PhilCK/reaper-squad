
#include <UnitTest.hpp>
#include <Vector3.hpp>
#include <typeinfo>   // operator typeid

using namespace Kakaw;

TEST(LoadVec3)
{	
	float x, y, z;		// Components

	float testVal1 = 4;
	float testVal2 = 1.23f;
	float testVal3 = 2.34f;
	float testVal4 = 3.45f;

	// -- Load Test 1 -- //
	Vector3 vec3Test1 = SetVec3(testVal1);

	GetVec3X(&x, vec3Test1);
	GetVec3Y(&y, vec3Test1);
	GetVec3Z(&z, vec3Test1);

	ASSERT_IS_EQUAL(testVal1, x);
	ASSERT_IS_EQUAL(testVal1, y);
	ASSERT_IS_EQUAL(testVal1, y);


	// -- Load Test 2 -- //
	Vector3 vec3Test2 = SetVec3(testVal2, testVal3, testVal4);

	GetVec3X(&x, vec3Test2);
	GetVec3Y(&y, vec3Test2);
	GetVec3Z(&z, vec3Test2);

	ASSERT_IS_EQUAL(testVal2, x);
	ASSERT_IS_EQUAL(testVal3, y);
	ASSERT_IS_EQUAL(testVal4, z);
}


TEST(BasicArithmeticVec3)
{
	// Components
	float x, y, z;
	float err = 0.00001f;

	Vector3 a 		= SetVec3(1.02f, 0.0f, 3.456f);
	Vector3 b 		= SetVec3(2.4f);
	Vector3 res 	= SetVec3(0);

	// -- Addition Vec3 + Vec3 -- //

	AddVec3(&res, a, b);

	GetVec3X(&x, res);
	GetVec3Y(&y, res);
	GetVec3Z(&z, res);	

	ASSERT_IS_NEAR(3.42f,  x, err);
	ASSERT_IS_NEAR(2.4f,   y, err);
	ASSERT_IS_NEAR(5.856f, z, err);


	// -- Addition Vec3 + float -- //
	AddVec3(&res, a, 2.4f);

	GetVec3X(&x, res);
	GetVec3Y(&y, res);
	GetVec3Z(&z, res);

	ASSERT_IS_NEAR(3.42f,  x, err);
	ASSERT_IS_NEAR(2.4f,   y, err);
	ASSERT_IS_NEAR(5.856f, z, err);



	// -- Subtraction Vec3 - Vec3 -- //
	SubVec3(&res, a, b);

	GetVec3X(&x, res);
	GetVec3Y(&y, res);
	GetVec3Z(&z, res);	

	ASSERT_IS_NEAR(-1.38f, x, err);
	ASSERT_IS_NEAR(-2.4f,  y, err);
	ASSERT_IS_NEAR(1.056f, z, err);


	// -- Subtraction Vec3 - float -- //
	SubVec3(&res, a, b);

	GetVec3X(&x, res);
	GetVec3Y(&y, res);
	GetVec3Z(&z, res);

	ASSERT_IS_NEAR(-1.38,  x, err);
	ASSERT_IS_NEAR(-2.4f,  y, err);
	ASSERT_IS_NEAR(1.056f, z, err);
}


TEST(CrossAndDotVec3)
{

}


TEST(NormalizeVec3)
{

}


TEST(LengthVec3)
{
	
}



TEST(LerpVec3)
{
	
}



TEST(SlerpVec3)
{

}

int main()
{
	Test::RunTests();

	//float a = -1.38f;
	//float b = -1.38f;


	return 0;
}
