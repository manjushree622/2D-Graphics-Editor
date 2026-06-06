#include<stdio.h>
#define rows 30
#define columns 30
char canvas[rows][columns];
void initializeCanvas()
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
void drawCircle(int xc,int yc,int r)
{
  int x,y,d;
  for(y=0;y<rows;y++)
   {
     for(x=0;x<columns;x++)
      {
        d=(x-xc)*(x-xc)+(y-yc)*(y-yc);
         if(d<=r*r)
          {
           canvas[y][x]='*';
          }
      }
   }
}
void deleteArea(int row,int col,int height,int width)
{
  int i,j;
  for(i=row;i<row+height;i++)
  {
    for(j=col;j<col+width;j++)
     {
       canvas[i][j]=' ';
     }
  }
}
void  deleteCircle(int xc,int yc,int r)
{
  int x,y;
for(y=0;y<rows;y++)
 {
for(x=0;x<columns;x++)
 {
 if((x-xc)*(x-xc)+(y-yc)*(y-yc)<=r*r)
   {
  canvas[y][x]=' ';
   }
  }
 }
}

  

void modifyRectangle(int oldrow,int oldcol,int oldheight,int oldwidth,int newrow,int newcol,int newheight,int newwidth)
{
  deleteArea(oldrow,oldcol,oldheight,oldwidth);
  drawRectangle(newrow,newcol,newheight,newwidth);
}
void deleteCircle(int xc,int yc,int r)
{
  int x,y;
for(y=0;y<rows;y++)
{
for(x=0;x<columns;x++)
 {
  if((x-xc)*(x-xc) + (y-yc)*(y-yc)<= r*r)
   {
     canvas[y][x]=' ';
   }
 }
}
}

void modifyCircle(int oldxc,int oldyc,int oldr,int newxc,int newyc,int newr)
{
  deleteCircle(oldxc,oldyc,oldr);
  drawCircle(newxc,newyc,newr);
}
void modifyLine(int oldrow1,int oldcol1,int oldrow2,int oldcol2,int newrow1,int newcol1,int newrow2,int newcol2)
{
  deleteArea(oldrow1,oldcol1,oldrow2-oldrow1+1,oldcol2-oldcol1+1);
  drawLine(newrow1,newcol1,newrow2,newcol2);
}
void modifyTriangle(int oldrow,int oldcol,int oldheight,int newrow,int newcol,int newheight)
{
  deleteArea(oldrow,oldcol-oldheight,oldheight,2*oldheight+1);
  drawTriangle(newrow,newcol,newheight);
}
int main()
{
  int choice;
  int row,col,height,width;
  int row1,col1,row2,col2;
  int trow,tcol,theight;
  int xc,yc,r;
  int drow,dcol,dheight,dwidth;
  int oldrow,oldcol,oldheight,oldwidth;
  int newrow,newcol,newheight,newwidth;
  int oldxc,oldyc,oldr;
  int newxc,newyc,newr;
  int oldrow1,oldcol1,oldrow2,oldcol2;
  int newrow1,newcol1,newrow2,newcol2;
  int oldtrow,oldtcol,oldtheight;
  int newtrow,newtcol,newtheight;
  initializeCanvas();
  do
  {
    printf("\n1. Draw Rectangle");
    printf("\n2. Draw Line");
    printf("\n3. Draw Triangle");
    printf("\n4. Draw Circle");
    printf("\n5. Delete Area");
    printf("\n6. Display Canvas");
    printf("\n7. Modify Rectangle");
    printf("\n8. Modify Circle");
    printf("\n9. Modify Line");
    printf("\n10. Modify Triangle");
    printf("\n11. Exit");
    printf("\n Enter choice:");
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
        printf("Enter center row center column radius:");
        scanf("%d%d%d",&yc,&xc,&r);
        drawCircle(xc,yc,r);
        break;
      case 5:
        printf("Enter row column height width:");
        scanf("%d%d%d%d",&drow,&dcol,&dheight,&dwidth);
        deleteArea(drow,dcol,dheight,dwidth);
       break;
      case 6:
        displayCanvas();
        break;
      case 7:
        printf("Enter old row col height width:");
        scanf("%d%d%d%d",&oldrow,&oldcol,&oldheight,&oldwidth);
        printf("Enter new row col height width:");
        scanf("%d%d%d%d",&newrow,&newcol,&newheight,&newwidth);
        modifyRectangle(oldrow,oldcol,oldheight,oldwidth,newrow,newcol,newheight,newwidth);
        break;
      case 8:
        printf("Enter old center row center column radius:");
        scanf("%d%d%d",&oldyc,&oldxc,&oldr);
        printf("Enter new center row center column radius:");
        scanf("%d%d%d",&newyc,&newxc,&newr);
        modifyCircle(oldxc,oldyc,oldr,newxc,newyc,newr);
        break;
      case 9:
        printf("Enter old row1 col1 row2 col2:");
        scanf("%d%d%d%d",&oldrow1,&oldcol1,&oldrow2,&oldcol2);
        printf("Enter new row1 col1 row2 col2:");
        scanf("%d%d%d%d",&newrow1,&newcol1,&newrow2,&newcol2);
        modifyLine(oldrow1,oldcol1,oldrow2,oldcol2,newrow1,newcol1,newrow2,newcol2);
        break;
      case 10:
        
        printf("Enter old row col height:");
        scanf("%d%d%d",&oldtrow,&oldtcol,&oldtheight);
        printf("Enter new row col height:");
        scanf("%d%d%d",&newtrow,&newtcol,&newtheight);
        modifyTriangle(oldtrow,oldtcol,oldtheight,newtrow,newtcol,newtheight);
        break;
      case 11:
        printf("Exiting .....\n");
      break;
       default:
         printf("Invalid choice \n");
     }
  }while(choice!=11);
  return 0;
}
           
