#include <iostream> // для ввода, вывода и т.д.
#include <time.h> // для работы со временем (используется для генерации рандомного числа)
#include <fstream> // для работы с файлами
#include <vector> // динамический массив
#include <random> // для случайного перемешивания генов в хромосоме

using namespace std;

class Graph {
public:
	int n; // количество вершин
	vector<vector<int>> matrix; // матрица смежности
	Graph(int N, vector<vector<int>> Matrix) { // конструктор класса
		n = N;
		matrix.resize(n, vector<int>(n)); // установление размеров матрицы
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				matrix[i][j] = Matrix[i][j]; // заполнение матрицы
			}
		}
	}
	Graph() {} // конструктор по умолчанию
};

class Input_Data {
private:
	int n; // количество вершин
	vector<vector<int>> matrix; // матрица смежности
public:
	int start_ver; // стартовая вершина
	Graph graph; // экземпляр класса граф
	void get_info(string filename) { // получение информации из файла
		ifstream file;
		file.open(filename); // открытие файла
		if (file.is_open()) { // если удалось открыть файл
			file >> n >> start_ver; // считываем количество вершин и стартовую вершину
			matrix.resize(n, vector<int>(n)); // Устанавливаем размер двумерного вектора
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					file >> matrix[i][j]; // заполняем матрицу из файла
				}
			}
			graph = Graph(n, matrix); // заполняем граф полученными данными
			file.close(); // закрываем файл
		}
	}
	Input_Data(string filename) { // конструктор класса
		get_info(filename);
	}
	Input_Data() {} // конструктор по умолчанию

};

class Menu {
private:
	int mode = 0; // выбранный пункт меню
public:
	string filename; // имя файла
	void Show_Menu() {
		cout << "Добро пожаловать в программу для решения задачи о коммивояжере с помощью генетического алгоритма!" << endl;
		cout << "Пожалуйста, выберите файл с данными о графе, для которого необходимо найти решение:" << endl;
		cout << "1. Файл 1" << endl;
		cout << "2. Файл 2" << endl;
		cout << "3. Файл 3" << endl;
		cout << "4. Файл 4" << endl;
		cout << "5. Файл 5" << endl;
		cout << "Введите число от 1 до 5: ";
		// проверка на корректность ввода
		while (mode != 1 && mode != 2 && mode != 3 && mode != 4 && mode != 5) {
			cin >> mode;
			if (mode != 1 && mode != 2 && mode != 3 && mode != 4 && mode != 5) {
				cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 5: ";
			}
		}
		// определение именин файла по выбранному пункту меню
		switch (mode) {
		case 1: {
			filename = "file1.txt";
			break;
		}
		case 2: {
			filename = "file2.txt";
			break;
		}
		case 3: {
			filename = "file3.txt";
			break;
		}
		case 4: {
			filename = "file4.txt";
			break;
		}
		case 5: {
			filename = "file5.txt";
			break;
		}
		default: {
			filename = "file1.txt";
		}
		}
	}
};

class Algorithm {
private:
	int n;
	int start_ver;
	vector<vector<int>> matrix;
	vector<int> result;
	// метод для выяснения есть ли элемент в списке
	bool In_Array(vector<int> arr, int n, int num) {
		for (int i = 0; i < n; i++) {
			if (arr[i] == num) {
				return true;
			}
		}
		return false;
	}
	// метод для нахождения суммарной длины пути
	int Sum_Way(int start_ver, vector<int> chrom, vector<vector<int>> matrix) {
		int sum_way = matrix[start_ver - 1][chrom[0] - 1];
		for (int i = 0; i < n - 2; i++) {
			sum_way += matrix[chrom[i] - 1][chrom[i + 1] - 1];
		}
		sum_way += matrix[chrom[n - 2] - 1][start_ver - 1];
		return sum_way;
	}
	// метод для сортировки хромосом в порядке возрастания их суммарных длин путей
	void Sort(int n, int start_ver, vector<vector<int>> &population, vector<vector<int>> matrix) {
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (Sum_Way(start_ver, population[j], matrix) > Sum_Way(start_ver, population[j + 1], matrix)) {
					vector<int> temp = population[j];
					population[j] = population[j + 1];
					population[j + 1] = temp;
				}
			}
		}
	}

