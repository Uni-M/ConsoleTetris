#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <Windows.h>
#include "keys.h"
#include "figures.h"

#define _USE_MATH_DEFINES
#include <math.h>

const size_t FIELD_ROWS = 20;
const size_t FIELD_COLUMNS = 10;

int** initFigure(int* arr, size_t width, size_t hight);
int GameOver(bool** field);
bool** InitField();
Figure CopyFigure(Figure figure);
void GenerateRandomFigure(Figure FIGURES[]);
void PrintField(bool** field);
void DeleteFigure(Figure f);
void Rotate(bool** field);
void ShiftLeft(bool** field);
void ShiftRight(bool** field);
void ShiftDown(bool** field);
int IsFixed(bool** field);
void ShiftDownLine(int line, bool** field);
int CheckLine(bool** field);
int CountScore(int count_lines);
void AddInfo(int i);
void DeleteFigures(Figure FIGURES[]);

void hidecursor();
void setcur(int x, int y);