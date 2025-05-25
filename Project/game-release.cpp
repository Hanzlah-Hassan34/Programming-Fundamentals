//============================================================================
// Name        : Tetris.cpp
// Author      : Hanzlah Hassan
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//============================================================================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

char checkKey = 'R';
int board[35][35];

int sizeofSnake = 2;
int speedofgame = 5;
ofstream history[5];
int menubar;

int noSFoodx[5] = {1, 1, 1, 1, 1};
int noSFoody[5] = {1, 1, 1, 1, 1};
int randomPowerx;
int randomPowery;
int hurdletime = 0;
int hurdlesize = 2;
bool flag = false;
int hurdleShapeX[35];
int hurdleShapeY[35];
int hurdleShapeX2[35];

int hurdleShape = 0;

int stageOfGame = 0;
bool pline = false;
int circleX = 200;
int circleY = 160;

int squareX = circleX;
int squareY = circleY;
int changePosition = 0;
int score = 0;
bool win = false;

int arrx[300] = {squareX - 20, squareX - 40};
int arry[300] = {squareY, squareY, squareY, squareY, squareY};
int sFoodTime[5] = {-1, -1, -1, -1, -1};
int sFoodTimecount = 0;
int Pfood = -1;
int emptyboard = 0;

void SetCanvasSize(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
};

double startx = 320, starty = 400;
void playerhistory()
{
  ifstream playersrecord;
  string record;
  int y = 0;
  playersrecord.open("playerhistory.txt");
  if (!playersrecord)
  {
    cout << "File cannot be opened or does not exist." << endl;
  }
  else
  {
    while (!playersrecord.eof())
    {
      getline(playersrecord, record);

      DrawString(150, 600 - y, record, colors[MISTY_ROSE]);
      y += 100;
    }
  }
  playersrecord.close();
}
void highscoreshow()
{
  ifstream out;
  int Hscore;
  out.open("gamedata.txt");
  if (!out)
  {
    cout << "File is not opened";
  }
  else
  {

    out >> Hscore;
    DrawString(200, 400, "Player Name:  Hanzlah Hassan", colors[MISTY_ROSE]);
    DrawString(200, 300, "and high score is : ", colors[MISTY_ROSE]);
    DrawString(400, 300, to_string(Hscore), colors[5]);
  }
}
void HighScore()
{
  ifstream highscore;
  int h_score;
  highscore.open("gamedata.txt");
  if (!highscore)
  {
    cout << "File cannot be opened or does not exist." << endl;
  }
  else
  {
    cout << "File is opened successfully." << endl;
    highscore >> h_score;

    if (h_score < score)
    {
      win = true;
    }
  }
  highscore.close();
  if (win == true)
  {
    ofstream scoreenter;
    scoreenter.open("gamedata.txt");
    if (!scoreenter)
    {
      cout << "File is not opened";
    }
    else
    {
      scoreenter << score;
    }
  }
}

void snakemove()
{
  for (int i = sizeofSnake - 1; i > 0; i--)
  {
    arrx[i] = arrx[i - 1];
    arry[i] = arry[i - 1];
  }
  arrx[0] = circleX;
  arry[0] = circleY;

  switch (checkKey)
  {
  case 'R':
    circleX += 20;

    if (circleX >= 700)
    {
      circleX = 720 - circleX;
    }
    break;
  case 'L':
    // what to do when left key is pressed...
    circleX -= 20;

    if (circleX <= 0)
    {
      circleX = 720 + circleX;
    }
    break;
  case 'U':
    circleY += 20;

    if (circleY >= 680)
    {
      circleY = 720 - circleY;
    }
    break;
  case 'D':
    circleY -= 20;
    if (circleY <= 0)
    {
      circleY = 720 + circleY;
    }
    break;
  }
}
void endgame()
{
  pline = false;
  HighScore();
  stageOfGame = 2;
}
// check snkae body touch
void bodyTouch()
{
  for (int i = 1; i < sizeofSnake; i++)
  {
    // check snkae body touch
    if (circleX == arrx[i] && circleY == arry[i])

    {
      endgame();
    }
  }
}
// score progress
void scoreProgress()
{
  for (int i = 0; i < 5; i++)
  {
    if (((circleX / 20) == noSFoodx[i]) && ((circleY / 20) == noSFoody[i]))
    {
      score += 5;
      sizeofSnake++;
      sFoodTime[i] = 0;
    }
  }
}

