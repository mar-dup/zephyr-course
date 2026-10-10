/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* Precondition: push(99) */
	zassert_ok(rb_push(99), "Push needs to be possible, but failed.");

	/* now reset */
	zassert_ok(rb_init(4), "Re-init with 4 needs to pass");

	/* test if empty and size == 0 */
	zassert_true(rb_is_empty(), "Re-init buffer must be empty");
	zassert_equal(rb_count(), 0, "Re-init buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int v = 0xff;

	/* push, pop, is_empty */
	zassert_ok(rb_push(42), "push needs to be successful");
	zassert_ok(rb_pop(&v), "pop needs to return ok");
	zexpect_equal(v, 42, "expected hodled value is 42");
	zexpect_true(rb_is_empty(), "expected empty ring buffer");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int v = 0xff;

	/* push 1,2,3; then verify that the pop order is 1,2,3 */
	zassert_ok(rb_push(1), "push needs to be successful");
	zassert_ok(rb_push(2), "push needs to be successful");
	zassert_ok(rb_push(3), "push needs to be successful");

	/* 3 times: pop check return value and popped value */
	zassert_ok(rb_pop(&v), "pop needs to return ok");
	zassert_equal(v, 1, "expected hodled value is 1, but got %d", v);

	zassert_ok(rb_pop(&v), "pop needs to return ok");
	zassert_equal(v, 2, "expected hodled value is 2, but got %d", v);

	zassert_ok(rb_pop(&v), "pop needs to return ok");
	zassert_equal(v, 3, "expected hodled value is 3, but got %d", v);

	zexpect_equal(rb_count(), 0, "expected count 0");

}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* Preparation: push 1,2,3,4, then 99, expect -ENOSPC */
	zassert_ok(rb_push(1), "push needs to be successful");
	zassert_ok(rb_push(2), "push needs to be successful");
	zassert_ok(rb_push(3), "push needs to be successful");
	zassert_ok(rb_push(4), "push needs to be successful");

	/* Test: is_full == true, push returns -ENOSPC, count == 4*/
	zexpect_true(rb_is_full(), "expected true");
	zexpect_equal(rb_push(99), -ENOSPC, "Expected -ENOSPC");
	zexpect_equal(rb_count(), 4, "expected ringbuffer with size 4");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int v = 0xff;

	/* preparation: push 7 */
	zassert_ok(rb_push(7), "push needs to be successful");

	/* 1. peek exp ok and v == 7, peek again exp ok, v == 7 */
	zexpect_ok(rb_peek(&v), "Expected ok");
	zexpect_equal(rb_count(), 1, "expected ringbuffer with size 1");
	zexpect_equal(v, 7, "expected v == 7, got %d", v);

	v = 0xff;
	zexpect_ok(rb_peek(&v), "Expected ok");
	zexpect_equal(rb_count(), 1, "expected ringbuffer with size 1");
	zexpect_equal(v, 7, "expected v == 7, got %d", v);
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* pop but use NULL */
	zexpect_equal(rb_pop(NULL), -EINVAL, "expected -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* Preparation: push 1 .. 4 */
	zassert_ok(rb_push(1), "Expected ok");
	zassert_ok(rb_push(2), "Expected ok");
	zassert_ok(rb_push(3), "Expected ok");
	zassert_ok(rb_push(4), "Expected ok");

	/* Test: is_full returns true */
	zexpect_true(rb_is_full(), "expected true");
	zexpect_equal(rb_count(), 4, "expected ringbuffer with size 4");
}
