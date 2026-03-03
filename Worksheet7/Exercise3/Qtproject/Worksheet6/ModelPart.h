#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>
#include <vtkSmartPointer.h>

class vtkSTLReader;
class vtkPolyDataMapper;
class vtkActor;

class ModelPart {
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;

    int columnCount() const;
    QVariant data(int column) const;
    void set(int column, const QVariant& value);

    ModelPart* parentItem();
    int row() const;

    // --- Worksheet 7: STL loading ---
    void loadSTL(const QString& filename);
    vtkSmartPointer<vtkActor> getActor() const { return actor; }

    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);
    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    void setVisible(bool isVisible);
    bool visible();

private:
    // VTK pipeline objects
    vtkSmartPointer<vtkSTLReader> reader;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    QList<ModelPart*> m_childItems;
    QList<QVariant>   m_itemData;
    ModelPart* m_parentItem;

    bool isVisible = true;
};

#endif