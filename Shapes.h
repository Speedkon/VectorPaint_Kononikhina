#ifndef _VectorPaint_Shapes_h_
#define _VectorPaint_Shapes_h_

#include <cmath>
#include <CtrlLib/CtrlLib.h>

//Базовая фигура
//состоит из масива точек, имени и цвета фигуры

#define EUCLID(points) (std::sqrt((points[0].x-points[1].x)*(points[0].x-points[1].x)+(points[0].y-points[1].y)*(points[0].y-points[1].y)));

#define ABS_VAL(vec) (std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]))

class Shape{
	friend class OperationsHelper;
	
	protected:
		std::string name;
		Upp::Color color;
		Upp::Color inner_color;
		int width;
		
		bool selected;
		Upp::Vector<Upp::Point> points;
	public:
		Shape(std::string nm):name(nm),width(1){}
		virtual ~Shape(){}

		void setSelect(bool sel){
			selected=sel;
		}
		int size(){
			return points.size();
		}
		virtual void setponit(int i, Upp::Point pnt){
			if(i<points.size()) points[i]=pnt;
		}
		std::string getName(){
			return name;
		}
		void setName(std::string an){
			name=an;
		}
		Upp::Color getColor(){
			return color;
		}
		void setColor(Upp::Color cl){
			color=cl;
		}
		Upp::Color getInnerColor(){
			return inner_color;
		}
		void setInnerColor(Upp::Color cl){
			inner_color = cl;
		}
		int getWidth(){
			return width;
		}
		void setWidth(int w){
			width=w;
		}
		virtual void paint(Upp::Draw& w)=0;
		virtual void moveto(Upp::Point to)=0;	
};


//Наследник базовой фигуры
//Линия - две точки
class Line: public Shape{
	public:
		Line(std::string name="line"):Shape(name){
			points.push_back(Upp::Point(100,100));
			points.push_back(Upp::Point(300,300));
			
			color=Upp::Black();
		}
		
		virtual void paint(Upp::Draw& w){
			w.DrawLine(points[0], points[1],
				selected? 3: width,
				color);
		}
		
		virtual void moveto(Upp::Point to){
			int dx=points[0].x-points[1].x;
			int dy=points[0].y-points[1].y;
			
			points[0]=to;
			points[1]=Upp::Point(to);
			points[1].Offset(-dx,-dy);
		}
};

//Наследник Линии, состоит из трех точек

class Triangle: public Line{
	public:
		Triangle(std::string name="triangle"): Line(name){
			points.push_back(Upp::Point(100,300));
			
			color=Upp::Red();
			width = 3;
		}
		virtual void paint(Upp::Draw& w){
			//w.DrawLine(points[0], points[1],selected?3: width, color);	
			//w.DrawLine(points[1], points[2],selected?3: width, color);
			//w.DrawLine(points[2], points[0],selected?3: width, color);
			w.DrawPolygon(points, inner_color, width, color);
		}
		
		virtual void moveto(Upp::Point to){
			int dx=points[0].x-points[1].x;
			int dy=points[0].y-points[1].y;
			int dx1=points[0].x-points[2].x;
			int dy1=points[0].y-points[2].y;
			
			points[0]=to;
			points[1]=Upp::Point(to);
			points[1].Offset(-dx,-dy);
			points[2]=Upp::Point(to);
			points[2].Offset(-dx1,-dy1);
		}
};

class Circle: public Line{
	public:
		Circle(std::string name = "circle"): Line(name){
			color = Upp::Red();
			width = 3;
		}
		
		virtual void paint(Upp::Draw& w){
			float p_dist = EUCLID(points);
			
			int radius = int(sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x) +
                              (points[1].y - points[0].y) * (points[1].y - points[0].y)));
			
			w.DrawEllipse(points[0].x - radius, points[0].y - radius, radius * 2, radius * 2, inner_color, width, color);
		}
};

//Класс текст
class TextShape: public Shape{
	public:
		TextShape(std::string name="text"):Shape(name){
			points.push_back(Upp::Point(100,100));
		
			color=Upp::Green();
		}
		
		virtual void paint(Upp::Draw& w){
			w.DrawText(points[0].x, points[0].y, name, Upp::Arial(10+width), color);
		}
		
		virtual void moveto(Upp::Point to){
			points[0]=to;
		}
};


class ImageShape: public Shape{
	Upp::Image img;
	
	public:
		ImageShape(Upp::Image image, std::string name="image"):Shape(name){
			points.push_back(Upp::Point(100,100));
			img = image;
		}
		
		virtual void paint(Upp::Draw& w){
			w.DrawImage(points[0].x, points[0].y, img);
		}
		
		virtual void moveto(Upp::Point to){
			points[0]=to;
		}
};

//Фрактал Мандельброта
//задается двкмя точками

class Mandelbrot: public Line{
	private:
		Upp::Image img;
		long double scale=0;
		long double yscale=0;
		const int maxIt=100;
		int num_iters(double cx, double cy) {
	        std::complex<double> z(0.0, 0.0);
	        std::complex<double> c(cx, cy);
	        for (int i = 0; i <= maxIt; ++i) {
	            if (std::abs(z) > 2.0) return i;
	            z = z * z + c;
	        }
	        return maxIt;
		}
		
	public:
		Mandelbrot(std::string name="mandelbrot"):Line(name){
			update();
		}
		
		virtual void setponit(int i, Upp::Point pnt){
			Shape::setponit(i,pnt);
			
			update();
		}
		
		virtual void paint(Upp::Draw& w){
			w.DrawImage(points[0].x, points[0].y, img);
		}
		
		void update(){
			int width = points[1].x - points[0].x;
			int height = points[1].y - points[0].y;
			if(width<=0||height<=0)return;
			
			Upp::ImageBuffer ib(width, height);
			Upp::RGBA* t = ~ib;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					double cx = (double)x/ width*2.0/(1.0+scale) - 1.5;
					double cy = (double)y/ height*2.0/(1.0+yscale) - 1.0;
					int iterations = num_iters(cx, cy);
					Upp::Color color = Upp::HsvColorf(double(1.0/iterations) , 1, (iterations < maxIt));
					*t++ = color;
				}
			}
			img = ib;
		}
		
		long double& getScale(){
			return scale;
		}
		long double& getYScale(){
			return yscale;
		}
};


#endif
