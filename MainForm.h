#ifndef _VectorPaint_MainForm_h_
#define _VectorPaint_MainForm_h_

#include <CtrlLib/CtrlLib.h>
#include "PictureController.h"
#include "ShapePicture.h"
#include "Propertys.h"
#include "Operations.h"

#define LAYOUTFILE <VectorPaint/MainForm.lay>
#include <CtrlCore/lay.h>

#include <iostream>

using namespace Upp;

//Главная форма программы
//содержит изображение

class MainForm : public WithMainFormLayout<TopWindow> {
private:
    ShapePicture pict;
    
    StatusBar status;
    InfoCtrl shapesNum;

    void updateList() {
        nameList.Clear();
        for (int i = 0; i < pict.size(); i++)
            nameList.Add(pict.getShape(i)->getName());
        
        shapesNum.Set(0, "currently shapes stored: " + AsString(pict.size()), 200);
    }

    void OnAddLine() {
        Shape* shp = pict.addShape("line");
        pictController.startInput(shp);
        updateList();
    }

    void OnAddTri() {
        Shape* shp = pict.addShape("tri");
        pictController.startInput(shp);
        updateList();
    }

    void OnAddFrci() {
        Shape* shp = pict.addShape("frc");
        pictController.startInput(shp);
        updateList();
    }

    void OnAddText() {
        Shape* shp = pict.addShape("text");
        pictController.startInput(shp);
        updateList();
    }
   
    void OnMoveSelected() {
        if (nameList.GetCursor() >= 0) {
            pictController.startMove(pict.getShape(nameList.GetCursor()));
        }
    }

    void OnResizeSelected() {
        if (nameList.GetCursor() >= 0) {
            pictController.startResize(pict.getShape(nameList.GetCursor()));
        }
    }

	void OnRotateSelected() {
		int c=nameList.GetCursor();
				if(c>=0){
					OperationsHelper::rotate(pict.getShape(c), 10);	
					Refresh();	
				}
	}

	void OnScaleSelected() {
		int c=nameList.GetCursor();
				if(c>=0){
					OperationsHelper::scale(pict.getShape(c), 1.5);	
					Refresh();	
				}
	}

	void OnLeftRotateSelected() {
		int c=nameList.GetCursor();
				if(c>=0){
					OperationsHelper::rotate(pict.getShape(c), -10);	
					Refresh();	
				}
	}

	void OnLeftScaleSelected() {
		int c=nameList.GetCursor();
				if(c>=0){
					OperationsHelper::scale(pict.getShape(c), 1.0/1.5);	
					Refresh();	
				}
	}

    void OnPropertySelected() {
        int c = nameList.GetCursor();
        if (c >= 0) {
            PropertyHelper::showDialog(pict.getShape(c));
            updateList();
            nameList.SetCursor(c);
        }
    }
   
public:
    MainForm() {
        CtrlLayout(*this, "Vector paint");
        Zoomable().Sizeable();
        pictController.init(&pict);

		AddFrame(status);
		status.AddFrame(shapesNum.Left(200));
		
		shapesNum.Set(0, "currently shapes stored: 0", 200);

		status = "Welcome to VectorPaint";

        // Existing button bindings
        addLineButton << [=] { OnAddLine(); };
        addTriButton << [=] { OnAddTri(); };
        addFrciButton << [=] { OnAddFrci(); };
        addTextButton << [=] { OnAddText(); };

        moveSelected << [=] { OnMoveSelected(); };
        resizeSelected << [=] { OnResizeSelected(); };
        proprtySelected << [=] { OnPropertySelected(); };
        rotateSelected << [=] {OnRotateSelected(); };
        scaleSelected << [=] {OnScaleSelected(); };
        rotateLeftSelected << [=] {OnLeftRotateSelected(); };
        scaleLeftSelected << [=] {OnLeftScaleSelected(); };
    }
};


#endif