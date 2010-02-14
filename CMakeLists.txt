
project(bootstrap C)

cmake_minimum_required(VERSION 2.6)

INCLUDE(CPack)
INCLUDE(CTest)

set(UPDATE_TYPE git)

set(VERSION 0.2.0)

add_subdirectory(src)

ADD_TEST(for-each src/bootstrap test/test_foreach.scm)
ADD_TEST(and_or src/bootstrap test/test_and_or.scm)
ADD_TEST(car_cdr src/bootstrap test/test_car_cdr.scm)
ADD_TEST(equal src/bootstrap test/test_equal.scm)
