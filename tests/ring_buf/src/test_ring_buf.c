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
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	zassert_ok(rb_push(10), "Added a value into of the buffer");
	zassert_equal(rb_count(), 1, "Fresh buffer count must be 0");
	zassert_ok(rb_init(4), "Buffer reinitialized");

	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");

	//ztest_test_skip();
}

ZTEST(ring_buf_init, test_return_EINVAL_CAPACITY_ZERO)
{
	zassert_equal(rb_init(0), -EINVAL,  "The capacity of buffer is zero");
}

ZTEST(ring_buf_init, test_return_EINVAL_MAX_CAPACITY)
{
	zassert_equal(rb_init(17), -EINVAL,  "The capacity of buffer is only for 16 elements");
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
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int v;

	zassert_ok(rb_push(42), "Added a new value in the buffer");
	zassert_ok(rb_pop(&v), "Pop the last value in front of the buffer");

	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	//ztest_test_skip();
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int v;

	zassert_ok(rb_push(1), "Added first value");
	zassert_ok(rb_push(2), "Added second value");
	zassert_ok(rb_push(3), "Added third value");

	zassert_ok(rb_pop(&v), "Pop failed");
	zassert_equal(v, 1, "Expected first value to be 1");
	zassert_ok(rb_pop(&v), "Pop failed");
	zassert_equal(v, 2, "Expected second value to be 2");
	zassert_ok(rb_pop(&v), "Pop failed");
	zassert_equal(v, 3, "Expected third value to be 3");
	//ztest_test_skip();
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	zassert_ok(rb_push(1), "Added first value");
	zassert_ok(rb_push(2), "Added second value");
	zassert_ok(rb_push(3), "Added third value");
	zassert_ok(rb_push(4), "Added fourth value");

	zassert_true(rb_is_full(), "Verify if the buffer is full");

	zassert_equal(rb_push(4), -ENOSPC,  "The buffer is only for 4 elements, Buffer is full");

	//ztest_test_skip();
}

ZTEST(ring_buf_push_pop, test_pop_return_enodata)
{
    int value = 0;

    zassert_true(rb_is_empty(), "Fresh buffer must be empty");
    zassert_equal(rb_count(), 0U, "Fresh buffer count must be 0");

    zassert_equal(rb_pop(&value), -ENODATA,
                  "Pop from an empty buffer must return -ENODATA");
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
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int v;

	zassert_ok(rb_push(7), "Added a value into the buffer");
	zassert_ok(rb_peek(&v), "Peek failed");
	zassert_ok(rb_peek(&v), "Peek failed");

	zassert_equal(rb_count(), 1, "Fresh buffer count must be 1");

	//ztest_test_skip();
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */

	zassert_equal(rb_pop(NULL), -EINVAL, "Pointer NULL"); 
	//ztest_test_skip();
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	zassert_ok(rb_push(1), "Added first value");
	zassert_ok(rb_push(2), "Added second value");
	zassert_ok(rb_push(3), "Added third value");
	zassert_ok(rb_push(4), "Added fourth value");

	zassert_true(rb_is_full(), "Verify if the buffer is full");
	zassert_equal(rb_count(), 4, "Fresh buffer count must be 4");
	//ztest_test_skip();
}

ZTEST(ring_buf_boundaries, test_peek_return_einval)
{
	zassert_equal(rb_peek(NULL), -EINVAL, "The value is NULL");
}

ZTEST(ring_buf_boundaries, test_peek_return_enodata)
{
    int value = 0;

    zassert_true(rb_is_empty(), "Fresh buffer must be empty");
    zassert_equal(rb_count(), 0U, "Fresh buffer count must be 0");

    zassert_equal(rb_peek(&value), -ENODATA,
                  "Pop from an empty buffer must return -ENODATA");
}