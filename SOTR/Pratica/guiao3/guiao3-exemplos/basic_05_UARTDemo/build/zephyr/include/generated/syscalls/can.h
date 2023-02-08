/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_CAN_H
#define Z_INCLUDE_SYSCALLS_CAN_H


#include <tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#include <linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_can_get_core_clock(const struct device * dev, uint32_t * rate);

__pinned_func
static inline int can_get_core_clock(const struct device * dev, uint32_t * rate)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t * val; } parm1 = { .val = rate };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CAN_GET_CORE_CLOCK);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_core_clock(dev, rate);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_core_clock(dev, rate) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_CORE_CLOCK, can_get_core_clock, dev, rate); 	retval = can_get_core_clock(dev, rate); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_CORE_CLOCK, can_get_core_clock, dev, rate, retval); 	retval; })
#endif
#endif


extern int z_impl_can_get_max_bitrate(const struct device * dev, uint32_t * max_bitrate);

__pinned_func
static inline int can_get_max_bitrate(const struct device * dev, uint32_t * max_bitrate)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t * val; } parm1 = { .val = max_bitrate };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CAN_GET_MAX_BITRATE);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_max_bitrate(dev, max_bitrate);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_max_bitrate(dev, max_bitrate) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_MAX_BITRATE, can_get_max_bitrate, dev, max_bitrate); 	retval = can_get_max_bitrate(dev, max_bitrate); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_MAX_BITRATE, can_get_max_bitrate, dev, max_bitrate, retval); 	retval; })
#endif
#endif


extern const struct can_timing * z_impl_can_get_timing_min(const struct device * dev);

__pinned_func
static inline const struct can_timing * can_get_timing_min(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		return (const struct can_timing *) arch_syscall_invoke1(parm0.x, K_SYSCALL_CAN_GET_TIMING_MIN);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_timing_min(dev);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_timing_min(dev) ({ 	const struct can_timing * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_TIMING_MIN, can_get_timing_min, dev); 	retval = can_get_timing_min(dev); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_TIMING_MIN, can_get_timing_min, dev, retval); 	retval; })
#endif
#endif


extern const struct can_timing * z_impl_can_get_timing_max(const struct device * dev);

__pinned_func
static inline const struct can_timing * can_get_timing_max(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		return (const struct can_timing *) arch_syscall_invoke1(parm0.x, K_SYSCALL_CAN_GET_TIMING_MAX);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_timing_max(dev);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_timing_max(dev) ({ 	const struct can_timing * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_TIMING_MAX, can_get_timing_max, dev); 	retval = can_get_timing_max(dev); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_TIMING_MAX, can_get_timing_max, dev, retval); 	retval; })
#endif
#endif


extern int z_impl_can_calc_timing(const struct device * dev, struct can_timing * res, uint32_t bitrate, uint16_t sample_pnt);

__pinned_func
static inline int can_calc_timing(const struct device * dev, struct can_timing * res, uint32_t bitrate, uint16_t sample_pnt)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct can_timing * val; } parm1 = { .val = res };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = bitrate };
		union { uintptr_t x; uint16_t val; } parm3 = { .val = sample_pnt };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_CAN_CALC_TIMING);
	}
#endif
	compiler_barrier();
	return z_impl_can_calc_timing(dev, res, bitrate, sample_pnt);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_calc_timing(dev, res, bitrate, sample_pnt) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_CALC_TIMING, can_calc_timing, dev, res, bitrate, sample_pnt); 	retval = can_calc_timing(dev, res, bitrate, sample_pnt); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_CALC_TIMING, can_calc_timing, dev, res, bitrate, sample_pnt, retval); 	retval; })
#endif
#endif


extern const struct can_timing * z_impl_can_get_timing_min_data(const struct device * dev);

__pinned_func
static inline const struct can_timing * can_get_timing_min_data(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		return (const struct can_timing *) arch_syscall_invoke1(parm0.x, K_SYSCALL_CAN_GET_TIMING_MIN_DATA);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_timing_min_data(dev);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_timing_min_data(dev) ({ 	const struct can_timing * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_TIMING_MIN_DATA, can_get_timing_min_data, dev); 	retval = can_get_timing_min_data(dev); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_TIMING_MIN_DATA, can_get_timing_min_data, dev, retval); 	retval; })
#endif
#endif


extern const struct can_timing * z_impl_can_get_timing_max_data(const struct device * dev);

