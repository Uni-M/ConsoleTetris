#include "tetris.h"

Figure current;
Figure next;

int main()
{
	srand(time(NULL));
	hidecursor();
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	int score = 0;
	bool in_game = true;
	bool** field = InitField();

	const Figure FIGURES[] = {
		{ initFigure((int*)g0, 2, 4), 2, 4},
		{ initFigure((int*)g1, 2, 4), 2, 4},
		{ initFigure((int*)g2, 2, 4), 2, 4},
		{ initFigure((int*)g3, 2, 4), 2, 4},
		{ initFigure((int*)g4, 2, 4), 2, 4},
		{ initFigure((int*)g5, 2, 4), 2, 4},
		{ initFigure((int*)g6, 2, 4), 2, 4}
	};

	GenerateRandomFigure(FIGURES, &current);
	GenerateRandomFigure(FIGURES, &next);

	clock_t start = clock(), diff;

	while (in_game)
	{
		SetConsoleTextAttribute(consoleHandle, 11);
		printf("=========================================\n");
		printf("                  TETRIS                 \n");
		printf("             SCORE........%d\n", score);
		printf("=========================================\n");

		PrintField(field);

		char c = (_kbhit()) ? _getch() : -1;

		diff = clock() - start;

		if      (c == KEY_MOVE_LEFT)  ShiftLeft(field);
		else if (c == KEY_MOVE_RIGHT) ShiftRight(field);
		else if (c == KEY_MOVE_DOWN)  ShiftDown(field);
		else if (c == KEY_ROTATE)     Rotate(field);
		else if (c == KEY_QUIT)       in_game = false;
		else
		{
			if (diff > 1000) 
			{ 
				ShiftDown(field); 
				start = clock(); 
			}
		} 

		if (IsFixed(field) == 0)
		{
			score += CheckLine(field);
			ShowNextFigure(FIGURES, &current, &next);

			if (GameOver(field) == 1) {
				in_game = false;
				DeleteFigure(current);
				DeleteFigure(next);
				DeleteFigures(FIGURES);
			}
		}

		Sleep(500);
		setcur(0, 0);
	}

	return 0;
}



int** initFigure(int* arr, size_t columns_count, size_t rows_count)
{

	int** a = (int**)malloc(rows_count * sizeof(int*));
	for (int i = 0; i < rows_count; ++i)
	{
		a[i] = (int*)malloc(columns_count * sizeof(int));
		for (int j = 0; j < columns_count; ++j)
		{
			a[i][j] = *(arr + i * columns_count + j);
		}
	}
	return a;

}

bool** InitField()
{
	bool** field = (bool**)calloc(FIELD_ROWS, sizeof(bool*));

	for (int i = 0; i < FIELD_ROWS; ++i)
	{
		field[i] = (bool*)calloc(FIELD_COLUMNS, sizeof(bool));
	}
	return field;
}

void GenerateRandomFigure(Figure FIGURES[], Figure* figure)
{
	Figure random_figure = FIGURES[rand() % FIGURES_COUNT];
	Figure new_figure = random_figure;
	int k = ((FIELD_COLUMNS - 1) / 2);

	new_figure.data = (int**)malloc(new_figure.rows_count * sizeof(int*));
	for (int i = 0; i < new_figure.rows_count; ++i) {

		new_figure.data[i] = (int*)malloc(new_figure.columns_count * sizeof(int));

		new_figure.data[i][0] = random_figure.data[i][0] + k;
		new_figure.data[i][1] = random_figure.data[i][1];

	}

	DeleteFigure(*figure);
	*figure = new_figure;
}

void ShowNextFigure(Figure FIGURES[], Figure* current, Figure* next)
{
	DeleteFigure(*current);

	current->data = (int**)malloc(next->rows_count * sizeof(int*));
	for (int i = 0; i < next->rows_count; ++i) {

		current->data[i] = (int*)malloc(next->columns_count * sizeof(int));

		current->data[i][0] = next->data[i][0];
		current->data[i][1] = next->data[i][1];
	}

	current->columns_count = next->columns_count;
	current->rows_count = next->rows_count;
	GenerateRandomFigure(FIGURES, next);

}

