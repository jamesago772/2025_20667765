#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QStatusBar>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkActor.h>
#include <vtkCylinderSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // ----------------------------
    // VTK: initialise MEMBER renderWindow + renderer (not local variables)
    // ----------------------------
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();

    // If your promoted widget is named "widget", keep ui->widget
    // If you renamed it, use that name instead.
    ui->widget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    // Optional: show a cylinder at startup (you can remove later)
    {
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
    }

    // Status bar messages
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    // ----------------------------
    // Tree model
    // ----------------------------
    partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(partList);

    // (Optional demo tree) - you can keep this, but it creates dummy items
    ModelPart* rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible }, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString cname = QString("Item %1,%2").arg(i).arg(j);
            QString cvis("true");

            ModelPart* childChildItem = new ModelPart({ cname, cvis }, childItem);
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionOpen_File_triggered()
{
    on_loadButton_clicked();
}

void MainWindow::on_loadButton_clicked()
{
    emit statusUpdateMessage("Load button clicked", 3000);

    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open STL file",
        "",
        "STL Files (*.stl)"
    );

    if (filename.isEmpty())
        return;

    QModelIndex parentIndex = ui->treeView->currentIndex();
    ModelPart* part = partList->addPart(filename, parentIndex);
    if (!part)
        return;

    part->loadSTL(filename);

    updateRender();
}

void MainWindow::on_resetButton_clicked()
{
    emit statusUpdateMessage("Reset button clicked", 3000);

    // Optional: reset camera
    if (renderer) {
        renderer->ResetCamera();
        renderWindow->Render();
    }
}

void MainWindow::updateRender()
{
    if (!renderer || !renderWindow || !partList)
        return;

    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid())
    {
        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
        if (part && part->getActor())
        {
            renderer->AddActor(part->getActor());
        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
        return;

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; ++i)
        updateRenderFromTree(partList->index(i, 0, index));
}