// if to draw the grid
void drawGrid()
{

  int tx = 20;
  int ty = 20;
  for (int i = 0; i < 35; i++)
  {
    for (int j = 0; j < 35; j++)
    {
      tx = 20;
      ty = 20;

      DrawSquare(j * ty, i * tx, 20, colors[MINT_CREAM]);
    }
  }
}
// print snake
void printSnake()
{
  DrawLine(0, 720, 700, 720, 3, colors[YELLOW]);
  for (int i = 0; i < sizeofSnake; i++)
  {
    DrawSquare(arrx[i], arry[i], 20, colors[5]);
    switch (checkKey)
    {
    case 'R':
      DrawTriangle(arrx[i] + 15, arry[i] + 10, arrx[i] + 5, arry[i] + 5, arrx[i] + 5, arry[i] + 15, colors[BLACK]);
      break;
    case 'L':
      DrawTriangle(arrx[i] + 5, arry[i] + 10, arrx[i] + 15, arry[i] + 5, arrx[i] + 15, arry[i] + 15, colors[BLACK]);
      break;
    case 'U':
      DrawTriangle(arrx[i] + 10, arry[i] + 15, arrx[i] + 5, arry[i] + 5, arrx[i] + 15, arry[i] + 5, colors[BLACK]);
      break;
    case 'D':
      DrawTriangle(arrx[i] + 10, arry[i] + 5, arrx[i] + 5, arry[i] + 15, arrx[i] + 15, arry[i] + 15, colors[BLACK]);
      break;
    }
  }
  DrawCircle(circleX + 10, circleY + 10, 10, colors[5]);
}
// to empty board array firstly
void emptyBoard()
{
  for (int i = 0; i < 35; i++)
  {
    for (int j = 0; j < 35; j++)
    {
      board[i][j] = 0;
    }
  }
  emptyboard++;
}
// for small food dosplay
void SmallFood()
{
  for (int i = 0; i < 5; i++)
  {

    if ((sFoodTime[i] % (5 * 40)) == 0)
    {

      sFoodTime[i] = 0;
    }

    if (sFoodTime[i] == 0)
    {
      board[noSFoody[i]][noSFoodx[i]] = 0;

      bool flag;
      do
      {

        flag = true;

        noSFoodx[i] = rand() % 34;
        noSFoody[i] = rand() % 34;

        // condition

        for (int y = 0; y < 35; y++)
        { // as equal also checks itself if there any food before it
          if (board[y][noSFoodx[i]] == 7)
          {
            flag = false;

          } // for row
          if (board[noSFoody[i]][y] == 7)
          {

            flag = false;

          } // main diagonal up
          if (board[noSFoody[i] + y][noSFoodx[i] - y] == 7)
          {

            flag = false;

          } // for main diagonal down
          if (board[noSFoody[i] - y][noSFoodx[i] + y] == 7)
          {

            flag = false;

          } // for counter diagonal up
          if (board[noSFoody[i] + y][noSFoodx[i] + y] == 7)
          {

            flag = false;

          } // for counter diagonal down
          if (board[noSFoody[i] - y][noSFoodx[i] - y] == 7)
          {

            flag = false;
          }
        }
        // for snake mouth

        if (noSFoody[i] * 20 == circleX && noSFoody[i] * 20 == circleY)
        {
          flag = false;
        } // for snake body
        for (int h = 0; h < sizeofSnake; h++)
        {
          if (noSFoodx[i] * 20 == arrx[h] && noSFoody[i] * 20 == arry[h])
          {
            flag = false;
          }
        }
        for (int k = 0; k < hurdlesize; k++)
        {
          if (noSFoodx[i] == hurdleShapeX[k] && noSFoody[i] == hurdleShapeY[k])
          {
            flag = false;
          }
        }

      } while (flag == false);
      board[noSFoody[i]][noSFoodx[i]] = 7;
    }
    sFoodTime[i] += 1;
  }
}
// for display ing foods
void displayFood()
{
  for (int y = 0; y < 34; y++)
  {
    for (int x = 0; x < 34; x++)

    {

      if (board[y][x] == 1)
      {
        // 1 for hurdle
        DrawSquare(x * 20, y * 20, 20, colors[2]);
      }
      else if (board[y][x] == 7)
      {
        DrawCircle(x * 20 + 10, y * 20 + 10, 10, colors[8]);
      }
      else if (board[y][x] == 9)
      {
        DrawCircle(x * 20 + 10, y * 20 + 10, 12, colors[1]);
      }
    }
  }
}
// for PFOOd proess
void timeOfPFood()
{
  flag = false;
  while (flag == false)
  {
    flag = true;
    randomPowerx = rand() % 34;
    randomPowery = rand() % 34;
    for (int i = 0; i < 5; i++)
    { // for small food
      if ((noSFoodx[i] == randomPowerx && noSFoody[i] == randomPowery))
      {
        flag = false;
      }
    } // for snake body
    if ((randomPowerx == (circleX / 20)) && (randomPowery == (circleY / 20)))
    {
      flag = false;
    }
    for (int i = 0; i < sizeofSnake; i++)
    {
      if (randomPowerx == (arrx[i] / 20) && randomPowery == (arry[i] / 20))
      {
        flag = false;
      }
    } // for hurdle check
    for (int i = 0; i < hurdlesize; i++)
    {
      if (randomPowerx == hurdleShapeX[i] && randomPowery == hurdleShapeY[i])
      {
        flag = false;
      }
    }
  }
  board[randomPowery][randomPowerx] = 9; // for power food
  Pfood = 0;
}
// for horizontal hurdle
void hurdle0()
{
  flag = false;
  while (flag == false)
  {
    flag = true; // for horizontal hurdle
    hurdlesize = rand() % 10 + 1;

    hurdleShapeY[0] = rand() % 32 + 1;
    hurdleShapeX[0] = rand() % 32 + 1;

    // for not to go to outside
    while ((hurdleShapeX[0] + hurdlesize) > 34)
    {
      hurdleShapeX[0] = rand() % 32 + 1;
    } // for storing axis
    for (int i = 1; i < hurdlesize; i++)
    {
      hurdleShapeX[i] = hurdleShapeX[i - 1] + 1;
      hurdleShapeY[i] = hurdleShapeY[i - 1];
    }
    for (int i = 0; i < hurdlesize; i++)
    { // for checking power food
      if (randomPowerx == hurdleShapeX[i] && randomPowery == hurdleShapeY[i])
      {
        flag = false;
      } // for small food

      for (int h = 0; h < 5; h++)
      {
        if (noSFoodx[h] == hurdleShapeX[i] && noSFoody[h] == hurdleShapeY[i])
        {
          flag = false;
        }
      }

      // for snake body checking
      for (int k = 0; k < sizeofSnake; k++)
      {
        if (hurdleShapeX[i] == circleX / 20 && hurdleShapeY[i] == circleY / 20)
        {
          flag = false;
        }
        if (hurdleShapeX[i] == arrx[k] / 20 && hurdleShapeY[i] == arry[k] / 20)
        {
          flag = false;
        }
      }
    }
  }
  for (int k = 0; k < hurdlesize; k++)
  {
    board[hurdleShapeY[k]][hurdleShapeX[k]] = 1;
  }
  glutPostRedisplay();
}
// for vertical hurdle
void hurdle1()
{
  flag = false;
  while (flag == false)
  {
    flag = true; // for verticle hurdle
    hurdlesize = rand() % 34;

    hurdleShapeX[0] = rand() % 32 + 1;
    hurdleShapeY[0] = rand() % 32 + 1;

    // for not to go to outside
    while ((hurdleShapeY[0] + hurdlesize) > 34)
    {
      hurdleShapeY[0] = rand() % 32 + 1;
    } // for storing axis
    for (int i = 1; i < hurdlesize; i++)
    {
      hurdleShapeY[i] = hurdleShapeY[i - 1] + 1;
      hurdleShapeX[i] = hurdleShapeX[i - 1];
    }
    for (int i = 0; i < hurdlesize; i++)
    { // for checking power food
      if (randomPowerx == hurdleShapeX[i] && randomPowery == hurdleShapeY[i])
      {
        flag = false;
      } // for small food

      for (int h = 0; h < 5; h++)
      {
        if (noSFoodx[h] == hurdleShapeX[i] && noSFoody[h] == hurdleShapeY[i])
        {
          flag = false;
        }
      }

      // for snake body checking
      for (int k = 0; k < sizeofSnake; k++)
      {
        if (hurdleShapeX[i] == circleX / 20 && hurdleShapeY[i] == circleY / 20)
        {
          flag = false;
        }
        if (hurdleShapeX[i] == arrx[k] / 20 && hurdleShapeY[i] == arry[k] / 20)
        {
          flag = false;
        }
      }
    }
  }
  for (int k = 0; k < hurdlesize; k++)
  {
    board[hurdleShapeY[k]][hurdleShapeX[k]] = 1;
  }
}
// for l shap hurdle
void hurdle2()
{
  flag = false;
  while (flag == false)
  {
    flag = true; // for verticle line of hurdle
    hurdlesize = rand() % 34;

    hurdleShapeX[0] = rand() % 32 + 1;
    hurdleShapeY[0] = rand() % 32 + 1;

    // for not to go to outside
    while (((hurdleShapeX[0] + hurdlesize) > 34))
    {
      hurdlesize = rand() % 34;
      hurdleShapeX[0] = rand() % 32 + 1;
      hurdleShapeY[0] = rand() % 32 + 1;
    } // for storing axis
    for (int i = 1; i < hurdlesize; i++)
    {
      hurdleShapeY[i] = hurdleShapeY[i - 1] + 1;
      hurdleShapeX[i] = hurdleShapeX[i - 1];
    }
    for (int i = 0; i < hurdlesize; i++)
    { // for checking power food
      if (randomPowerx == hurdleShapeX[i] && randomPowery == hurdleShapeY[i])
      {
        flag = false;
      } // for small food

      for (int h = 0; h < 5; h++)
      {
        if (noSFoodx[h] == hurdleShapeX[i] && noSFoody[h] == hurdleShapeY[i])
        {
          flag = false;
        }
      }

      // for snake body checking
      for (int k = 0; k < sizeofSnake; k++)
      {
        if (hurdleShapeX[i] == circleX / 20 && hurdleShapeY[i] == circleY / 20)
        {
          flag = false;
        }
        if (hurdleShapeX[i] == arrx[k] / 20 && hurdleShapeY[i] == arry[k] / 20)
        {
          flag = false;
        }
      }
    }

    // for horizontal hurdle

    hurdleShapeX2[0] = hurdleShapeX[0] + 1;

    // for storing axis
    for (int i = 1; i < hurdlesize; i++)
    {
      hurdleShapeX2[i] = hurdleShapeX2[i - 1] + 1;
    }
    for (int i = 0; i < hurdlesize; i++)
    { // for checking power food
      if (randomPowerx == hurdleShapeX2[i] && randomPowery == hurdleShapeY[0])
      {
        flag = false;
      } // for small food

      for (int h = 0; h < 5; h++)
      {
        if (noSFoodx[h] == hurdleShapeX2[i] && noSFoody[h] == hurdleShapeY[0])
        {
          flag = false;
        }
      }
      // for snake body checking
      for (int k = 0; k < sizeofSnake; k++)
      {
        if (hurdleShapeX2[i] == circleX / 20 && hurdleShapeY[0] == circleY / 20)
        {
          flag = false;
        }
        if (hurdleShapeX2[i] == arrx[k] / 20 && hurdleShapeY[0] == arry[k] / 20)
        {
          flag = false;
        }
      }
    }
  }

  for (int k = 0; k < hurdlesize; k++)
  {
    board[hurdleShapeY[0]][hurdleShapeX2[k]] = 1;
  }

  // for the first and 2nd hurdle
  for (int k = 0; k < hurdlesize; k++)
  {
    board[hurdleShapeY[k]][hurdleShapeX[k]] = 1;
  }

  flag = true;
}
// to display the menu
void displayMenu()
{

  DrawString(300, 600, "Start Game", colors[5]);
  DrawString(300, 500, "Resume Game", colors[5]);
  DrawString(300, 400, "Change level", colors[5]);
  DrawString(300, 300, "High Score", colors[5]);
  DrawString(300, 200, "History", colors[5]);
  DrawString(300, 100, "Exit", colors[5]);
}
// for display the whole outputs
void Display()

