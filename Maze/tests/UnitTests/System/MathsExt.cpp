//
//  MathsExt.cpp
//  Maze
//
//  Created by Jaraxus on 15/03/2020.
//

# include "../../../external/Catch/Catch.hpp"

# include "../../../include/System/MathsExt.hpp"

TEST_CASE("nearlyEquals", "[Math, Algorithm]")
{
    SECTION("1.0 nearly equals 1.0")
    {
        REQUIRE(mz::nearlyEquals(1., 1.) == true);
    }
    
    SECTION("0.0 nearly equals 0.0")
    {
        REQUIRE(mz::nearlyEquals(0., 0.) == true);
    }
    
    SECTION("10.0 doesn't nearly equals 5.0")
    {
        REQUIRE(mz::nearlyEquals(10., 5.) == false);
    }
    
    SECTION("1.0 nearly equals 2.0 with a maximum diference of 3.0")
    {
        REQUIRE(mz::nearlyEquals(1., 2., 3.) == true);
    }
    
    SECTION("5.0 nearly equals 10. with a maximum diference of 5.")
    {
        REQUIRE(mz::nearlyEquals(5., 10., 5.) == true);
    }
    
    SECTION("5.0 doesn't nearly equals 10.0 with a maximum diference of 3.0")
    {
        REQUIRE(mz::nearlyEquals(5., 10., 3.) == false);
    }
}

TEST_CASE("From degrees to radian", "[Math][Algorithm")
{
    SECTION("0 degree equals to 0 radian")
    {
        constexpr double radianAngle = mz::fromDegreesToRadian(0.);
        REQUIRE(radianAngle == 0.);
    }
    
    SECTION("90 degrees equal to pi / 2 radian")
    {
        constexpr double radianAngle = mz::fromDegreesToRadian(90.);
        REQUIRE(mz::nearlyEquals(radianAngle, M_PI_2));
    }
    
    SECTION("180 degrees equal to pi radian")
    {
        constexpr double radianAngle = mz::fromDegreesToRadian(180.);
        REQUIRE(mz::nearlyEquals(radianAngle, M_PI));
    }
    
    SECTION("270 degrees equal to 3 * pi / 2 radian")
    {
        const double radianAngle = mz::fromDegreesToRadian(270.);
        REQUIRE(mz::nearlyEquals(radianAngle, 3 * M_PI_2));
    }
    
    SECTION("360 degrees equal to 2 * M_PI radian")
    {
        const double radianAngle = mz::fromDegreesToRadian(360.);
        REQUIRE(mz::nearlyEquals(radianAngle, 2 * M_PI));
    }
    
    SECTION("-90 degrees equal to -pi / 2 radian")
    {
        const double radianAngle = mz::fromDegreesToRadian(-90.);
        REQUIRE(mz::nearlyEquals(radianAngle, -M_PI_2));
    }
}

TEST_CASE("From radian to degrees", "[Math][Algorithm")
{
    SECTION("0 radian equals to 0 degree")
    {
        constexpr double degreeAngle = mz::fromRadianToDegrees(0.);
        REQUIRE(degreeAngle == 0.);
    }
    
    SECTION("pi / 2 radian equal to 90 degrees")
    {
        constexpr double degreeAngle = mz::fromRadianToDegrees(M_PI_2);
        REQUIRE(mz::nearlyEquals(degreeAngle, 90.));
    }
    
    SECTION("pi radian equal to 180 degrees")
    {
        constexpr double degreeAngle = mz::fromRadianToDegrees(M_PI);
        REQUIRE(mz::nearlyEquals(degreeAngle, 180.));
    }
    
    SECTION("pi * 3/2 radian equal to 270 degrees")
    {
        const double degreeAngle = mz::fromRadianToDegrees(3 * M_PI_2);
        REQUIRE(mz::nearlyEquals(degreeAngle, 270.));
    }
    
    SECTION("2 * pi radian equal to 360 degrees")
    {
        const double degreeAngle = mz::fromRadianToDegrees(2 * M_PI);
        REQUIRE(mz::nearlyEquals(degreeAngle, 360.));
    }
    
    SECTION("-pi / 2 radian equal to -90 degrees")
    {
        const double degreeAngle = mz::fromRadianToDegrees(-M_PI_2);
        REQUIRE(mz::nearlyEquals(degreeAngle, -90.));
    }
}

TEST_CASE("Normalize vector")
{
    SECTION("Norm of normalized vector (1.0, 1.0) must be 1.0")
    {
        auto const normalizedVector = mz::normalizeVector<double>({1., 1.});
        const double norm = mz::getVectorLength(normalizedVector);
        REQUIRE(mz::nearlyEquals(norm, 1.) == true);
    }
    
    SECTION("Norm of normalized vector (0.0, 0.0) is 0.0")
    {
        auto const normalizedVector = mz::normalizeVector<double>({0., 0.});
        const double norm = mz::getVectorLength(normalizedVector);
        REQUIRE(mz::nearlyEquals(norm, 0.) == true);
    }
    
    SECTION("Norm of normalized vector (-5.0, -5.0) must be 1.0")
    {
        auto const normalizedVector = mz::normalizeVector<double>({-5., 5.});
        const double norm = mz::getVectorLength(normalizedVector);
        REQUIRE(mz::nearlyEquals(norm, 1.) == true);
    }
    
    SECTION("Norm of normalized vector (2.0, 2.0) must be 1.0")
    {
        auto const normalizedVector = mz::normalizeVector<double>({2., -2.});
        const double norm = mz::getVectorLength(normalizedVector);
        REQUIRE(mz::nearlyEquals(norm, 1.) == true);
    }
}

TEST_CASE("Compute length of vector")
{
    SECTION("Norm of vector (1.0, 1.0) must be sqrt of 2")
    {
        double length = mz::getVectorLength<double>({1.0, 1.0});
        REQUIRE(mz::nearlyEquals(length, M_SQRT2) == true);
    }
    
    SECTION("Norm of vector (-1.0, 1.0) must be sqrt of 2")
    {
        double length = mz::getVectorLength<double>({-1.0, 1.0});
        REQUIRE(mz::nearlyEquals(length, M_SQRT2) == true);
    }
    
    SECTION("Norm of vector (sqrt(2), -sqrt(2)) must be 2.0")
    {
        double length = mz::getVectorLength<double>({M_SQRT2, M_SQRT2});
        REQUIRE(mz::nearlyEquals(length, 2.0));
    }
}

TEST_CASE("Computes length squarred of vector")
{
    SECTION("Squarred norm of vector (1.0, 1.0) must be 2")
    {
        double length = mz::getVectorLength2<double>({1.0, 1.0});
        REQUIRE(mz::nearlyEquals(length, 2.0));
    }
    
    SECTION("Squarred norm of vector (0.0, 0.0) must be 0")
    {
        double length = mz::getVectorLength2<double>({0., 0.});
        REQUIRE(mz::nearlyEquals(length, 0.));
    }
}
