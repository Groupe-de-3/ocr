
#define clamp(X, MIN, MAX) ({   \
    const typeof(X) _h_x = (X); \
    const typeof(MIN) _h_min = (MIN); \
    const typeof(MAX) _h_max = (MAX); \
    _h_x > _h_max ? _h_max : _h_x < _h_min ? _h_min : _h_x; \
})

#define min(X, Y) ({   \
    const typeof(X) _h_x = (X); \
    const typeof(Y) _h_y = (Y); \
    _h_y > _h_x ? _h_x : _h_y; \
})

#define max(X, Y) ({   \
    const typeof(X) _h_x = (X); \
    const typeof(Y) _h_y = (Y); \
    _h_y < _h_x ? _h_x : _h_y; \
})