void PrintField(bool** field)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 14);

	int** arr = current.data;
	size_t h = current.rows_count;

	for (int i = 0, n = 0; i < FIELD_ROWS; ++i)
	{
		for (int j = 0; j < FIELD_COLUMNS; ++j)
		{
			int fig = 0;
			for (int k = 0; k < h; k++)
			{
				if (arr[k][1] == i && arr[k][0] == j)
				{
					fig = 1;
				}
			}

			if (fig == 1 || field[i][j] != 0)
			{
				SetConsoleTextAttribute(consoleHandle, 9);
				printf("# ");
				SetConsoleTextAttribute(consoleHandle, 14);
			}
			else
			{
				printf("- ");
			}
		}

		printf("\n");
	}

	AddInfo();

}

void AddInfo()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	int row = 5;
	int column = FIELD_COLUMNS * 2 + 4;
	int frameSize = 4;

	SetConsoleTextAttribute(consoleHandle, 13);
	PrintInfo(column, row, "control keys:");
	SetConsoleTextAttribute(consoleHandle, 7);

	for (row+=2; row <= 11; ++row)
	{
		COORD position = { column, row };
		SetConsoleCursorPosition(consoleHandle, position);
		if (row == 7)         printf("move left.......%c", KEY_MOVE_LEFT);
		else if (row == 8)    printf("move right......%c", KEY_MOVE_RIGHT);
		else if (row == 9)    printf("move down.......%c", KEY_MOVE_DOWN);
		else if (row == 10)   printf("rotate..........%c", KEY_ROTATE);
		else if (row == 11)   printf("quit............%c", KEY_QUIT);	
	}

	row = 13;
	SetConsoleTextAttribute(consoleHandle, 13);
	PrintInfo(column, row++, "next figure:");
	SetConsoleTextAttribute(consoleHandle, 7);


	PrintInfo(column, row++, "+----------+");

	int frameRow;
	for (frameRow = row; frameRow < row + frameSize; ++frameRow)
	{
		PrintInfo(column, frameRow, "|          |");
	}

	PrintInfo(column, frameRow, "+----------+");


	SetConsoleTextAttribute(consoleHandle, 9);

	for (int i = 0; i < next.rows_count; ++i)
	{
		PrintInfo(column + next.data[i][0], row + next.data[i][1], "#");
	}

}

void PrintInfo(int x, int y, char* text)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };
	SetConsoleCursorPosition(consoleHandle, position);
	printf("%s", text);
}


Figure CopyFigure(Figure figure) {

	Figure new_figure = figure;

	new_figure.data = (int**)malloc(new_figure.rows_count * sizeof(int*));
	for (int i = 0; i < new_figure.rows_count; ++i) {
		new_figure.data[i] = (int*)malloc(new_figure.columns_count * sizeof(int));
		for (int j = 0; j < new_figure.columns_count; j++) {
			new_figure.data[i][j] = figure.data[i][j];
		}
	}
	return new_figure;
}

void DeleteFigure(Figure f) {

	for (int i = 0; i < f.rows_count; ++i) {
		free(f.data[i]);
	}
	free(f.data);
}

void Rotate(bool** field)
{
	Figure current_copy = CopyFigure(current);

	double angle = 90;
	const double g2r = M_PI / 180.;
	angle *= g2r;

	int k_x = 0, k_y = 0;
	for (int i = 0; i < current.rows_count; ++i)
	{
		double x_shifted = (double)current_copy.data[i][0] - current_copy.data[1][0];
		double y_shifted = (double)current_copy.data[i][1] - current_copy.data[1][1];

		int x = current_copy.data[1][0] + rint(x_shifted * cos(angle) - y_shifted * sin(angle));
		int y = current_copy.data[1][1] + rint(x_shifted * sin(angle) + y_shifted * cos(angle));

		if (x < 0 && k_x > x)				k_x = -x;
		if (y < 0 && k_y > y)				k_y = -y;
		if (x >= FIELD_COLUMNS && k_x < x)  k_x = x - FIELD_COLUMNS - 1;
		if (y >= FIELD_ROWS && k_y < y)		k_y = x - FIELD_ROWS - 1;

		current.data[i][0] = x;
		current.data[i][1] = y;
	}

	for (int i = 0; i < current.rows_count; i++)
	{
		current.data[i][0] += k_x;
		current.data[i][1] += k_y;
	}

	DeleteFigure(current_copy);
}