{

  usleep(speedofgame * 10000);
  glClearColor(0 /*Red Component*/, 0.0 /*Green Component*/,
               0.0 /*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
  glClear(GL_COLOR_BUFFER_BIT);

  /* int i=0;
   while(i<100000000000)
   {
     i++;
   }*/

  srand(time(NULL));
  // at end game

  if (stageOfGame == 0)
  {
    displayMenu();
  }

  if (stageOfGame == 2 && win == false)
  {

    DrawString(200, 550, " Game is over  and score is :", colors[MISTY_ROSE]);
    DrawString(350, 500, to_string(score), colors[5]);
    DrawString(200, 400, " Do you want to continue ", colors[MISTY_ROSE]);
    DrawString(200, 300, "Yes             No ", colors[MISTY_ROSE]);
  }
  else if (stageOfGame == 2 && win == true)
  {
    DrawString(100, 600, " Congratulations you have broken allthe records", colors[MISTY_ROSE]);

    DrawString(200, 550, " Game is over  and score is :", colors[MISTY_ROSE]);
    DrawString(350, 500, to_string(score), colors[5]);
    DrawString(200, 400, " Do you want to continue ", colors[MISTY_ROSE]);
    DrawString(200, 300, "Yes             No ", colors[MISTY_ROSE]);
  }

  // move snack
  if (stageOfGame == 1 && menubar == 0)
  {
    speedofgame = 10;

    snakemove();

    // check snkae body touch
    bodyTouch();

    // score progress
    scoreProgress();

    DrawString(80, 780, "Player : Hanzlah Hassan", colors[MISTY_ROSE]);

    DrawString(420, 780, " Score : " + to_string(score), colors[5]);

    // board
    // drawGrid();
    // print snake
    printSnake();

    // snake

    if (emptyboard == 0)
    {
      emptyBoard();
    }
    SmallFood();

    // 7==sfood
    // display food

    displayFood();

    // to check
    // check();

    Pfood++;
    if ((pline == true))
    {
      int sizeofPline = Pfood / 20;
      sizeofPline = 10 - sizeofPline;
      for (int i = 0; i < sizeofPline; i++)
      {
        DrawSquare(450 + (i * 20), 730, 20, colors[5]);

        DrawString(400, 730, "Pow.Time : ", colors[GREEN]);
      }
    }

    if (Pfood == (5 * 40))
    {
      pline = false;
      board[randomPowery][randomPowerx] = 0;
    }
    if (Pfood == (5 * 10))
    {
      pline = true;
      timeOfPFood();
    }

    if (circleX == randomPowerx * 20 && circleY == randomPowery * 20)
    {
      score += 20;
      Pfood = 0;
      board[randomPowery][randomPowerx] = 0;
      pline = false;
    }

    // hurdle

    int sizeofhline = hurdletime / 30;
    sizeofhline = 10 - sizeofhline;
    for (int i = 0; i < sizeofhline; i++)
    {
      DrawSquare(150 + (i * 20), 730, 20, colors[2]);

      DrawString(5, 730, "Hur.Time : ", colors[LIGHT_CYAN]);
    }

    if (hurdletime % 300 == 0)
    {
      hurdletime = 0;

      if (hurdleShape == 0)
      {
        // for epmty body
        for (int i = 0; i < hurdlesize; i++)
        {
          board[hurdleShapeY[i]][hurdleShapeX[i]] = 0;
        }
      }

      if (hurdleShape == 1)
      {
        // for epmty body
        for (int i = 0; i < hurdlesize; i++)
        {
          board[hurdleShapeY[i]][hurdleShapeX[i]] = 0;
        }

      } // for random hurdle
      hurdleShape = rand() % 2;

      if (hurdleShape == 0)
      {
        hurdle0();
      }
      if (hurdleShape == 1)
      {

        hurdle1();
      }
    }

    hurdletime++;

    // for the first and 2nd hurdle

    flag = true;

    // for horizontal check
    for (int a = 0; a < hurdlesize; a++)
    {
      if ((hurdleShapeX[a] == (circleX / 20)) && (hurdleShapeY[a] == (circleY / 20)))
      {
        endgame();
      }
    }
  }
  // for verticle check

  
  // for high level
  if (stageOfGame == 1 && menubar == 2)
  {
    speedofgame = 3;
    snakemove();

    // check snkae body touch
    bodyTouch();

    // score progress
    scoreProgress();

    DrawString(80, 780, "Player : Hanzlah Hassan", colors[MISTY_ROSE]);

    DrawString(420, 780, " Score : " + to_string(score), colors[5]);

    // board
    // drawGrid();

    // print snake

    printSnake();

    // snake

    if (emptyboard == 0)
    {
      emptyBoard();
    }
    SmallFood();

    // 7==sfood
    // display food

    displayFood();

    // to check
    // check();

    Pfood++;
    if ((pline == true))
    {
      int sizeofPline = Pfood / 20;
      sizeofPline = 10 - sizeofPline;
      for (int i = 0; i < sizeofPline; i++)
      {
        DrawSquare(450 + (i * 20), 730, 20, colors[5]);

        DrawString(400, 730, "Pow.Time : ", colors[GREEN]);
      }
    }

    if (Pfood == (5 * 40))
    {
      pline = false;
      board[randomPowery][randomPowerx] = 0;    if (Pfood == (5 * 10))
    {
      pline = true;
      timeOfPFood();
    }

    if (circleX == randomPowerx * 20 && circleY == randomPowery * 20)
    {
      score += 20;
      Pfood = 0;
      board[randomPowery][randomPowerx] = 0;
      pline = false;
    }

    // hurdle

    int sizeofhline = hurdletime / 10;
    sizeofhline = 10 - sizeofhline;
    for (int i = 0; i < sizeofhline; i++)
    {
      DrawSquare(150 + (i * 20), 730, 20, colors[2]);

      DrawString(5, 730, "Hur.Time : ", colors[LIGHT_CYAN]);
    }

    if (hurdletime % 100 == 0)
    {
      hurdletime = 0;

      // for epmty body
      for (int i = 0; i < hurdlesize; i++)
      {
        board[hurdleShapeY[i]][hurdleShapeX[i]] = 0;
      }

      if (hurdleShape == 2)
      {
        // for epmty body
        for (int i = 0; i < hurdlesize; i++)
        {
          board[hurdleShapeY[0]][hurdleShapeX2[i]] = 0;
        }

      } // for random hurdle
      hurdleShape = rand() % 3;

      if (hurdleShape == 0)
      {
        hurdle0();
      }
      if (hurdleShape == 1)
      {

        hurdle1();
      }

      if (hurdleShape == 2)
      {
        hurdle2();
      }
    }
    hurdletime++;
    // for horizontal check
    for (int a = 0; a < hurdlesize; a++)
    {
      if ((hurdleShapeX[a] == (circleX / 20)) && (hurdleShapeY[a] == (circleY / 20)))
      {
        endgame();
      }
    }
    // for vertical check
    if (hurdleShape == 2)
    {
      for (int a = 0; a < hurdlesize; a++)
      {
        if ((hurdleShapeX2[a] == (circleX / 20)) && (hurdleShapeY[0] == (circleY / 20)))
        {
          endgame();
        }
      }
      // for verticle check
    }
  }
  if (menubar == 3 && stageOfGame == 1)
  {
    highscoreshow();
  }

  if (menubar == 4 && stageOfGame == 1)
  {
    playerhistory();
  }
  glutPostRedisplay();
  glutSwapBuffers();
}
// do not modify this line..
// mouse click function
void MouseClicked(int button, int state, int x, int y)
{

  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 150 && y < 200)
  {
    menubar = 0;

    stageOfGame++;
  }

  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 250 && y < 300)
  {

    menubar = 0;
    stageOfGame++;
  }

  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 350 && y < 400)
  {
    menubar = 2;

    stageOfGame++;
  }

  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 450 && y < 500)
  {
    menubar = 3;

    stageOfGame++;
  }
  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 530 && y < 600)
  {
    menubar = 4;

    stageOfGame++;
  }
  if (button == GLUT_LEFT_BUTTON && stageOfGame == 0 && x > 280 && x < 450 && y > 630 && y < 700)
  {
    menubar = 4;

    stageOfGame++;
    exit(1);
  }
  if (button == GLUT_LEFT_BUTTON && stageOfGame == 2 && x > 200 && x < 250 && y > 430 && y < 460)
  {
    stageOfGame = 1;
    menubar = 0;
  }

  if (button == GLUT_LEFT_BUTTON && stageOfGame == 2 && x > 330 && x < 360 && y > 430 && y < 460)
  {

    exit(1);
  }
}
// for non printable keys use
void NonPrintableKeys(int key, int x, int y)
{

  if (key == GLUT_KEY_LEFT
      /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
  {
    if (checkKey != 'R')
    {

      checkKey = 'L';
    }
  }
  else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/)
  {
    if (checkKey != 'L')
    {

      checkKey = 'R';
    }
  }
  else if (key == GLUT_KEY_UP) /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/
  {
    if (checkKey != 'D')
    {

      checkKey = 'U';
    }
  }
  else if (key == GLUT_KEY_DOWN)
  { /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/

    if (checkKey != 'U')
    {
      checkKey = 'D';
    }
  }

  /* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
   * this function*/
  glutPostRedisplay();
}
// for printable kays facyors
void PrintableKeys(unsigned char key, int x, int y)
{
  if (key == KEY_ESC /* Escape key ASCII*/)
  {
    exit(1); // exit the program when escape key is pressed.
  }
  if (key == '<' /* Escape key ASCII*/)
  {
    if (checkKey != 'R')
    {

      checkKey = 'L';
    }

    // exit(1); // exit the program when escape key is pressed.
    // aswangle+=90;
  }

  else if (key == '>')
  {
    if (checkKey != 'L')
    {

      checkKey = 'R';
    }
  }
  else if (key == '^')
  {
    if (checkKey != 'D')
    {

      checkKey = 'U';
    }
  }
  else if (key == 'v')
  {
    if (checkKey != 'U')
    {

      checkKey = 'D';
    }
  }

  glutPostRedisplay();
}
// timer function

