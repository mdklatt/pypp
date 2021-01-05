/**
 * Iterator tools.
 *
 * @file
 */
#ifndef PYPP_GENERATOR_HPP
#define PYPP_GENERATOR_HPP

#include <iterator>
#include <tuple>
#include <utility>


namespace pypp { namespace generator {

/**
 * Interface for generators that work via C++ iterators.
 *
 * A generator produces a sequence of values on-demand, as opposed to a fixed
 * in-memory container. Generator iterators are forward iterators that always
 * point to a current generator value or a special end value. If multiple
 * iterators point to the same same generator they will always point to the
 * same value, i.e. advancing one iterator advances all sibling iterators.
 *
 * @tparam T: generator return type
 */
template <typename T>
class Generator {
public:
    class iterator {
    public:
        // Iterator traits.
        using value_type = T;
        using reference = T;
        using iterator_category=std::input_iterator_tag;
        using pointer = void;  // no indirection operator
        using difference_type = void;  // no difference operator

        /**
         * Return value for operator++(int).
         *
         * This is a holder for the current iterator value that will no longer
         * be available once the iterator is incremented.
         */
        class postinc_value {
        public:
            /**
             * Value constructor.
             *
             * @param value: current iterator value
             */
            explicit postinc_value(const T& value): value(value) {}

            /**
             * Dereference operator.
             *
             * @return: saved iterator value
             */
            T operator*() const {
                return value;
            }

        private:
            T value;
        };

        /**
         * Constructor.
         *
         * @param generator to iterate over
         */
        explicit iterator(Generator *const generator=nullptr):
            target(generator and generator->active() ? generator : nullptr) {}

        /**
         * Pre-increment operator.
         *
         * @return incremented iterator
         */
        iterator& operator++() {
            advance();
            return *this;
        }

        /**
         * Post-increment operator.
         *
         * @return iterator pointing to the value prior to incrementing
         */
        postinc_value operator++(int) {
            postinc_value value(**this);
            advance();
            return value;
        }

        /**
         * Dereference operator.
         *
         * @return: current value of the underlying generator
         */
        T operator*() const {
            if (not target) {
                throw std::out_of_range("cannot dereference end iterator");
            }
            return target->value();
        }

        /**
         * Equality operator.
         *
         * @param lhs: left operand
         * @param rhs: right operand
         * @return true if iterators are equivalent
         */
        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.target == rhs.target;
        }

        /**
         * Inequality operator.
         *
         * @param lhs: left operand
         * @param rhs: right operand
         * @return true if iterators are not equivalent
         */
        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            // Guaranteed to be the inverse of operator==().
            return not (lhs == rhs);
        }

    protected:
        /**
         * Advance the iterator to the next value.
         *
         * This throws std::out_of_range if the iterator is already at the end
         * of the sequence.
         */
        void advance() {
            assert(target);
            target->next();
            if (not target->active()) {
                target = nullptr;
            }
        }

    private:
        Generator* target;
    };

    /**
     * Return an iterator to the current position of the iterator sequence.
     *
     * @return: iterator
     */
    iterator begin() {
        return iterator(this);
    }

    /**
     * Return an iterator representing an inactive generator.
     *
     * @return: iterator
     */
    iterator end() {
        return iterator();
    }

protected:
    /**
     * Default constructor.
     *
     * Once constructed, value() should return a valid value or end() should
     * be true.
     */
    Generator() = default;

    /**
     * Test if the generator is active.
     *
     * @return: true if the generator is still active
     */
    virtual bool active() const = 0;

    /**
     * Get the current value of the generator.
     *
     * The result is only valid if the generator is active.
     *
     * @return: current generator value
     */
    virtual T value() const = 0;

    /**
     * Generate the next value.
     */
    virtual void next() = 0;

};


/**
 * Generate a sequential range of values.
 *
 * @tparam T: range type that supports addition and comparison operators
 * @tparam Step: type that supports T + Step
 */
template <typename T, typename Step=T>
class Range: public Generator<T> {
public:
    /**
     * Constructor.
     *
     * @param start: range start
     * @param stop: range stop (exclusive)
     * @param step: range increment
     */
    Range(T start, T stop, Step step):
        current(start), stop(stop), step(step) {
        if (step == 0) {
            throw std::invalid_argument("step must be nonzero");
        }
    }

    /**
     * Test if the generator is active.
     *
     * A subsequent call to next() will succeed for an active generator.
     *
     * @return: true if the generator is still active
     */
    bool active() const override {
        return step > 0 ?  current < stop : current > stop;
    }

    /**
     * Get the current value of the generator.
     *
     * @return: current generator value
     */
    T value() const override {
        return current;
    }

    /**
     * Generate the next value.
     *
     * A std::out_of_range exception is thrown if active() is false.
     */
    void next() override {
        current += step;
    }

private:
    T current;
    T stop;
    Step step;
};


/**
 * Enumerate a sequence of items.
 *
 * @tparam IT: forward iterator type
 */
template <typename IT>
class Enumerator: public Generator<std::pair<ssize_t, typename IT::value_type>> {
public:
    /**
     * Constructor.
     *
     * @param first: first position
     * @param last: last position (exclusive)
     * @param start: starting count
     */
    Enumerator(IT first, IT last, ssize_t start=0):
        pos(first), last(last), count(start) {}

    /**
     * Test if the generator is active.
     *
     * A subsequent call to next() will succeed for an active generator.
     *
     * @return: true if the generator is still active
     */
    bool active() const override {
        return pos != last;
    }

    /**
     * Get the current value of the generator.
     *
     * @return: current generator value
     */
    std::pair<ssize_t, typename IT::value_type> value() const override {
        assert(active());
        return std::make_pair(count, *pos);
    }

    /**
     * Generate the next value.
     */
    void next() override {
        std::advance(pos, 1);
        ++count;
    }

private:
    ssize_t count;
    IT pos;
    IT last;
};


/**
 * Elementwise combination of sequences.
 *
 * @tparam IT1: first forward iterator type
 * @tparam IT2: second forward iterator type
 */
template <typename IT1, typename IT2>
class Zipper: public Generator<std::tuple<typename IT1::value_type, typename IT2::value_type>> {
public:
    /**
     * Constructor.
     *
     * @param first1: first position of first sequence
     * @param last1: last position of first sequence (exclusive)
     * @param first2: first position of second sequence
     * @param last2: last position of second sequence (exclusive)
     */
    Zipper(IT1 first1, IT1 last1, IT2 first2, IT2 last2):
        pos1(first1), last1(last1), pos2(first2), last2(last2) {}

    /**
     * Test if the generator is active.
     *
     * @return: true if the generator is still active
     */
    bool active() const override {
        return pos1 != last1 and pos2 != last2;
    }

    /**
     * Get the current value of the generator.
     *
     * @return: current generator value
     */
    std::tuple<typename IT1::value_type, typename IT2::value_type> value() const override {
        assert(active());
        return std::make_tuple(*pos1, *pos2);
    }

    /**
     * Generate the next value.
     */
    void next() override {
        std::advance(pos1, 1);
        std::advance(pos2, 1);
    }

private:
    IT1 pos1;
    IT1 last1;
    IT2 pos2;
    IT2 last2;
};


}}  // pypp::generator


#endif  // PYPP_GENERATOR_HPP