public:
	Input_Data input_data;
	Graph graph;
	vector<vector<int>> population;
	int num_of_chroms;
	int min_way;
	Algorithm() {} // конструктор по умолчанию
	Algorithm(string filename) { // конструктор класса
		input_data = Input_Data(filename);
		graph = input_data.graph;
		start_ver = input_data.start_ver;
		n = graph.n;
		matrix.resize(n, vector<int>(n));
		matrix = graph.matrix; 
		if (n % 2 == 0) num_of_chroms = n;
		else num_of_chroms = n - 1;
		population.resize(num_of_chroms, vector<int>(n - 1)); // устанавливаем размер популяции
	}
	vector<int> gen_algo(int &min_way) {
		random_device rand_dev; // создание инструмента для генерации рандомных чисел
		mt19937 generator(rand_dev()); // создание генератора рандомных чисел на основе этого инструмента
		// создание начальной популяции
		for (int i = 0; i < num_of_chroms; i++) { // для всех особей
			int gen = 1; // первый ген
			for (int j = 0; j < n - 1; j++) { // для всех n - 1 генов 
				if (gen == start_ver) gen++; // если ген равен начальной вершине, не включаем его в хромосому
				population[i][j] = gen; // заполняем место j-го гена в i-ой хромосоме геном gen 
				gen++; // переходим к следующему гену
			}
			shuffle(population[i].begin(), population[i].end(), generator); // перемешивание генов особи в случайном порядке
		}
		int count_of_iterations = 0;
		while (count_of_iterations < 100) { // условие завершение алгоритма - выполнение 100 итераций
			// скрещивание особей
			// формирование родительских пар (перемешиваем особи в популяции в случайном порядке и распределяем на пары по-порядку)
			shuffle(population.begin(), population.end(), generator); // перемешивание особей в случайном порядке
			// формирование хромосом потомков
			population.resize(2 * num_of_chroms, vector<int>(n - 1)); // увеличиваем размер вектора особей в два раза
			int separation_point; // точка разделения для скрещивания
			srand(time(NULL)); // случайное число не зависит от времени и генерируется случайно при каждом запуске программы
			for (int i = 0; i < num_of_chroms; i += 2) {
				separation_point = rand() % (n - 1); // случайное число от 0 до n - 2
				int j;
				// первая часть родительских хромосом копируется в хромосомы потомков
				for (j = 0; j < separation_point; j++) {
					population[num_of_chroms + i][j] = population[i][j];
					population[num_of_chroms + i + 1][j] = population[i + 1][j];
				}
				// заполнение второй части хромосомы первого потомка
				int index = separation_point;
				for (int k = 0; k < n - 1; k++) {
					if (In_Array(population[num_of_chroms + i], n - 1, population[i + 1][k]) == false) {
						population[num_of_chroms + i][index] = population[i + 1][k];
						index++;
					}
				}
				// заполнение второй части хромосомы второго потомка
				index = separation_point;
				for (int k = 0; k < n - 1; k++) {
					if (In_Array(population[num_of_chroms + i + 1], n - 1, population[i][k]) == false) {
						population[num_of_chroms + i + 1][index] = population[i][k];
						index++;
					}
				}
			}
			// мутация особей
			int start_mutation, end_mutation;
			for (int i = num_of_chroms; i < 2 * num_of_chroms; i++) {
				int rand1 = rand() % (n - 1); // начало и конец мутации - случайные числа
				int rand2 = rand() % (n - 1);
				if (rand1 >= rand2) {
					start_mutation = rand2;
					end_mutation = rand1;
				}
				else {
					start_mutation = rand1;
					end_mutation = rand2;
				}
				// переворот генов от начала до конца мутации в обратном порядке
				for (int j = 0; j < (end_mutation - start_mutation); j++) {
					int temp = population[i][start_mutation + j];
					population[i][start_mutation + j] = population[i][end_mutation - j];
					population[i][end_mutation - j] = temp;
				}

			}
			// селекция особей

			Sort(2 * num_of_chroms, start_ver, population, matrix); // сортируем по возрастанию суммарной длины пути
			/*
			cout << endl;
			for (int i = 0; i < 2 * num_of_chroms; i++) {
				for (int j = 0; j < n - 1; j++) {
					cout << population[i][j] << " ";
				}
				cout << Sum_Way(start_ver, population[i], matrix) << endl;
			}
			cout << count_of_iterations << endl;
			*/
			population.resize(num_of_chroms, vector<int>(n - 1)); // оставляем только лучшую половину
			/*
			for (int i = 0; i < num_of_chroms; i++) {
				for (int j = 0; j < n - 1; j++) {
					cout << population[i][j] << " ";
				}
				cout << Sum_Way(start_ver, population[i], matrix) << endl;
			}
			*/
			count_of_iterations++; // добавляем 1 к количеству итераций
		}
		result.resize(n - 1);
		result = population[0];
		min_way = Sum_Way(start_ver, result, matrix); // определение минимальной длины пути
		return result;
	}
};

