#ifndef TNT_FOREVER_DOWNCAST_H
#define TNT_FOREVER_DOWNCAST_H

/*
 * Generates functions to safely downcast a pointer Bar* to FBar*
 * FOREVER_DOWNCAST() should be included in the header file
 * declaring FBar, e.g.:
 *
 * #include <Bar.h>
 *
 * class FBar : public Bar {
 * };
 *
 * FOREVER_DOWNCAST(Bar)
 *
 */

#define FOREVER_DOWNCAST(CLASS)                                    \
    inline F##CLASS& downcast(CLASS& that) noexcept {               \
        return static_cast<F##CLASS &>(that);                       \
    }                                                               \
    inline const F##CLASS& downcast(const CLASS& that) noexcept {   \
        return static_cast<const F##CLASS &>(that);                 \
    }                                                               \
    inline F##CLASS* downcast(CLASS* that) noexcept {               \
        return static_cast<F##CLASS *>(that);                       \
    }                                                               \
    inline F##CLASS const* downcast(CLASS const* that) noexcept {   \
        return static_cast<F##CLASS const *>(that);                 \
    }

#endif // TNT_FOREVER_DOWNCAST_H
