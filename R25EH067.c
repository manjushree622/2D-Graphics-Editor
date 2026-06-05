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
void drawLine(int row1,int col1,int row2,int col2)
{
  int i;
if(row1==row2)
 {
for(i=col1;i<=col2;i++)
  canvas[row1][i]='*';
 }
else
if(col1==col2)
 {
for(i=row1;i<=row2;i++)
  canvas[i][col1]='*';
 }
}
void drawTriangle(int row,int col,int height)
{
  int i;
   for(i=0;i<height;i++)
{
  canvas[row+i][col-i]='*';
  canvas[row+i][col+i]='*';
}
for(i=col-height+1;i<=col+height-1;i++)
  {
   canvas[row+height-1][i]='*';
  }
}
int main()
{
  int choice;
  int row,col,height,width;
  int row1,col1,row2,col2;
  int trow,tcol,theight;
  initilizeCanvas();
  do
  {
    printf("\n1. Draw Rectangle");
    printf("\n2.Draw Line");
    printf("\n3. Draw Triangle");
    printf("\n4. Display Canvas");
    printf("\n5.Exit");
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
       printf("Enter row1 col1 row2 col2:");
    scanf("%d%d%d%d",&row1,&col1,&row2,&col2);
       drawLine(row1,col1,row2,col2);
       break;
      case 3:
        printf("Enter row column height:");
        scanf("%d%d%d",&trow,&tcol,&theight);
        drawTriangle(trow,tcol,theight);
      break;
      
      case 4:
        displayCanvas();
        break;
      case 5:
        printf("Exiting .....\n");
      break;
       default:
         printf("Invalid choice \n");
     }
  }while(choice!=5);
  return 0;
}
           
