/**
 * Iterator tools.
 *
 * @file
 */
#ifndef PYPP_GENERATOR_HPP
#define PYPP_GENERATOR_HPP

namespace pypp { namespace generator {

/**
 * Interface for a Python-like generator that works via C++ iterators.
 *
 * A generator produces a sequence of values on-demand, as opposed to a fixed
 * in-memory container. Generator iterators are forward iterators that always
 * point to a current generator value or a special end value. If multiple
 * iterators point to the same same generator they will always point to the
 * same value, i.e. advancing one iterator advances all sibling iterators.
 *
 * @tparam T: generator return typ
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
            if (not target) {
                throw std::out_of_range("cannot advance end iterator");
            }
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
     * A subsequent call to next() will succeed for an active generator.
     *
     * @return: true if the generator is still active
     */
    virtual bool active() const = 0;

    /**
     * Get the current value of the generator.
     *
     * @return: current generator value
     */
    virtual T value() const = 0;

    /**
     * Generate the next value.
     *
     * A std::out_of_range exception should be thrown if end() is true.
     */
    virtual void next() = 0;

};


/**
 * Generate a sequential range of values.
 *
 * @tparam T1: range type that supports addition and comparison operators
 * @tparam Step: type that supports T1 + T2
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
        if (not active()) {
            throw std::out_of_range("generator is exhausted");
        }
        return current;
    }

    /**
     * Generate the next value.
     *
     * A std::out_of_range exception is thrown if end() is true.
     */
    void next() override {
        if (not active()) {
            throw std::out_of_range("generator is exhausted");
        }
        current += step;
    }

private:
    T current;
    T stop;
    Step step;
};


}}  // pypp::generator


#endif  // PYPP_GENERATOR_HPP
