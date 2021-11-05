#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include "pixelwidget.hpp"
#include <math.h>
#define MAX_RGB 255
#define MIN_RGB 0






void PixelWidget::DefinePixelValues(){ //add pixels here; write methods like this for the assignments
  SetPixel(10,30,RGBVal(255,0,0));
}


// -----------------Most code below can remain untouched -------------------------
// ------but look at where DefinePixelValues is called in paintEvent--------------

PixelWidget::PixelWidget(unsigned int n_vertical, unsigned int n_horizontal):
  _n_vertical   (n_vertical),
  _n_horizontal (n_horizontal),
  _vec_rects(0)
{
  // all pixels are initialized to black
     for (unsigned int i_col = 0; i_col < n_vertical; i_col++)
       _vec_rects.push_back(std::vector<RGBVal>(n_horizontal));
}



void PixelWidget::SetPixel(unsigned int i_column, unsigned int i_row, const RGBVal& rgb)
{

  // if the pixel exists, set it
  if (i_column < _n_vertical && i_row < _n_horizontal)
    _vec_rects[i_column][i_row] = rgb;
}

void PixelWidget::DrawLine(pixel start_p, pixel end_p){

    int r_diff=0,g_diff=0, b_diff=0;

    //calculate diffs
    r_diff = -(start_p.rgbVal._red - end_p.rgbVal._red);
    g_diff = -(start_p.rgbVal._green - end_p.rgbVal._green);
    b_diff = -(start_p.rgbVal._blue - end_p.rgbVal._blue);







    for(double i =0.0f; i<=1.00f; i+=0.01f){

        double x= (end_p.x + (i * (start_p.x-end_p.x)));
        double y= (end_p.y + (i * (start_p.y-end_p.y)));

        RGBVal newRGBVal;
        //add accumulative diff to each rgb val
        newRGBVal._red= (end_p.rgbVal._red + (r_diff*-i));
        newRGBVal._green= (end_p.rgbVal._green + (g_diff*-i));
        newRGBVal._blue= (end_p.rgbVal._blue + (b_diff*-i));

        //if diff exceeds max rgb val, remain ta max
        newRGBVal._red >= MAX_RGB ? newRGBVal._red = MAX_RGB : newRGBVal._red = newRGBVal._red;
        newRGBVal._red <= MIN_RGB ? newRGBVal._red = MIN_RGB : newRGBVal._red = newRGBVal._red;

        newRGBVal._green >= MAX_RGB ? newRGBVal._green = MAX_RGB : newRGBVal._green = newRGBVal._green;
        newRGBVal._green <= MIN_RGB ? newRGBVal._green = MIN_RGB : newRGBVal._green = newRGBVal._green;

        newRGBVal._blue >= MAX_RGB ? newRGBVal._blue = MAX_RGB : newRGBVal._blue = newRGBVal._blue;
        newRGBVal._blue <= MIN_RGB ? newRGBVal._blue = MIN_RGB : newRGBVal._blue = newRGBVal._blue;


        SetPixel((int)x,(int)y,newRGBVal);

    }


}


void PixelWidget::DrawTriangle(pixel p, pixel r, pixel q){

    for(double a =0.0f; a<=1.0f; a+=0.01f){
        for(double b =0.0f; b<=1.0f; b+=0.01f){

            double x= (a*p.x + b*q.x + (1-a-b)* q.x);
            double y= (a*p.y + b*q.y + (1-a-b)* q.y);
            RGBVal newRGBVal;
            //add accumulative diff to each rgb val
            newRGBVal._red= a*p.rgbVal._red + b*q.rgbVal._red + (1-a-b)* q.rgbVal._red;
            newRGBVal._green= a*p.rgbVal._green + b*q.rgbVal._green + (1-a-b)* q.rgbVal._green;
            newRGBVal._blue= a*p.rgbVal._blue + b*q.rgbVal._blue + (1-a-b)* q.rgbVal._blue;



            pixel x1;
            x1.x =x;
            x1.y = y;
            x1.rgbVal = newRGBVal;
            DrawLine(x1,r);
        }


    }

}


bool PixelWidget::IsInside(pixel p, pixel r, pixel q, point f){




}

void PixelWidget::createFile(pixel p, pixel r, pixel q){

}



void PixelWidget::paintEvent( QPaintEvent * )
{

  QPainter p( this );
  // no antialiasing, want thin lines between the cell
  p.setRenderHint( QPainter::Antialiasing, false );

  // set window/viewport so that the size fits the screen, within reason
  p.setWindow(QRect(-1.,-1.,_n_vertical+2,_n_horizontal+2));
  int side = qMin(width(), height());
  p.setViewport(0, 0, side, side);

  // black thin boundary around the cells
  QPen pen( Qt::black );
  pen.setWidth(0.);

  // here the pixel values defined by the user are set in the pixel array
  DefinePixelValues();


  pixel a,b,c,d,e,f,g,h,i;
  a.x=5.0;
  a.y=5.0;
  a.rgbVal=RGBVal(255,0,0);

  b.x=55.0;
  b.y=5.5;
  b.rgbVal=RGBVal(0,255,0);



  c.x=30.0;
  c.y=50.0;
  c.rgbVal=RGBVal(0,0,255);

  DrawTriangle(a,b,c);

  d.x = 2.5;
  d.y = 50.0;
  d.rgbVal = RGBVal(0,0,255);

  e.x = 2.5;
  e.y = 5.0;
  e.rgbVal = RGBVal(255,0,0);
  DrawLine(e,d);

  f.x = 5.0;
  f.y = 50.0;
  f.rgbVal = RGBVal(255,0,0);

  g.x = 55.0;
  g.y = 50.0;
  g.rgbVal = RGBVal(0,255,0);
  DrawLine(f,g);

  h.x = 57.5;
  h.y = 5.0;
  h.rgbVal = RGBVal(0,255,0);

  i.x = 57.5;
  i.y = 50.0;
  i.rgbVal = RGBVal(0,0,255);
  DrawLine(h,i);



  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++)
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);

      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);
    }
}
