// SaperGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

using namespace std;

// генерация игрового поля
// создание числовой матрицы
int field[100][100];
// -1 мина
// 0-8 клетка

// создание поля игрока
int field_p[100][100];
// -2 если клетка не открыта
// -1 если стоит флажок
// -3 мина *
// 0-8 если клетка открыта и в ней стоит цифра

// задаем кол-во строк и столбцов/ K - кол-во мин
const int N = 5, M = 5, K = 5;

// очистка поля и вывод на экран
void print_field() {
	system("cls");
	// вывод координат на игровом поле
	cout << "  ";
	for (int i = 0; i < M; ++i) {
		cout << " " << i + 1; // вывод координат столбцов
		if (i + 1 < 10) cout << ' '; // проверка условия что бы не сьезжали координаты
	}
	cout << endl;
	// вывод игрового поля
	for (int i = 0; i < N; i++) {		
		cout << i + 1 << " ";  // вывод координат строк
		if (i + 1 < 10) cout << ' '; // проверка условия что бы не сьезжали координаты
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] == -2) cout << "_  ";
			else if (field_p[i][j] == -1) cout << "f  ";
			else if (field_p[i][j] == -3) cout << "*  ";
			else cout << field_p[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

// рекурсивная функция открытия пустых клеток
void dfs(int x, int y) {
	if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] > -2) return;
	field_p[x][y] = field[x][y];
	if (field[x][y] > 0) return;
	for (int i2 = x - 1; i2 <= x + 1; ++i2) {
		for (int j2 = y - 1; j2 <= y + 1; ++j2) {
			if (i2 != x || j2 != y) dfs(i2, j2);
		}
	}
}

// true - не взорвались
// false - взорвались
bool open_cell(int x, int y) {
	if (field[x][y] == -1) return false;
	if (field[x][y] > 0) {
		field_p[x][y] = field[x][y];
		return true;
	}
	// открытие области пустых клеток рекурсией
	dfs(x, y);
}

bool is_win() {
	int opened = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] >= 0) opened++;
		}
	}
	return (N * M - K == opened);
}

void end_game(bool is_win = false) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] == -1) field_p[i][j] = -2;
			if (field[i][j] == -1) field_p[i][j] = -3;
		}
	}
	print_field();
	cout << "You " << (is_win ? "winner" : "lose") << ".\n Enter any string a new game: \n";
	string s;
	cin >> s;
}

int main()
{
	while (true) {
		srand(time(0)); // зависимость рандома от текущего времени
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				field[i][j] = 0;
				field_p[i][j] = -2;
			}
		}
		// на поле мины это -1. остальные клетки от 0 до 8 по кол-ву окружающий мин
		for (int i = 0; i < K; ++i) {
			while (true) {
				int x = rand() % N;
				int y = rand() % M;
				// rand() выдает случайное 2байтное число
				// если в данной координате не стоит мина, то устанавливаем ее и выходим из цикла
				if (field[x][y] != -1) {
					field[x][y] = -1;
					break;
				}
			}
		}
		// присвоим остальным клеткам значения зависящие от кол-ва мин вокруг
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (field[i][j] != -1) {
					field[i][j] = 0;
					for (int i2 = i - 1; i2 <= i + 1; ++i2) {
						for (int j2 = j - 1; j2 <= j + 1; ++j2) {
							if (i2 >= 0 && i2 < N && j2 >= 0 && j2 < M &&
								field[i2][j2] == -1) ++field[i][j];
						}
					}
				}
			}
		}
		while (true) {
			print_field();
			cout << "Enter comand:";
			/*
			\o x y открыть клетку
			\f x y  поставить/убрать флаг
			\n новая игра
			\q  завершение программы
			*/
			string comand;
			cin >> comand;
			if (comand == "\\o") {
				// ввод координат
				int x, y;
				cin >> x >> y;
				--x; --y;
				// проверка
				if (x < 0 || x >=N || y < 0 || y >=M || field_p[x][y] >= 0) continue;
				// откроем клетки
				if (!open_cell(x, y)) {
					end_game();
					break;
				}
				if (is_win()) {
					end_game(true);
					break;
				}
			}
			else if (comand == "\\f") {
				// ввод координат
				int x, y;
				cin >> x >> y;
				--x; --y;
				// проверка
				if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] >= 0) continue;
				// поставить флаг
				if (field_p[x][y] == -1) field_p[x][y] = -2;
				else field_p[x][y] = -1;
			}
			else if (comand == "\\n") {
				break;
			}
			else if (comand == "\\q") {
				return 0;
			}
		}
	}
}











// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
