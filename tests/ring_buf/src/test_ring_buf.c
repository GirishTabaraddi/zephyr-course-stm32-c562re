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

    /* Push a value */
    zassert_equal(rb_push(99), 0, "Push should succeed");
    
    /* Verify it is no longer empty */
    zassert_false(rb_is_empty(), "Buffer should not be empty after push");
    zassert_equal(rb_count(), 1, "Buffer count should be 1");

    /* Re-initialize with capacity 4 */
    rb_init(4);

    /* Verify state is reset */
    zassert_true(rb_is_empty(), "Buffer must be empty after reinit");
    zassert_equal(rb_count(), 0, "Buffer count must be 0 after reinit");
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
	int32_t v;
    
    /* Push 42 */
    zassert_equal(rb_push(42), 0, "Push should succeed");
    
    /* Pop the value */
    zassert_equal(rb_pop(&v), 0, "Pop should succeed");
    zassert_equal(v, 42, "Popped value must be 42");

    /* Verify buffer is empty */
    zassert_true(rb_is_empty(), "Buffer must be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int32_t v;

    /* Push 1, 2, 3 */
    zassert_equal(rb_push(1), 0, "Push 1 should succeed");
    zassert_equal(rb_push(2), 0, "Push 2 should succeed");
    zassert_equal(rb_push(3), 0, "Push 3 should succeed");

    /* Pop and verify they come out in FIFO order (1, 2, 3) */
    zassert_equal(rb_pop(&v), 0, "First pop should succeed");
    zassert_equal(v, 1, "First popped value must be 1");

    zassert_equal(rb_pop(&v), 0, "Second pop should succeed");
    zassert_equal(v, 2, "Second popped value must be 2");

    zassert_equal(rb_pop(&v), 0, "Third pop should succeed");
    zassert_equal(v, 3, "Third popped value must be 3");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	/* Fill the buffer to its initialized capacity of 4 */
    for (int i = 0; i < 4; i++) {
        zassert_equal(rb_push(i), 0, "Push should succeed before full");
    }

    /* Verify it is full */
    zassert_true(rb_is_full(), "Buffer should report full");

    /* Try pushing one more value and expect -ENOSPC */
    zassert_equal(rb_push(99), -ENOSPC, "Push on full buffer must return -ENOSPC");
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
	int32_t v = 0;

    /* Push 7 */
    zassert_equal(rb_push(7), 0, "Push should succeed");

    /* Peek first time */
    zassert_equal(rb_peek(&v), 0, "First peek should succeed");
    zassert_equal(v, 7, "Peeked value must be 7");

    /* Reset local variable just to be sure */
    v = 0;

    /* Peek second time - value should still be there */
    zassert_equal(rb_peek(&v), 0, "Second peek should succeed");
    zassert_equal(v, 7, "Second peeked value must still be 7");

    /* Verify count hasn't changed */
    zassert_equal(rb_count(), 1, "Buffer count must remain 1 after peek");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	/* Attempt to pop into a NULL pointer */
    zassert_equal(rb_pop(NULL), -EINVAL, "Passing NULL to pop must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	/* Push 4 values */
    for (int i = 0; i < 4; i++) {
        zassert_equal(rb_push(i), 0, "Push should succeed");
    }

    /* Verify boundary states */
    zassert_true(rb_is_full(), "Buffer must be true for rb_is_full()");
    zassert_equal(rb_count(), 4, "Buffer count must exactly equal 4");
}
