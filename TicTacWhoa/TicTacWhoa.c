// Ashray Nair
// QV Dang
// 11/30/2022
// My TicTacWhoa game
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


char Grid[3][3];
const char PLAYER1 = 'X';
const char PLAYER2 = 'O';

// ----------------------------------------------------------------------------

void resetGrid();
void printGrid();
int checkFreeSpaces();
int playerMove(int pNum);
char checkWinner();
void stateWinner(char);

// ----------------------------------------------------------------------------

int main()
{
   char winner = '_';
   char response = ' ';
   int pReturn;

   do
   {
      winner = '_';
      response = ' ';
      resetGrid();
      
    printf("\nMain Menu\nReady to play ? (Y/N): ");
    scanf(" %c", &response);
    response = toupper(response);
    
    if (response == 'N')
        {
            break;
        }

      while(winner == '_' && checkFreeSpaces() != 0)
      {
        
         printGrid();

         pReturn = playerMove(1);
         if (pReturn == -1)
         {
             return 0;
         }
         
         winner = checkWinner();
         if(winner != '_' || checkFreeSpaces() == 0)
         {
            break;
         }
         
         printGrid();
         
         pReturn = playerMove(2);
         if (pReturn == -1)
         {
             return 0;
         }
         
         winner = checkWinner();
         if(winner != '_' || checkFreeSpaces() == 0)
         {
            break;
         }
      }

      printGrid();
      stateWinner(winner);

      printf("\nWould you like to play again? (Y/N): ");
      scanf(" %c", &response);
      response = toupper(response);
   } while (response == 'Y');

   printf("\nThank you! Bye!\n");

   return 0;
}

// ----------------------------------------------------------------------------

void resetGrid()
{
   for(int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         Grid[i][j] = '_';
      }
   }
}

// ----------------------------------------------------------------------------

void printGrid()
{
   printf("\n_%c_|_%c_|_%c_\n", Grid[0][0], Grid[0][1], Grid[0][2]);
   printf("_%c_|_%c_|_%c_\n", Grid[1][0], Grid[1][1], Grid[1][2]);
   printf("_%c_|_%c_|_%c_\n", Grid[2][0], Grid[2][1], Grid[2][2]);
   
}

// ----------------------------------------------------------------------------

int checkFreeSpaces()
{
   int freeSpaces = 9;

   for(int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         if(Grid[i][j] != '_')
         {
            freeSpaces--;
         }
      }
   }
   return freeSpaces;
}

// ----------------------------------------------------------------------------

int playerMove(int pNum)
{
   int y;
   int x;
   char row;
   char column;
   char pInput[6];
   int cResult;
   
   char pSymbol;
   
   if (pNum == 1)
   {
       pSymbol = PLAYER1;
   }
   if (pNum == 2)
   {
       pSymbol = PLAYER2;
   }
   
   do
   {
    
      printf("Player %d's Move: ", pNum);
      scanf("%s", pInput);
      
      cResult = strcmp(pInput, "quit");
      if (cResult == 0)
      {
        return(-1);
      }
      
      column = pInput[0];
      row = pInput[1];
      
      if (strlen(pInput) > 2)
      {
        printf("Invalid move, please specify both column and row.\n");
        continue;
      }
      
      
      switch(column)
      {
        case 'A':
            y = 0;
            break;
        case 'B':
            y = 1;
            break;
        case 'C':
            y = 2;
            break;
        default:
            y = -1;
      }
      
      switch(row)
      {
        case '3':
            x = 0;
            break;
        case '2':
            x = 1;
            break;
        case '1':
            x = 2;
            break;
        default:
            x = -1;
      }
      
      if (x == -1 || y == -1)
      {
        printf("Invalid move, please specify both column and row.\n");
      }
      

      else
      {
        if(Grid[x][y] != '_')
        {
         printf("Invalid move, please specify both column and row.\n");
        }
        else
        {
            Grid[x][y] = pSymbol;
            break;
        }
      }
   } while (1);
   return(0);
   
}

// ----------------------------------------------------------------------------

char checkWinner()
{
   // check if any of the rows is a winning column
   for(int i = 0; i < 3; i++)
   {
      if(Grid[i][0] == Grid[i][1] && Grid[i][0] == Grid[i][2])
      {
         return Grid[i][0];
      }
   }
   // check if any of the columns is a winning column
   for(int i = 0; i < 3; i++)
   {
      if(Grid[0][i] == Grid[1][i] && Grid[0][i] == Grid[2][i])
      {
         return Grid[0][i];
      }
   }
   // check if any of the diagonals is a win
   if(Grid[0][0] == Grid[1][1] && Grid[0][0] == Grid[2][2])
   {
      return Grid[0][0];
   }
   if(Grid[0][2] == Grid[1][1] && Grid[0][2] == Grid[2][0])
   {
      return Grid[0][2];
   }

   return '_';
}

// ----------------------------------------------------------------------------

void stateWinner(char winner)
{
   if(winner == PLAYER1)
   {
      printf("PLAYER 1 WINS!\n");
   }
   else if(winner == PLAYER2)
   {
      printf("PLAYER 2 WINS!\n");
   }
   else{
      printf("IT'S A TIE!\n");
   }
}
