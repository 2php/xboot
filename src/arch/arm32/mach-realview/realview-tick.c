/*
 * realview-tick.c
 *
 * realview tick timer, using arm sp804 timer module.
 *
 * Copyright(c) 2007-2014 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <realview/reg-timer.h>

/*
 * tick timer interrupt.
 */
static void timer_interrupt(void * data)
{
	tick_interrupt();

	/* clear interrupt - timer3 */
	writel(REALVIEW_T3_ICLR, 0x0);
}

static bool_t tick_timer_init(void)
{
	u64_t timclk;
	u32_t count;

	if(!clk_get_rate("timclk", &timclk))
	{
		LOG("can't get the clock of \'timclk\'");
		return FALSE;
	}

	/* for 1ms reload count */
	count = (u32_t)(timclk / 1000);

	if(!request_irq("TMIER2_3", timer_interrupt, NULL))
	{
		LOG("can't request irq \'TMIER2_3\'");
		return FALSE;
	}

	/* using timer3 for tick, 1ms for reload value */
	writel(REALVIEW_T3_LOAD, count);

	/* setting timer controller */
	writel(REALVIEW_T3_CTRL, REALVIEW_TC_32BIT | REALVIEW_TC_DIV1 | REALVIEW_TC_IE | REALVIEW_TC_PERIODIC);

	/* clear all interrupt */
	writel(REALVIEW_T3_ICLR, 0x0);

	/* enable timer3 */
	writel(REALVIEW_T3_CTRL, readl(REALVIEW_T3_CTRL) | REALVIEW_TC_ENABLE);

	return TRUE;
}

static struct tick_t realview_tick = {
	.hz		= 1000,
	.init	= tick_timer_init,
};

static __init void realview_tick_init(void)
{
	if(register_tick(&realview_tick))
		LOG("Register tick");
	else
		LOG("Failed to register tick");
}
core_initcall(realview_tick_init);
