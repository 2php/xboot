#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

bool_t istimeout(u64_t start, u64_t offset);
void udelay(u32_t us);
void mdelay(u32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H__ */