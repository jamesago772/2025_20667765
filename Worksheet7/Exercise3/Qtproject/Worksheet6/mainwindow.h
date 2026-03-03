#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "ModelPart.h"
#include "ModelPartList.h"
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkSmartPointer.h>

// ✅ Forward declarations MUST be at global scope (NOT inside Ui namespace)
class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_resetButton_clicked();
    void on_actionOpen_File_triggered();


signals:
    void statusUpdateMessage(const QString& message, int timeout);

private:
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

    // ✅ Typed smart pointers (now valid because forward decls are correct)
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    Ui::MainWindow* ui;
    ModelPartList* partList;
};

#endif // MAINWINDOW_H