void Timer(int m)
{

  // implement your functionality here
  glutPostRedisplay();
  // once again we tell the library to call our Timer function after next 1000/FPS
  glutTimerFunc(100000.0 / FPS, Timer, 0);
}

int main(int argc, char *argv[])
{
  int width = 700, height = 800; // i have set my window size to be 800 x 600
  InitRandomizer();              // seed the random number generator...
  glutInit(&argc, argv);         // initialize the graphics library...

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
  glutInitWindowPosition(50, 50);               // set the initial position of our window
  glutInitWindowSize(width, height);            // set the size of our window
  glutCreateWindow("Hanzlah'S Game");           // set the title of our game window
  SetCanvasSize(width, height);                 // set the number of pixels...

  // Register your functions to the library,
  // you are telling the library names of function to call for different tasks.
  // glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
  glutDisplayFunc(Display);          // tell library which function to call for drawing Canvas.
  glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
  glutKeyboardFunc(PrintableKeys);   // tell library which function to call for printable ASCII characters
                                     // This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
  glutTimerFunc(5.0 / FPS, Timer, 0);
  glutMouseFunc(MouseClicked);
  // now handle the control to library and it will call our registered functions when
  // it deems necessary...
  glutMainLoop();

  return 1;

#endif /* Snake Game */
}