__pinned_func
static inline const struct can_timing * can_get_timing_max_data(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		return (const struct can_timing *) arch_syscall_invoke1(parm0.x, K_SYSCALL_CAN_GET_TIMING_MAX_DATA);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_timing_max_data(dev);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_timing_max_data(dev) ({ 	const struct can_timing * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_TIMING_MAX_DATA, can_get_timing_max_data, dev); 	retval = can_get_timing_max_data(dev); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_TIMING_MAX_DATA, can_get_timing_max_data, dev, retval); 	retval; })
#endif
#endif


extern int z_impl_can_calc_timing_data(const struct device * dev, struct can_timing * res, uint32_t bitrate, uint16_t sample_pnt);

__pinned_func
static inline int can_calc_timing_data(const struct device * dev, struct can_timing * res, uint32_t bitrate, uint16_t sample_pnt)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct can_timing * val; } parm1 = { .val = res };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = bitrate };
		union { uintptr_t x; uint16_t val; } parm3 = { .val = sample_pnt };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_CAN_CALC_TIMING_DATA);
	}
#endif
	compiler_barrier();
	return z_impl_can_calc_timing_data(dev, res, bitrate, sample_pnt);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_calc_timing_data(dev, res, bitrate, sample_pnt) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_CALC_TIMING_DATA, can_calc_timing_data, dev, res, bitrate, sample_pnt); 	retval = can_calc_timing_data(dev, res, bitrate, sample_pnt); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_CALC_TIMING_DATA, can_calc_timing_data, dev, res, bitrate, sample_pnt, retval); 	retval; })
#endif
#endif


extern int z_impl_can_set_timing(const struct device * dev, const struct can_timing * timing, const struct can_timing * timing_data);

__pinned_func
static inline int can_set_timing(const struct device * dev, const struct can_timing * timing, const struct can_timing * timing_data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const struct can_timing * val; } parm1 = { .val = timing };
		union { uintptr_t x; const struct can_timing * val; } parm2 = { .val = timing_data };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CAN_SET_TIMING);
	}
#endif
	compiler_barrier();
	return z_impl_can_set_timing(dev, timing, timing_data);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_set_timing(dev, timing, timing_data) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_SET_TIMING, can_set_timing, dev, timing, timing_data); 	retval = can_set_timing(dev, timing, timing_data); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_SET_TIMING, can_set_timing, dev, timing, timing_data, retval); 	retval; })
#endif
#endif


extern int z_impl_can_set_mode(const struct device * dev, enum can_mode mode);

__pinned_func
static inline int can_set_mode(const struct device * dev, enum can_mode mode)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum can_mode val; } parm1 = { .val = mode };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CAN_SET_MODE);
	}
#endif
	compiler_barrier();
	return z_impl_can_set_mode(dev, mode);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_set_mode(dev, mode) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_SET_MODE, can_set_mode, dev, mode); 	retval = can_set_mode(dev, mode); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_SET_MODE, can_set_mode, dev, mode, retval); 	retval; })
#endif
#endif


extern int z_impl_can_set_bitrate(const struct device * dev, uint32_t bitrate, uint32_t bitrate_data);

__pinned_func
static inline int can_set_bitrate(const struct device * dev, uint32_t bitrate, uint32_t bitrate_data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t val; } parm1 = { .val = bitrate };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = bitrate_data };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CAN_SET_BITRATE);
	}
#endif
	compiler_barrier();
	return z_impl_can_set_bitrate(dev, bitrate, bitrate_data);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_set_bitrate(dev, bitrate, bitrate_data) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_SET_BITRATE, can_set_bitrate, dev, bitrate, bitrate_data); 	retval = can_set_bitrate(dev, bitrate, bitrate_data); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_SET_BITRATE, can_set_bitrate, dev, bitrate, bitrate_data, retval); 	retval; })
#endif
#endif


extern int z_impl_can_send(const struct device * dev, const struct zcan_frame * frame, k_timeout_t timeout, can_tx_callback_t callback, void * user_data);

__pinned_func
static inline int can_send(const struct device * dev, const struct zcan_frame * frame, k_timeout_t timeout, can_tx_callback_t callback, void * user_data)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const struct zcan_frame * val; } parm1 = { .val = frame };
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm2 = { .val = timeout };
		union { uintptr_t x; can_tx_callback_t val; } parm3 = { .val = callback };
		union { uintptr_t x; void * val; } parm4 = { .val = user_data };
		return (int) arch_syscall_invoke6(parm0.x, parm1.x, parm2.split.lo, parm2.split.hi, parm3.x, parm4.x, K_SYSCALL_CAN_SEND);
	}
