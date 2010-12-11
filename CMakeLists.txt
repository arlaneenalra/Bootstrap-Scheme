
project(bootstrap C)

cmake_minimum_required(VERSION 2.8.1)

INCLUDE(CPack)
INCLUDE(CTest)

set(UPDATE_TYPE git)

set(VERSION 0.2.0)

add_subdirectory(src)

ADD_TEST(with-exception-handler src/bootstrap test/test_with_exception_handler.scm)
ADD_TEST(for-each src/bootstrap test/test_foreach.scm)
ADD_TEST(map src/bootstrap test/test_map.scm)
ADD_TEST(and_or src/bootstrap test/test_and_or.scm)
ADD_TEST(car_cdr src/bootstrap test/test_car_cdr.scm)
ADD_TEST(equal src/bootstrap test/test_equal.scm)
ADD_TEST(let src/bootstrap test/test_let.scm)
ADD_TEST(vector src/bootstrap test/test_vector.scm)
ADD_TEST(macros src/bootstrap test/test_macros.scm)
ADD_TEST(list src/bootstrap test/test_list.scm)
ADD_TEST(apply src/bootstrap test/test_apply.scm)


