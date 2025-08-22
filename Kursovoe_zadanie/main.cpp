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
    cout << "ERROR! File in.txt is not definded!" << endl;
    return 0; //-- Бросаем;
  }

  std::system("cls"); //- Очищаем консоль

  int rows = 0;
  int columns = 0;
  fileFirstGeneration >> rows;    //-- Ряды;
  fileFirstGeneration >> columns; //-- Колонки;

  string **generation = new string *[rows]; //-- Делаем пустой массив;
  for (int i = 0; i < rows; i++)
  {
    generation[i] = new string[columns];
  }

  if (fileFirstGeneration.eof()) //-- Если файл кончился,
  {
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        generation[i][j] = "-"; //-- То заполняем массив минусами;
      }
    }
  }
  else //-- Если файл не кончился, то:
  {
    int verticalPosition = 0;
    int horizontalPosition = 0;
    fileFirstGeneration >> verticalPosition;
    fileFirstGeneration >> horizontalPosition; //-- Координаты первой *

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        if ((i == verticalPosition) && (j == horizontalPosition)) //-- Если равно,
        {
          generation[i][j] = "*";         //-- Ставим *
          if (!fileFirstGeneration.eof()) //-- Если файл не кончился,
          {
            fileFirstGeneration >> verticalPosition; //-- то обновляем позиции;
            fileFirstGeneration >> horizontalPosition;
          }
        }
        else
        {
          generation[i][j] = "-"; //-Если координаты элемента не совпадают с позицией *
        }
      }
    }
  }

  /*-- Делаем пустой динамический массив, будем его заполнять по правилам --*/

  string **secondGeneration = new string *[rows];
  for (int i = 0; i < rows; i++)
  {
    secondGeneration[i] = new string[columns];
  }

  /*-- Объявляем переменные.
  В них заносим знаки, которые вокруг клетки:

  h a b
  g x c
  f e d

  x - это клетка.

  В цикле будем только менять их значения,
  а определим - здесь. --*/

  string a, b, c, d, e, f, g, h = "";
  int cellsAround = 0;
  int generationNumber = 0;
  int aliveCells = 0;
  bool difference = false;

  while (true)
  {
    aliveCells = 0;
    difference = false;

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        a, b, c, d, e, f, g, h = "";
        cellsAround = 0;

        int stepTop = i - 1;
        int stepLeft = j - 1;
        int stepBottom = i + 1;
        int stepRight = j + 1;

        bool top = stepTop > -1;
        bool left = stepLeft > -1;
        bool bottom = stepBottom < rows;
        bool right = stepRight < columns;

        a = top ? generation[stepTop][j] : "";
        b = (top && right) ? generation[stepTop][stepRight] : "";
        c = right ? generation[i][stepRight] : "";
        d = (bottom && right) ? generation[stepBottom][stepRight] : "";
        e = bottom ? generation[stepBottom][j] : "";
        f = (bottom && left) ? generation[stepBottom][stepLeft] : "";
        g = left ? generation[i][stepLeft] : "";
        h = (top && left) ? generation[stepTop][stepLeft] : "";

        cellsAround = (a == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (b == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (c == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (d == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (e == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (f == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (g == "*") ? cellsAround + 1 : cellsAround;
        cellsAround = (h == "*") ? cellsAround + 1 : cellsAround;

        if (generation[i][j] == "*")
        {
          aliveCells++;

          if ((cellsAround == 0) || (cellsAround == 1))
          {
            secondGeneration[i][j] = "-";
            difference = true;
          }
          else if ((cellsAround == 2) || (cellsAround == 3))
          {
            secondGeneration[i][j] = "*";
          }
          else
          {
            secondGeneration[i][j] = "-";
            difference = true;
          }
        }
        else
        {
          if (cellsAround == 3)
          {
            secondGeneration[i][j] = "*";
            difference = true;
          }
          else
          {
            secondGeneration[i][j] = "-";
          }
        }
      }
    }

    generationNumber++;

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < columns; j++)
      {
        cout << generation[i][j] << " "; //- Отрисовываем первый массив
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
          cout << secondGeneration[i][j] << " "; //- Рисуем второй массив
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