class Decision {
public:
	Menu menu;
	string filename;
	Input_Data input_data;
	Algorithm algorithm;
	vector<int> result;
	int min_way;
	Decision() { // конструктор класса
		menu.Show_Menu();
		filename = menu.filename;
		input_data = Input_Data(filename);
		Run_Algorithm();
	}
	void Run_Algorithm() { // запуск генетического алгоритма и получение решения
		algorithm = Algorithm(filename);
		result.resize(input_data.graph.n - 1);
		result = algorithm.gen_algo(min_way);
	}
};

class Output_Data {
private:
	vector<int> result;
	Decision decision;
	string filename;
	string input_filename;
	int min_way;
	int file_number;
	int n;
	int start_ver;
public:
	void Output() { 
		// определяем название файла, в который запишем решение 
		if (filename == "file1.txt") {
			input_filename = "decision1.txt";
			file_number = 1;
		}
		else if (filename == "file2.txt") {
			input_filename = "decision2.txt";
			file_number = 2;
		}
		else if (filename == "file3.txt") {
			input_filename = "decision3.txt";
			file_number = 3;
		}
		else if (filename == "file4.txt") {
			input_filename = "decision4.txt";
			file_number = 4;
		}
		else if (filename == "file5.txt") {
			input_filename = "decision5.txt";
			file_number = 5;
		}
		// выводим решение в консоль
		cout << "Решение примера №" << file_number << ":" << endl;
		cout << "Последовательность городов с минимальной суммарной длиной пути: ";
		cout << start_ver << " - ";
		for (int i = 0; i < n - 1; i++) {
			cout << result[i] << " - ";
		}
		cout << start_ver << endl;
		cout << "Суммарная длина пути: " << min_way << endl;
		// выводим решение в файл
		ofstream file(input_filename, ios_base::out);
		if (file.is_open()) {
			file << "Решение примера №" << file_number << ":" << endl;
			file << "Последовательность городов с минимальной суммарной длиной пути: ";
			file << start_ver << " - ";
			for (int i = 0; i < n - 1; i++) {
				file << result[i] << " - ";
			}
			file << start_ver << endl;
			file << "Суммарная длина пути: " << min_way << endl;
			file.close();
		}
	}
	Output_Data() { // конструктор класса
		n = decision.input_data.graph.n;
		result.resize(n - 1);
		result = decision.result;
		filename = decision.filename;
		min_way = decision.min_way;
		start_ver = decision.input_data.start_ver;
		Output();
	}
};

int main() { // главная функция
	setlocale(LC_ALL, "RU"); // устанавливаем русский язык
	Output_Data output_data; // создаем экземпляр решения
	system("pause");
	return 0;
}


/* // генерация матрицы смежности графа и начальной вершины
srand(time(NULL));
ofstream file("file5.txt", ios_base::out);
vector<vector<int>> a;
a.resize(100, vector<int>(100));
file << 100 << " " << 1 + rand() % 100 << endl;
if (file.is_open()) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (i == j) a[i][j] = 0;
			else if (i < j) a[i][j] = 1 + rand() % 99;
			else if (i > j) a[i][j] = a[j][i];
		}
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			file << a[i][j] << " ";
		}
		file << endl;
	}
	file.close();
}
*/