#include <iostream>
#include <fstream> // Для работы с файлами
#include <thread>  // Для std::this_thread::sleep_for
#include <chrono>  // Для std::chrono::milliseconds
#include <cstdlib> // Для std::system("cls");
using namespace std;

int main()
{
  ifstream fileFirstGeneration("in.txt"); //-- Читаем файл поколения;

  if (!fileFirstGeneration.is_open()) //-- Если не открылся;
  {
    fileFirstGeneration.close(); //-- Закрываем;
    cout << "ERROR! File \"in.txt\" is not definded!" << endl;
    return 0; //-- Бросаем;
  }

  std::system("cls"); //- Очищаем консоль

  int rows = 0;
  int columns = 0;
  fileFirstGeneration >> rows;    //-- Ряды;
  fileFirstGeneration >> columns; //-- Колонки;

  char **generation = new char *[rows]; //-- Делаем пустой массив;
  for (int i = 0; i < rows; i++)
  {
    generation[i] = new char[columns];
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      generation[i][j] = '-'; //-- Заполняем его минусами;
    }
  }

  int i = 0, j = 0;
  while (fileFirstGeneration >> i >> j) //- Пока файл не кончится,
  {
    if (i >= 0 && i < rows && j >= 0 && j < columns) //- Проверка переполнения
    {
      generation[i][j] = '*'; //-- Добавляем звёздочки из файла;
    }
  }

  /*-- Делаем второй массив, будем его заполнять по правилам --*/

  char **secondGeneration = new char *[rows];
  for (int i = 0; i < rows; i++)
  {
    secondGeneration[i] = new char[columns];
  }

  const int numberOfDirections = 8; //- 8 направлений вокруг клетки;
  int cellsAround = 0;              //- Клеток рядом изначально нет;
  int generationNumber = 0;         //- Номер поколения;
  int aliveCells = 0;               //- Живых клеток изначально нет;
  bool difference = false;          //- Отличий изначально нет;

  while (true)
  {
    aliveCells = 0;     //- Сбрасываем живые клетки преред каждым пробегом по массиву
    difference = false; //- Отличия тоже сбрасываем

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        char directions[numberOfDirections] = {}; //- Массив напрвалений
        cellsAround = 0;

        int stepTop = i - 1;
        int stepLeft = j - 1;
        int stepBottom = i + 1;
        int stepRight = j + 1;

        bool top = stepTop > -1;
        bool left = stepLeft > -1;
        bool bottom = stepBottom < rows;
        bool right = stepRight < columns;

        directions[0] = top ? generation[stepTop][j] : ' ';
        directions[1] = (top && right) ? generation[stepTop][stepRight] : ' ';
        directions[2] = right ? generation[i][stepRight] : ' ';
        directions[3] = (bottom && right) ? generation[stepBottom][stepRight] : ' ';
        directions[4] = bottom ? generation[stepBottom][j] : ' ';
        directions[5] = (bottom && left) ? generation[stepBottom][stepLeft] : ' ';
        directions[6] = left ? generation[i][stepLeft] : ' ';
        directions[7] = (top && left) ? generation[stepTop][stepLeft] : ' ';

        for (int i = 0; i < numberOfDirections; i++) //- Бежим по массиву напрвлений
        {
          cellsAround = (directions[i] == '*') ? cellsAround + 1 : cellsAround;
        }

        if (generation[i][j] == '*')
        {
          aliveCells++;

          if ((cellsAround == 0) || (cellsAround == 1))
          {
            secondGeneration[i][j] = '-';
            difference = true;
          }
          else if ((cellsAround == 2) || (cellsAround == 3))
          {
            secondGeneration[i][j] = '*';
          }
          else
          {
            secondGeneration[i][j] = '-';
            difference = true;
          }
        }
        else
        {
          if (cellsAround == 3)
          {
            secondGeneration[i][j] = '*';
            difference = true;
          }
          else
          {
            secondGeneration[i][j] = '-';
          }
        }
      }
    }

    generationNumber++;

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        cout << generation[i][j] << ' '; //- Отрисовываем первый массив
      }
      cout << endl;
    }
    cout << "Generation: " << generationNumber << ". Alive cells: " << aliveCells << endl;

    if (aliveCells == 0) //- Если живых клеток нет
    {
      cout << "All cells are dead. Game over." << endl;
      break; //- Прерываем цикл
    }

    if (!difference) //- Если отличий нет
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //- Задерживаем цикл
      std::system("cls");

      for (int i = 0; i < rows; i++)
      {
        for (int j = 0; j < columns; j++)
        {
          cout << secondGeneration[i][j] << ' '; //- Рисуем второй массив
        }
        cout << endl;
      }

      cout << "Generation: " << generationNumber + 1 << ". Alive cells: " << aliveCells << endl; //- Добавляем 1
      cout << "The world has stagnated. Game over." << endl;

      break; //- Прерываем цикл
    }

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        generation[i][j] = secondGeneration[i][j]; //- Копируем второй массив в первый,
      } //- с которым вновь начнём цикл
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //- Задерживаем цикл
    std::system("cls");
  }

  fileFirstGeneration.close(); //- Закрываем файл

  for (int i = 0; i < rows; i++)
  {
    delete[] generation[i];       //- Удаляем колонки первого массива
    delete[] secondGeneration[i]; //- Удаляем колонки второго массива
  }
  delete[] generation;       //- Удаляем первый массив
  delete[] secondGeneration; //- Удаляем второй массив

  return 0;
}