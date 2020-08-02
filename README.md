# StringSlice

String slice for use on embedded systems (noexcept).

This defines the `StringSlice` (slices) class that is a non-owning view of a character array. The slices' lifetime are the same as the underlying buffers. This implements functions like `find`, `strip`, and `substr` that act on slices as well as comparison operators.

All methods are noexcept.
