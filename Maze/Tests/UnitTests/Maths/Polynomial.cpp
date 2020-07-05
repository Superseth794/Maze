//
//  Polynomial.cpp
//  Maze
//
//  Created by Jaraxus on 25/04/2020.
//

#include "../../../External/Catch/Catch.hpp"

#include "../../../Utils/Polynomial.hpp"

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

TEST_CASE("polynomial of degree 0 with doubles", "[math]") {
    
    GIVEN("polynomial \"2.0\"") {
        const mz::Polynomial<2> poly{2.};
        
        THEN("description must be \"2.0\"") {
            REQUIRE(poly.display(1) == "2.0");
        }
        
        THEN("coeficient for degree 0 must be equal to 2.0") {
            REQUIRE(poly.getCoeficient(0) == 2.);
        }
        
        THEN("computation on any number must be equal to 2.0") {
            REQUIRE(poly.compute(4.) == 2.0);
            REQUIRE(poly.compute(-12.) == 2.0);
            REQUIRE(poly.compute(0.) == 2.0);
        }
    }
}

TEST_CASE("polynomial of degree 1 with doubles", "[math]") {
    
    GIVEN("polynomial \"10.0x + 7.0\"") {
        const mz::Polynomial<1> poly{10.0, 7.0};
        
        THEN("description must be \"10.0x + 7.0\"") {
            REQUIRE(poly.display(1) == "10.0x + 7.0");
        }
        
        THEN("coeficient for degree 0 must be equal to 7.0") {
            REQUIRE(poly.getCoeficient(0) == 7.);
        }
        
        THEN("coeficient for degree 1 must be equal to 10.0") {
            REQUIRE(poly.getCoeficient(1) == 10.);
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
    }
    
    GIVEN("polynomial \"10x\"") {
        const mz::Polynomial<1> poly{10.0, 0};
        
        THEN("description must be \"10.0x\"") {
            REQUIRE(poly.display(1) == "10.0x");
        }
        
        THEN("coeficient for degree 0 must be equal to 0.0") {
            REQUIRE(poly.getCoeficient(0) == 0.);
        }
        
        THEN("coeficient for degree 1 must be equal to 10.0") {
            REQUIRE(poly.getCoeficient(1) == 10.);
        }
        
        THEN("computation on 0 must be equal to 0") {
            REQUIRE(poly.compute(0.) == 0.);
        }
    }
}

TEST_CASE("polynomial of degree 2 with doubles", "[math]") {
    
    GIVEN("polynomial \"2x^2 + 7x - 3\"") {
        const mz::Polynomial<2> poly{2, 7, -3};
        
        THEN("description must be \"2.0x^2 + 7.0x - 3.0\"") {
            REQUIRE(poly.display(1) == "2.0x^2 + 7.0x - 3.0");
        }
        
        THEN("computation on -4 must be equal to 1") {
            REQUIRE(poly.compute(-4.0) == 1.0);
        }
        
        THEN("computation on 18 must be equal to 771") {
            REQUIRE(poly.compute(18.0) == 771.0);
        }
        
        THEN("computation on 3 must be equal to 36") {
            REQUIRE(poly.compute(3) == 36.0);
        }
        
//        WHEN("roots of type <complex<double>> are required") {
//            THEN("roots must be equal to -3.8860009363 and 0.386000963") {
//                auto roots {poly.findRoots<std::complex<double>>()};
//                constexpr double firstExpectedRoot = -3.8860009363;
//                constexpr double secondExpectedRoot = 0.386000963;
//                REQUIRE(std::abs(roots[0].real() - firstExpectedRoot) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(roots[0].imag() == 0.);
//                REQUIRE(std::abs(roots[1].real() - secondExpectedRoot) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(roots[0].imag() == 0.);
//            }
//        }
//
//        WHEN("roots of type <double> are required") {
//            THEN("roots must be equal to -3.8860009363 and 0.386000963") {
//                auto roots {poly.findRoots()};
//                REQUIRE(std::abs(roots[0] - (-3.8860009363)) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(std::abs(roots[1] - 0.386000963) <= std::numeric_limits<float>::epsilon());
//            }
//        }
    }
}

TEST_CASE("polynomial of degree 3 with doubles", "[math]") {
    
    GIVEN("polynomial \"-9x^3 - 21.7x^2 + 16.1x + 3.3\"") {
        const mz::Polynomial<3> poly{-9, -21.7, 16.1, 3.3};
        
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
        
//        WHEN("roots of type <double> are required") {
//            THEN("there must be a single root equal to -2.971585615") {
//                auto roots {poly.findRoots<double>()};
//                REQUIRE(roots.size() >= 1); // TODO
//                REQUIRE(roots[0] == -2.971585615);
//                REQUIRE(poly.compute(roots[0]) == 0.);
//            }
//        }
    }
    
    GIVEN("polynomial \"x^3\"") {
        const mz::Polynomial<3> poly{1., 0., 0., 0.};
        
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

SCENARIO("polynomials must be default-initialized with 0.", "[math]") {
    
    GIVEN("polynomial of degree 7 with [double] default-initialized") {
        mz::Polynomial<7> poly {};
        
        THEN("polynomial must have 8 coeficients") {
            std::size_t coeficientCount = 0;
            for ([[maybe_unused]] auto const& coef : poly) {
                ++coeficientCount;
            }
            REQUIRE(coeficientCount == 8);
        }
        
        THEN("all coeficients must be equal to 0") {
            for (auto const& coef : poly) {
                REQUIRE(coef == 0);
            }
        }
    }
}

SCENARIO("polynomials can be partially initialized", "[math]") {
    GIVEN("polynomial of degree 3 \"2.0x^2 -3.0x + 7.0\"") {
        mz::Polynomial<3> partialPoly {2., -3., 7.};
        
        THEN("polynomial must have 4 coeficients") {
            std::size_t coeficientsCount = 0;
            for ([[maybe_unused]] auto const& coef : partialPoly) {
                ++coeficientsCount;
            }
            REQUIRE(coeficientsCount == 4);
        }
        
        THEN("non specified coeficients must be equal to 0.") {
            REQUIRE(partialPoly.getCoeficient(3) == 0.);
        }
    }
    
    GIVEN("polynomial of degree 1 \"-2.0\"") {
        const mz::Polynomial<4> partialPoly {-2.};
        
        THEN("non specified coeficients must be equal to 0.") {
            REQUIRE(partialPoly.getCoeficient(4) == 0.);
            REQUIRE(partialPoly.getCoeficient(3) == 0.);
            REQUIRE(partialPoly.getCoeficient(2) == 0.);
            REQUIRE(partialPoly.getCoeficient(1) == 0.);
        }
    }
    
    GIVEN("polynomial of degree 4 \"12.0x + x\"") {
        const mz::Polynomial<2> partialPoly {12., 1.};
        
        THEN("non specified coeficients must be equal to 0.") {
            REQUIRE(partialPoly.getCoeficient(2) == 0.);
        }
    }
}

SCENARIO("we can access the specific polynomial's coeficient associated to a given degree") {
    
    GIVEN("polynomial of degree 2 with [double] \"1.3x^2 - 9x + 0.1\"") {
        mz::Polynomial<2> poly {1.3, -9, 0.1};
        
        THEN("coeficient for degree 0 must be equal to 0.1") {
            REQUIRE(poly.getCoeficient(0) == 0.1);
        }
        
        THEN("coeficient for degree 1 must be equal to -9.0") {
            REQUIRE(poly.getCoeficient(1) == -9.0);
        }
        
        THEN("coeficient for degree 2 must be equal to 1.3") {
            REQUIRE(poly.getCoeficient(2) == 1.3);
        }
        
        WHEN("coeficient for degree 0 is set to 3.7") {
            poly.setCoeficient(0, 3.7);
            
            THEN("coeficient for degree 0 must be equal to 3.7") {
                REQUIRE(poly.getCoeficient(0) == 3.7);
            }
        }
        
        WHEN("coeficient for degree 1 is set to -139.0") {
            poly.setCoeficient(1, -139.);
            
            THEN("coeficient for degree 1 must be equal to -139.0") {
                REQUIRE(poly.getCoeficient(1) == -139.);
            }
        }
        
        WHEN("coeficient for degree 2 is set to 17.2") {
            poly.setCoeficient(2, 17.2);
            
            THEN("coeficient for degree 2 must be equal to 17.2") {
                REQUIRE(poly.getCoeficient(2) == 17.2);
            }
        }
    }
    
    GIVEN("polynomial of degree 1 with [double] \"12.0x + 5.0\"") {
        mz::Polynomial<1> poly {12., 5.};
        
        THEN("coeficient for degree 0 must be equal to 5.0") {
            REQUIRE(poly.getCoeficient(0) == 5.);
        }
        
        THEN("coeficient for degree 1 must be equal to 12.0") {
            REQUIRE(poly.getCoeficient(1) == 12.);
        }
        
        WHEN("coeficient for degree 0 is set to -21.0") {
            poly.setCoeficient(0, -21.);
            
            THEN("coeficient for degree 0 must be equal to -21.0") {
                REQUIRE(poly.getCoeficient(0) == -21.);
            }
        }
        
        WHEN("coeficient for degree 1 is set to -9.0") {
            poly.setCoeficient(1, -9.);
            
            THEN("coeficient for degree 1 must be equal to -9.0") {
                REQUIRE(poly.getCoeficient(1) == -9.);
            }
        }
    }
}

SCENARIO("polynomials can compute values by substituting it as the variable") {
    
    GIVEN("polynomial of degree 6 with [double] \"31.2x^6 + 12.7x^5 - 3.0x^4 + 2.0x^2 - x + 9.0\"") {
        const mz::Polynomial<6> poly {31.2, 12.7, -3., 0., 2., -1., 9.};
        
        THEN("computation of [double] 2.0 must be equal to 2370.2") {
            REQUIRE(poly.compute(2.) == 2370.2);
        }
        
        THEN("computation of [double] -1.0 must be equal to 27.5") {
            REQUIRE(poly.compute(-1.) == 27.5);
        }
        
        THEN("computation of [double] 0. must be equal to 9.0") {
            REQUIRE(poly.compute(0.) == 9.0);
        }
        
        THEN("computation of [double] 16.0 must be equal to 536569951.4") {
            REQUIRE(poly.compute(16.) == 536569951.4);
        }
    }
    
    GIVEN("polynomial of degree 2 with [double] \"2.0x^2 + 1.0\"") {
        const mz::Polynomial<2> poly {2., 0., 1.};
        
        THEN("computation of [std::complex<double>] sqrt(2)/2i must equal to 0.") {
            auto result {poly.compute<std::complex<double>>(std::complex<double>{0., M_SQRT2 / 2})};
            REQUIRE(std::abs(result.real()) <= std::numeric_limits<float>::epsilon());
            REQUIRE(std::abs(result.imag()) <= std::numeric_limits<float>::epsilon());
        }
        
        THEN("computation of [std::complex<double>] -sqrt(2)/2i must equal to 0.") {
            auto result {poly.compute<std::complex<double>>(std::complex<double>{0., -M_SQRT2 / 2})};
            REQUIRE(std::abs(result.real()) <= std::numeric_limits<float>::epsilon());
            REQUIRE(std::abs(result.imag()) <= std::numeric_limits<float>::epsilon());
        }
        
        THEN("computation of [std::complex<double>] 3.0 + 2.0i must equal to 11.0 + 24.0i") {
            auto result {poly.compute<std::complex<double>>(std::complex<double>{3, 2})};
            REQUIRE(std::abs(result.real() - 11.) <= std::numeric_limits<float>::epsilon());
            REQUIRE(std::abs(result.imag() - 24.) <= std::numeric_limits<float>::epsilon());
        }
    }
}

SCENARIO("polynomials can display themselves") {
    
    GIVEN("polynomials of degree 0 with [double] \"10.0\"") {
        const mz::Polynomial<0> poly {10.};
        
        THEN("polynomial descrition must be \"10.0\"") {
            REQUIRE(poly.display(1) == "10.0");
        }
    }
    
    GIVEN("polynomials of degree 0 with [double] \"0.0\"") {
        const mz::Polynomial<0> poly {0.};
        
        THEN("polynomial descrition must be \"0.0\"") {
            REQUIRE(poly.display(1) == "0.0");
        }
    }
    
    GIVEN("polynomials of degree 1 with [double] \"-7.0x + 3.0\"") {
        const mz::Polynomial<1> poly {-7., 3.};
        
        THEN("polynomial descrition must be \"-7.0x + 3.0\"") {
            REQUIRE(poly.display(1) == "-7.0x + 3.0");
        }
    }
    
    GIVEN("polynomials of degree 1 with [double] \"-7.0x\"") {
        const mz::Polynomial<1> poly {-7., 0.};
        
        THEN("polynomial descrition must be \"-7.0x\"") {
            REQUIRE(poly.display(1) == "-7.0x");
        }
    }
    
    GIVEN("polynomials of degree 1 with [double] \"21.0\"") {
        const mz::Polynomial<1> poly {0., 21.};
        
        THEN("polynomial descrition must be \"21.0\"") {
            REQUIRE(poly.display(1) == "21.0");
        }
    }
    
    GIVEN("polynomials of degree 5 with [double] \"-2.235x^5 + 3.128x^4 + x^2 - 9.152x - 5.0\"") {
        const mz::Polynomial<5> poly {-2.235, 3.128, 0., 1., -9.152, -5.};
        
        THEN("polynomial descrition with 3 precision digits must be \"-2.235x^5 + 3.128x^4 + x^2 - 9.152x - 5.0\"") {
            REQUIRE(poly.display(3) == "-2.235x^5 + 3.128x^4 + x^2 - 9.152x - 5.000");
        }
        
        THEN("polynomial descrition with 0 precision digits must be \"-2.235x^5 + 3.128x^4 + x^2 - 9.152x - 5.0\"") {
            REQUIRE(poly.display(0) == "-2x^5 + 3x^4 + x^2 - 9x - 5");
        }
    }
    
    GIVEN("polynomials of degree 4 with [double] \"x^4 + x^2 + 1.0\"") {
        const mz::Polynomial<4> poly {1., 0., 1., 0., 1.};
        
        THEN("polynomial descrition with 1 precision digits must be \"x^4 + x^2 + 1.0\"") {
            REQUIRE(poly.display(1) == "x^4 + x^2 + 1.0");
        }
    }
}

SCENARIO("Polynomials of degree <= 4 can compute their roots", "[math], [algorithm]") {
    
    GIVEN("polynomial of degree 0 with [double] \"3.2\"") {
        const mz::Polynomial<0> poly {3.2};
        
        WHEN("roots [double] are computed") {
            THEN("no roots must be found") {
                REQUIRE(poly.findRoots().size() == 0);
            }
        }
    }
    
    GIVEN("polynomial of degree 0 with [double] \"0.0\"") {
        const mz::Polynomial<0> poly {0.};
        
        WHEN("roots [double] are computed") {
            THEN("no roots must be found") {
                REQUIRE(poly.findRoots().size() == 0);
            }
        }
    }
    
    GIVEN("polynomial of degree 1 with [double] \"-2x + 6\"") {
        const mz::Polynomial<1> poly {-2, 6};
        
        WHEN("roots [double] are computed") {
            THEN("a single root equal to 3.0 must be found") {
                auto roots {poly.findRoots()};
                REQUIRE(roots.size() == 1);
                REQUIRE(roots[0] == 3.0);
            }
        }
        
        WHEN("roots [std::complex<double>] are computed") {
            THEN("a single root equal to 3.0 + 0.0i must be found") {
                auto roots {poly.findRoots<std::complex<double>>()};
                REQUIRE(roots.size() == 1);
                REQUIRE(roots[0].real() == 3.0);
                REQUIRE(roots[0].imag() == 0.0);
            }
        }
    }
    
//    GIVEN("polynomial of degree 2 with [double] \"4x^2 - 8x\"") {
//        const mz::Polynomial<2> poly {4, -8, 0.};
//
//        WHEN("roots [double] are computed") {
//            THEN("two roots 0.0 and 2.0 must be found") {
//                auto roots {poly.findRoots()};
//                REQUIRE(roots.size() == 2);
//                REQUIRE(roots[0] == 0.);
//                REQUIRE(roots[1] == 2.);
//            }
//        }
//
//        WHEN("roots [std::complex<double>] are computed") {
//            THEN("two roots 0.0 + 0.0i and 2.0 + 0.0i must be found") {
//                auto roots {poly.findRoots<std::complex<double>>()};
//                REQUIRE(roots.size() == 2);
//                REQUIRE(roots[0].real() == 0.);
//                REQUIRE(roots[0].imag() == 0.);
//                REQUIRE(roots[1].real() == 2.);
//                REQUIRE(roots[0].imag() == 0.);
//            }
//        }
//    }
    
//    GIVEN("polynomial of degree 2 with [double] \"x^2\"") {
//        const mz::Polynomial<2> poly {1., 0., 0.};
//
//        WHEN("roots [double] are computed") {
//            THEN("a single root 0.0 must be found") {
//                auto roots {poly.findRoots()};
//                REQUIRE(roots.size() == 1);
//                REQUIRE(roots[0] == 0.);
//            }
//        }
//
//        WHEN("roots [std::complex<double>] are computed") {
//            auto roots {poly.findRoots<std::complex<double>>()};
//
//            THEN("a single root 0.0 + 0.0i must be found") {
//                REQUIRE(roots.size() == 1);
//                REQUIRE(std::abs(roots[0].real()) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(std::abs(roots[0].imag()) <= std::numeric_limits<float>::epsilon());
//            }
//        }
//    }
    
//    GIVEN("polynomial of degree 2 with [double] \"x^2 + 1.0\"") {
//        const mz::Polynomial<2> poly {1., 0., 1.};
//
//        WHEN("roots [double] are computed") {
//            THEN("no root must be found") {
//                REQUIRE(poly.findRoots().size() == 0);
//            }
//        }
//
//        WHEN("roots [std::complex<double>] are computed") {
//            auto roots {poly.findRoots<std::complex<double>>()};
//
//            THEN("two roots -i and i must be found") {
//                REQUIRE(roots.size() == 2);
//                REQUIRE(std::abs(roots[0].real()) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(std::abs(roots[0].imag() + 1.0) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(std::abs(roots[1].real()) <= std::numeric_limits<float>::epsilon());
//                REQUIRE(std::abs(roots[1].imag() - 1.0) <= std::numeric_limits<float>::epsilon());
//            }
//        }
//    }
    
//    GIVEN("polynomial of degree 3 with [double] \"3.0x^3 - 4.0x^2 + x + 2.0\"") {
//        const mz::Polynomial<3> poly {3., -4., 1., 2.};
//
//        WHEN("roots [std::complex<double>] are computed") {
//            auto roots {poly.findRoots<std::complex<double>>()};
//
//            THEN("one root must be found") {
//                REQUIRE(roots.size() == 1);
//            }
//
//            THEN("root must be solutions of the polynomial") {
//                for (auto const& root : roots) {
//                    REQUIRE(std::abs(poly.compute<std::complex<double>>(root)) <= std::numeric_limits<float>::epsilon());
//                }
//            }
//        }
//    }
}
