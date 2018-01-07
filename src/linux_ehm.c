#include <platform.h>
#include <ehm.h>
#include <ehm_assert.h>

// needed to ensure the linker has a reference to ehm_throw and friends.
void dummy_throw()
{
    EHM_TRY {
        EHM_ASSERT(1==0);
    } EHM_CATCH_ALL {
        ;
    } EHM_ENDTRY;
}
