/* g++ -std=c++17 SlurryLabels.cpp -o SlurryLabels */
/* All dimentions are points unless otherwise stated */
/* All points are from the bottom corner unless otherwise stated*/
  
// generic rectangle class

#define BOTTOM 001
#define LEFT   001
#define TOP    792
#define RIGHT  612
#define XHALF  306
#define YHALF  396
#define DX     10
#define DY     10
#define XSL    XHALF - DX
#define YSL    YHALF - DY

#include<iostream>        // for cout
#include<fstream>         // for writing to .ps file
#include<string>          // for making strings
#include <ctime>          // get system date and time


using namespace std;

/* g++ -std=c++17 SlurryLabels.cpp -o SlurryLabels */
/* All dimentions are points unless otherwise stated */
/* All points are from the bottom corner unless otherwise stated*/
  
// generic rectangle class
class Rectangle
{
  float x0, y0, dx, dy;             // position and size of rectagle
  int   r=0, g=0, b=0;              // color (rgb)
  int thickness = 0;                // stroke thickness (zero is minimum)

  public:
    // set rectable position (lower left corner), size, color and thickness
    void set_position(float x0in, float y0in)     {x0 = x0in; y0 = y0in;}
    void set_size    (float dxin, float dyin)     {dx = dxin; dy = dyin;}
    void set_color   (int rin, int bin, int gin)  {r = rin; b = bin; g = gin;}
    void set_thick   (int thin)                   {thickness = thin;}

    // get some parameters
    float bottom()  {return y0;}
    float top()     {return y0 + dy;}
    float left()    {return x0;}
    float right()   {return x0 + dx;}
    float hcent()   {return x0 + dx/2;}
    float vcent()   {return y0 + dy/2;}

    // draw rectangle
    void draw(ofstream& file)
    {
      file << "newpath" << endl;
      file << x0    << ' ' << y0    << " moveto" << endl;
      file << x0    << ' ' << y0+dy << " lineto" << endl;
      file << x0+dx << ' ' << y0+dy << " lineto" << endl;
      file << x0+dx << ' ' << y0    << " lineto" << endl;
      file << "closepath" << endl;
      file << r << ' ' << b << ' ' << g << " setrgbcolor stroke" << endl;
    }
};

// generic text class
class Textfield
{
  string str = "";
  float  x0, y0;                  // starting point for text
  int    r = 0, g = 0, b = 0;     // optional text color
  int    fontsize = 12;           // font size
  bool   centered   = false;      // optional centered specification
  bool   underlined = false;      // optional underlined
  string font = "Arial";

  public:
    void set_txt(string strin="", float px=1.0, float py=1.0, int fsize = 12)
    {
      str = strin; x0 = px; y0 = py; fontsize = fsize;
    }
    void set_color (int rin, int bin, int gin) {r = rin; b = bin; g = gin;}
    void set_thick (int fsize) {fontsize = fsize;}
    void set_fot(string fontstr) {font = fontstr;}
    void set_centered(bool set) {centered = set;}
    void set_underlined(bool set) {underlined = set;}

    string get_txt() {return str;}

    void draw(ofstream& file)
    {
      file << "/" + font+ " findfont" << endl;
      file << fontsize << " scalefont setfont" << endl;
      file << "newpath" << endl;
      file << x0 << ' ' << y0 << " moveto" << endl;
      if(centered)
      {
        file << "(" << str << ") stringwidth pop 0.5 mul neg -10 rmoveto" << endl;
      }
      file << r << ' ' << b << ' ' << g << " setrgbcolor" << endl;
      file << "(" << str << ") ";
      if(underlined)
      {
        file << "dup show stringwidth pop dup neg 0 rlineto stroke" << endl;
      }
      else
      {
        file << "show" << endl;
      }
      file << "closepath" << endl;
    }
};

// returns the current date + specified offset (number of days)
string getdate(int nday = 0)
{
  // get current date
  time_t t0 = time(0) + (60*60*24 * nday);
  tm *ltm   = localtime(&t0);
  int year  = 1900 + ltm->tm_year;
  int month = 0001 + ltm->tm_mon;
  int day   = 0000 + ltm->tm_mday;

  return to_string(year) + "." + to_string(month) + "." + to_string(day);
}

// Slurry-lable class
class Slurrylabel
{
    string labelstr;
    float top, bottom, left, right, xcenter;

    Rectangle box;
    Textfield label;
    Textfield owner;
    Textfield date;
    Textfield expdate;
    // pH
    // HMIS

  public:
    void set_position(float x0, float y0)
    {
      left    = x0;
      bottom  = y0;
      right   = x0 + XSL;
      top     = y0 + YSL;
      xcenter = x0 + 0.5*XSL;
      box.set_position(left, bottom);
      box.set_size(XSL, YSL);
    }

    void set_lable(string str) 
    {
      label.set_txt(str, xcenter, top - 25);
      label.set_centered(true);
    }

    void set_date(int nday = 0)
    {
      string datestr = getdate(nday);
      date.set_txt("Date: "+datestr, left+5, top-75);
    }

      void set_expdate(int nday = 30)
    {
      string datestr = getdate(nday);
      expdate.set_txt("Exp.  Date: "+datestr, left+175, top-75);
    }
    
    void set_owner(string str) 
    {
      owner.set_txt("Owner: "+str, left+5, top - 50);
      owner.set_underlined(true);
    }

    void set_color(int r, int g, int b)
    {
      box.set_color(r, g, b);
      label.set_color(r, g, b);
      owner.set_color(r, g, b);
      date.set_color(r, g, b);
      expdate.set_color(r, g, b);

    }

    void draw(ofstream& file)
    {
      box.draw(file);
      label.draw(file);
      owner.draw(file);
      date.draw(file);
      expdate.draw(file);
    }
};

void makeSlurryLabels()
{
  ofstream myfile;
  myfile.open ("example2.ps");
  myfile << "%!PS-Adobe-3.0 EPSF-3.0" << endl << endl;

  Slurrylabel lab;
  lab.set_position(1, 1);
  lab.set_lable("Test Label");
  lab.set_owner("Nathan");
  lab.set_color(1,1,1);
  lab.set_date();
  lab.set_expdate(90);
  lab.draw(myfile);
}

int main()
{

  makeSlurryLabels();

  // cout << "test... " << getdate() << endl;

  cout << "Done!\n";
  return 0;
}


  // Rectangle box;
  // box.set_position(1.0, 1.0);
  // box.set_size(300.0, 390.0);
  // box.set_color(1,1,1);
  // box.draw(myfile);
  // float vtop = box.top()-25;
  // float hcent = box.hcent();
  // Textfield txt;
  // txt.set_txt("Slurry XXX", hcent, vtop, 20);
  // txt.set_centered(true);
  // txt.set_underlined(false);

  // txt.set_color(1, 0, 0);
  // txt.draw(myfile);