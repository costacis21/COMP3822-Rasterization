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




    //calculate manhattan distance between the two points
    float steps = fabs((end_p.x-start_p.x)) + fabs((end_p.y-start_p.y));
    steps*=10;
    float stepsize=0.001;

    for(int i = 0; i<=steps; i++){ //number of steps = distance


        //step size i/steps

        stepsize= i/steps;
        float x= (end_p.x + (stepsize * (start_p.x-end_p.x)));
        float y= (end_p.y + (stepsize * (start_p.y-end_p.y)));

        RGBVal newRGBVal;
        //add accumulative diff to each rgb val
        newRGBVal._red= (end_p.rgbVal._red + (r_diff*-stepsize));
        newRGBVal._green= (end_p.rgbVal._green + (g_diff*-stepsize));
        newRGBVal._blue= (end_p.rgbVal._blue + (b_diff*-stepsize));

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


  pixel a,b,c,d;
  a.x=20.3;
  a.y=0.0;
  b.rgbVal=RGBVal(255,0,50);

  b.x=0.6;
  b.y=20.8;
  a.rgbVal=RGBVal(0,100,100);
  DrawLine(b,a);



  c.x=50.0;
  c.y=0.0;
  d.rgbVal=RGBVal(20,100,100);

  d.x=20.6;
  d.y=20.8;
  c.rgbVal=RGBVal(100,255,200);
  DrawLine(d,c);

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
