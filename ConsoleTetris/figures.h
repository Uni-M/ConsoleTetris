#define FIG_O		{{0, 0}, {1, 0}, {0, 1}, {1, 1}}
#define FIG_L_L		{{0, 0}, {0, 1}, {0, 2}, {1, 2}}
#define FIG_L_R		{{1, 0}, {1, 1}, {2, 1}, {2, 2}}
#define FIG_T		{{0, 0}, {1, 0}, {2, 0}, {1, 1}}
#define FIG_Z_L		{{0, 0}, {0, 1}, {1, 1}, {1, 2}}
#define FIG_Z_R		{{1, 0}, {0, 1}, {1, 1}, {0, 2}}
#define FIG_I		{{0, 0}, {0, 1}, {0, 2}, {0, 3}}

const int FIGURES_COUNT = 7;

typedef struct
{
	int** data;
	size_t columns_count;
	size_t rows_count;
} Figure;

const int g0[4][2] = FIG_O;
const int g1[4][2] = FIG_I;
const int g2[4][2] = FIG_L_L;
const int g3[4][2] = FIG_L_R;
const int g4[4][2] = FIG_T;
const int g5[4][2] = FIG_Z_L;
const int g6[4][2] = FIG_Z_R;