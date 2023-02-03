package dk.itu.parallel.morphology.util.queues;

import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReferenceArray;

/**
 * An array-backed concurrent wait-free implementation of
 * queue. The caller of the constructor needs to specify a
 * desired size. However, the queue will re-size accordingly
 * for performance reasons. The actual size will never be
 * lower than what the caller defined.
 *
 * @author Florian Biermann
 */
public class ConcurrentArrayQueue<E> {

	final AtomicLong head;
	final AtomicLong tail;

	final AtomicReferenceArray<E> array;
	final long mask;

	final int size;

	/**
	 * Instantiate a fixed-size array backed queue.
	 *
	 * @param size
	 * 	The desired size. However, the constructor finds the
	 *  next greatest power of two, if size not already is a power of two.
	 */
	public ConcurrentArrayQueue(final int size) {
		head = new AtomicLong(0);
		tail = new AtomicLong(0);

		// Find the most significant bit set in O(n).
		if (size != 0 && (size & (size - 1)) == 0) {
			this.size = size;
		} else {
			int bits = 0;
			for (int n = size; n > 0; n >>= 1)
				++bits;
			this.size = 1 << bits;
		}

		// Actual array size is now a power of two.
		array = new AtomicReferenceArray<E>(this.size);

		// All bits below the most significant are
		// set to one. Bitwise-and with the mask is
		// the same as modulo but much faster.
		mask = this.size - 1;
	}


	public boolean isEmpty() {
		return head.get() == tail.get();
	}


	public int size() {
		return (int)(tail.get() - head.get());
	}


	public synchronized boolean add(final E arg0) {
		while (true) {
			final long t = tail.get();
			if (size() < array.length()) {
				// If CAS fails, someone added in the meantime.
				if (array.compareAndSet((int)(t & mask), null, arg0)) {
					tail.incrementAndGet();
					return true;
				}
			} else {
				// Queue was already full.
				return false;
			}
		}
	}

	public E poll() {
		while (true) {
			final long h = head.get();
			if (!isEmpty()) {
				// If CAS fails, someone polled in the meantime.
				if (head.compareAndSet(h, h + 1)) {
					final E e = array.get((int)(h & mask));
					array.compareAndSet((int)(h & mask), e, null);
					return e;
				}
			} else {

				// The queue was already empty or
				// someone else took our element.
				return null;
			}
		}
	}

	/**
	 * @return The actual size of the array.
	 */
	public int arraySize() {
		return size;
	}

}
