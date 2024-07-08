#ifndef _VectorPaint_Picture_h_
#define _VectorPaint_Picture_h_

#include <CtrlLib/CtrlLib.h>
#include <cstring>

//Изображение из фигур
//Хранит массив фигур изображения
//выводит изображение на форму

class ShapePicture{
	private:
		Upp::Vector<Shape*> shps;
		
	public:
		~ShapePicture(){
			for(int i=0;i<shps.size();i++)
				delete shps[i];	
		}
		
		void paint(Upp::Draw& w){
			for(int i=0;i<shps.size();i++)
				shps[i]->paint(w);
		}
		
		int size(){
			return shps.size();
		}
		
		Shape* getShape(int i){
			if(i<size())
				return shps[i];
			else
				return NULL;
		}
		
		Shape* addShape(std::string kind){
			Shape *sp=NULL;
			//TODO тут изменить добавление новых типов фигур
			if(kind=="line") sp=new Line();
			if(kind=="tri") sp=new Triangle();
			if(kind=="frc") sp=new Mandelbrot();
			if(kind=="text") sp=new TextShape();
			if(kind=="circle") sp=new Circle();

			if(sp!=NULL)shps.push_back(sp);
			else{
				std::string msg = std::string("unknown shape: ") + kind;
				int len = msg.length();
				char* msgChar = new char[len+1];
				std::strcpy(msgChar, msg.c_str());
			 	Upp::PromptOK(msgChar);
			}
			return sp;
		}	
		
		Shape* addImage(std::string kind, Upp::Image img){
			Shape *sp=NULL;
			if(kind=="image") sp = new ImageShape(img);

			if(sp!=NULL)shps.push_back(sp);
			else{
				std::string msg = std::string("unknown shape: ") + kind;
				int len = msg.length();
				char* msgChar = new char[len+1];
				std::strcpy(msgChar, msg.c_str());
			 	Upp::PromptOK(msgChar);
			}
			return sp;
		}
};

#endif