void ShiftLeft(bool** field)
{
	for (int i = 0; i < current.rows_count; ++i)
	{
		if (current.data[i][0] == 0 || field[current.data[i][1]][current.data[i][0] - 1] == 1) return;
	}

	for (int i = 0; i < current.rows_count; ++i)
	{
		--current.data[i][0];
	}
}

void ShiftRight(bool** field)
{
	for (int i = 0; i < current.rows_count; ++i)
	{
		if (current.data[i][0] == FIELD_COLUMNS - 1 || field[current.data[i][1]][current.data[i][0] + 1] == 1) return;
	}

	for (int i = 0; i < current.rows_count; ++i)
	{
		++current.data[i][0];
	}
}

void ShiftDown(bool** field)
{
	for (int i = 0; i < current.rows_count; ++i)
	{
		if (current.data[i][1] == FIELD_ROWS - 1 || field[current.data[i][1] + 1][current.data[i][0]] == 1) return;
	}

	for (int i = 0; i < current.rows_count; ++i)
	{
		++current.data[i][1];
	}

}

int IsFixed(bool** field)
{
	for (int i = 0; i < current.rows_count; ++i)
	{
		if (current.data[i][1] == FIELD_ROWS - 1 || field[current.data[i][1] + 1][current.data[i][0]] == 1)
		{
			for (int i = 0; i < current.rows_count; ++i)
			{
				field[current.data[i][1]][current.data[i][0]] = 1;
			}
			return 0;
		}
	}
	return 1;
}

int CheckLine(bool** field)
{
	int count_lines = 0;

	for (int i = 0; i < FIELD_ROWS; ++i)
	{
		int sumCells = 0;

		for (int j = 0; j < FIELD_COLUMNS; ++j)
		{
			if (field[i][j] == 1) ++sumCells;
		}

		if (sumCells == FIELD_COLUMNS)
		{
			ShiftDownLine(i, field);
			++count_lines;
		}

	}
	return CountScore(count_lines);
}

int CountScore(int count_lines)
{
	if (count_lines == 1)  return 10;
	else if (count_lines == 2)  return 25;
	else if (count_lines == 3)  return 100;
	else if (count_lines == 4)  return 250;
	else						return 0;
}

void ShiftDownLine(int line, bool** field)
{
	for (int i = line; i > 0; --i)
	{
		for (int j = 0; j < FIELD_COLUMNS; ++j)
		{
			field[i][j] = field[i - 1][j];
		}
	}

	for (int l = 0; l < FIELD_COLUMNS; ++l)
	{
		field[0][l] = 0;
	}
}

int GameOver(bool** field)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 4);

	for (int i = 0; i < FIELD_COLUMNS; ++i)
	{
		for (int j = 0; j < current.columns_count; ++j)
		{
			if (field[0][i] == 1 && current.data[j][0] == i)
			{
				printf("=========================================\n");
				printf("=============== Game Over ===============\n");
				printf("=========================================\n");

				SetConsoleTextAttribute(consoleHandle, 11);
				return 1;
			}
		}
	}
	SetConsoleTextAttribute(consoleHandle, 11);
	return 0;
}

void DeleteFigures(Figure FIGURES[])
{
	for (int i = 0; i < FIGURES_COUNT; i++)
	{
		DeleteFigure(FIGURES[i]);
	}
}

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}