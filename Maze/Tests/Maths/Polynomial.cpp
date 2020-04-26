//
//  Polynomial.cpp
//  Maze
//
//  Created by Jaraxus on 25/04/2020.
//

#include "../../External/Catch/Catch.hpp"

#include "../../Utils/Polynomial.hpp"

TEST_CASE("polynomial with null coeficients") {
    
    GIVEN("polynomial of degree 3 with default initialisation") {
        mz::Polynomial<3> poly{};
        
        THEN("description must be \"0.0\"") {
            REQUIRE(poly.display(1) == "0.0");
        }
        THEN("all coeficients must be null") {
            for (auto coef : poly) {
                REQUIRE(coef == 0.);
            }
        }
        THEN("any computation must return 0 value") {
            REQUIRE(poly.compute(14.) == 0.);
            REQUIRE(poly.compute(-4.) == 0.);
            REQUIRE(poly.compute(0.) == 0.);
            REQUIRE(poly.compute(245687.) == 0.);
        }
    }
    
    GIVEN("polynomial of degree 0 with default initialisation") {
        mz::Polynomial<0> poly{};
        
        THEN("description must be \"0.0\"") {
            REQUIRE(poly.display(1) == "0.0");
        }
        THEN("all coeficients must be null") {
            for (auto coef : poly) {
                REQUIRE(coef == 0.);
            }
        }
        THEN("any computation must return 0 value") {
            REQUIRE(poly.compute(21.) == 0.);
            REQUIRE(poly.compute(-73.) == 0.);
            REQUIRE(poly.compute(0.) == 0.);
            REQUIRE(poly.compute(-2835.) == 0.);
        }
    }
}

TEST_CASE("polynomial of degree 1 with doubles", "[Math]") {
    
    GIVEN("polynomial \"10.0x + 7.0\"") {
        mz::Polynomial<1> poly{10.0, 7.0};
        
        THEN("description must be \"10.0x + 7.0\"") {
            REQUIRE(poly.display(1) == "10.0x + 7.0");
        }
        
        THEN("computation on 0 must be equal to 7") {
            REQUIRE(poly.compute(0.) == 7.);
        }
        
        THEN("computation on 1 must be equal to 17") {
            REQUIRE(poly.compute(1.) == 17.);
        }
        
        THEN("computation on -12 must be equal to 127") {
            REQUIRE(poly.compute(-12.) == -113.);
        }
        
        // TODO: roots of polynomial
    }
    
    GIVEN("polynomial \"10x\"") {
        mz::Polynomial<1> poly{10.0, 0};
        
        THEN("description must be \"10.0x\"") {
            REQUIRE(poly.display(1) == "10.0x");
        }
        
        THEN("computation on 0 must be equal to 0") {
            REQUIRE(poly.compute(0.) == 0.);
        }
    }
}

TEST_CASE("polynomial of degree 3 with doubles", "[math]") {
    
    GIVEN("polynomial \"-9x^3 - 21.7x^2 + 16.1x + 3.3\"") {
        mz::Polynomial<3> poly{-9, -21.7, 16.1, 3.3};
        
        THEN("description must be \"-9.0x^3 - 21.7x^2 + 16.1x + 3.3\"") {
            REQUIRE(poly.display(1) == "-9.0x^3 - 21.7x^2 + 16.1x + 3.3");
        }
        
        THEN("computation on 41 must be equal to -656103.3") {
            REQUIRE(poly.compute(41.) == -656103.3);
        }
        
        THEN("computation on -4 must be equal to 167.7") {
            REQUIRE(poly.compute(-4.) == 167.7);
        }
        
        THEN("computation on -13 must be equal to 15899.7") {
            REQUIRE(poly.compute(-13.) == 15899.7);
        }
    }
    
    GIVEN("polynomial \"x^3\"") {
        mz::Polynomial<3> poly{1, 0, 0, 0};
        
        THEN("description must be \"x^3\"") {
            REQUIRE(poly.display(1) == "x^3");
        }
        
        THEN("computation on 5 must be equal to 125") {
            REQUIRE(poly.compute(5.) == 125.);
        }
        
        THEN("computation on 10 must be equal to 1000") {
            REQUIRE(poly.compute(10.) == 1000.);
        }
        
        THEN("computation on -13 must be equal to -2197") {
            REQUIRE(poly.compute(-13.) == -2197.);
        }
    }
}
