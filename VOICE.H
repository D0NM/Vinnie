#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int pascal far PVOICE_INIT(void);
int pascal far PVOICE_CLEANUP(void);
int pascal far PVOICE_START(unsigned char far *,
                             long,
                             unsigned int,
                             unsigned int,
                             long,
                             long);
int pascal far PVOICE_CATCHUP(void);
void pascal far PVOICE_STOP(void);
int pascal far PVOICE_STATUS(long far *,long far *);
void pascal far PVOICE_CBREAK(void);

#ifdef __cplusplus
}
#endif
