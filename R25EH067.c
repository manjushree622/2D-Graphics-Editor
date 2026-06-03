#include<stdio.h>
#define rows 30
#define columns 30
char canvas[rows][columns];
void initilizeCanvas()
{
  int i,j;
  for(i=0;i<rows;i++)
  {
    for(j=0;j<columns;j++)
     {
       canvas[i][j]=' ';
     }
  }
}
void displayCanvas()
{
  int i,j;
  for(i=0;i<rows;i++)
  {
    for(j=0;j<columns;j++)
     {
       printf("%c",canvas[i][j]);
     }
    printf("\n");
  }
}
void drawRectangle(int row,int col,int height,int width)
{
  int i;
  for(i=col;i<col+width;i++)
   {
     canvas[row][i]='*';
     canvas[row+height-1][i]='*';
   }
   for(i=row;i<row+height;i++)
   {
      canvas[i][col]='*';
      canvas[i][col+width-1]='*';
   }
}
int main()
{
  int choice;
  int row,col,height,width;
  initilizeCanvas();
  do
  {
    printf("\n1. Draw Rectangle");
    printf("\n2. Display Canvas");
    printf("\n3.Exit");
    printf("\nEnter choice:");
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
      printf("Enter row column height width:");
      scanf("%d%d%d%d",&row,&col,&height,&width);
      drawRectangle(row,col,height,width);
      break;
     case 2:
       displayCanvas();
       break;
      case 3:
        printf("Exiting...\n");
        break;
       default:
         printf("Invalid choice \n");
     }
  }while(choice!=3);
  return 0;
}
           
