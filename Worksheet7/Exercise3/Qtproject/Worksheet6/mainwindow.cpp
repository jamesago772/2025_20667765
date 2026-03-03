#include "mainwindow.h"
#include <QDebug>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkCylinderSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->widget->setRenderWindow(renderWindow);

    vtkNew<vtkRenderer> renderer;
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(12);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());
    renderer->AddActor(actor);
    renderer->ResetCamera();
    renderWindow->Render();

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
    // Create / allocate the ModelPartList
    this->partList = new ModelPartList("Parts List");

    // Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // Manually create a model tree (demo)
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadButton_clicked()
{

qDebug() << "Load button clicked";
emit statusUpdateMessage(QString("Load button was clicked"), 0);
}


void MainWindow::on_resetButton_clicked()
{
 emit statusUpdateMessage("Reset button was clicked", 0);
}

