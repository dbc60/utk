#include <platform.h>
#include <ehm.h>

// needed to ensure the linker has a reference to ehm_throw and friends.
void dummy_throw()
{
    b32 test = FALSE;
    EHM_TRY {
        EHM_ASSERT(test);
    } EHM_CATCH_ALL {
        ;
    } EHM_ENDTRY;
}