#endif
	compiler_barrier();
	return z_impl_can_send(dev, frame, timeout, callback, user_data);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_send(dev, frame, timeout, callback, user_data) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_SEND, can_send, dev, frame, timeout, callback, user_data); 	retval = can_send(dev, frame, timeout, callback, user_data); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_SEND, can_send, dev, frame, timeout, callback, user_data, retval); 	retval; })
#endif
#endif


extern int z_impl_can_add_rx_filter_msgq(const struct device * dev, struct k_msgq * msgq, const struct zcan_filter * filter);

__pinned_func
static inline int can_add_rx_filter_msgq(const struct device * dev, struct k_msgq * msgq, const struct zcan_filter * filter)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct k_msgq * val; } parm1 = { .val = msgq };
		union { uintptr_t x; const struct zcan_filter * val; } parm2 = { .val = filter };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CAN_ADD_RX_FILTER_MSGQ);
	}
#endif
	compiler_barrier();
	return z_impl_can_add_rx_filter_msgq(dev, msgq, filter);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_add_rx_filter_msgq(dev, msgq, filter) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_ADD_RX_FILTER_MSGQ, can_add_rx_filter_msgq, dev, msgq, filter); 	retval = can_add_rx_filter_msgq(dev, msgq, filter); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_ADD_RX_FILTER_MSGQ, can_add_rx_filter_msgq, dev, msgq, filter, retval); 	retval; })
#endif
#endif


extern void z_impl_can_remove_rx_filter(const struct device * dev, int filter_id);

__pinned_func
static inline void can_remove_rx_filter(const struct device * dev, int filter_id)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; int val; } parm1 = { .val = filter_id };
		(void) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CAN_REMOVE_RX_FILTER);
		return;
	}
#endif
	compiler_barrier();
	z_impl_can_remove_rx_filter(dev, filter_id);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_remove_rx_filter(dev, filter_id) do { 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_REMOVE_RX_FILTER, can_remove_rx_filter, dev, filter_id); 	can_remove_rx_filter(dev, filter_id); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_REMOVE_RX_FILTER, can_remove_rx_filter, dev, filter_id); } while(false)
#endif
#endif


extern int z_impl_can_get_max_filters(const struct device * dev, enum can_ide id_type);

__pinned_func
static inline int can_get_max_filters(const struct device * dev, enum can_ide id_type)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum can_ide val; } parm1 = { .val = id_type };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CAN_GET_MAX_FILTERS);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_max_filters(dev, id_type);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_max_filters(dev, id_type) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_MAX_FILTERS, can_get_max_filters, dev, id_type); 	retval = can_get_max_filters(dev, id_type); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_MAX_FILTERS, can_get_max_filters, dev, id_type, retval); 	retval; })
#endif
#endif


extern int z_impl_can_get_state(const struct device * dev, enum can_state * state, struct can_bus_err_cnt * err_cnt);

__pinned_func
static inline int can_get_state(const struct device * dev, enum can_state * state, struct can_bus_err_cnt * err_cnt)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum can_state * val; } parm1 = { .val = state };
		union { uintptr_t x; struct can_bus_err_cnt * val; } parm2 = { .val = err_cnt };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CAN_GET_STATE);
	}
#endif
	compiler_barrier();
	return z_impl_can_get_state(dev, state, err_cnt);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_get_state(dev, state, err_cnt) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_GET_STATE, can_get_state, dev, state, err_cnt); 	retval = can_get_state(dev, state, err_cnt); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_GET_STATE, can_get_state, dev, state, err_cnt, retval); 	retval; })
#endif
#endif


extern int z_impl_can_recover(const struct device * dev, k_timeout_t timeout);

__pinned_func
static inline int can_recover(const struct device * dev, k_timeout_t timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { struct { uintptr_t lo, hi; } split; k_timeout_t val; } parm1 = { .val = timeout };
		return (int) arch_syscall_invoke3(parm0.x, parm1.split.lo, parm1.split.hi, K_SYSCALL_CAN_RECOVER);
	}
#endif
	compiler_barrier();
	return z_impl_can_recover(dev, timeout);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define can_recover(dev, timeout) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CAN_RECOVER, can_recover, dev, timeout); 	retval = can_recover(dev, timeout); 	sys_port_trace_syscall_exit(K_SYSCALL_CAN_RECOVER, can_recover, dev, timeout, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */