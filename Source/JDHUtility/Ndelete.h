#define NDELETE(_v) { if ((_v) != NULL) { delete (_v); (_v) = NULL; } }
#define NDELETE_ARRAY(_v) { if ((_v) != NULL) { delete [] (_v); (_v) = NULL; } }
