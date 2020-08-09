#include "tests.h"
int run_tests(){
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("PROCESO2 Suite",NULL,NULL);
    CU_add_test(tests,"Probar Suma", suma_proceso2);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
void suma_proceso2(){
    CU_ASSERT_EQUAL(2+2, 4);
}