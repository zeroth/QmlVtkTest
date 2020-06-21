#include "VtkSurfaceItem.h"
#include <QOpenGLFramebufferObject>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QVTKInteractor.h>
#include <QVTKInteractorAdapter.h>

#include <array>

class VtkSurfaceRenderer
        : public QQuickFramebufferObject::Renderer
{
public:
    VtkSurfaceRenderer(): m_initialized(false){}
    ~VtkSurfaceRenderer(){}

    // Renderer interface
    void render() {
        renderWindow->Render();
        renderWindow->Start();
    }


    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
        QOpenGLFramebufferObject *fbo = new QOpenGLFramebufferObject(size);
        fbo->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        return fbo;
    }

    void synchronize(QQuickFramebufferObject * item) {
        this->initialize();
        VtkSurfaceItem *dcmItem = (VtkSurfaceItem*)item;
        QList<QEvent*> events = dcmItem->pendingEvents();
        QVTKInteractorAdapter adapter(0);
        Q_FOREACH(QEvent *event, events)
            adapter.ProcessEvent(event, renderWindowInteractor);
        qDeleteAll(events);
    }

    void initialize() {
        if(m_initialized)
            return;
        colors = vtkSmartPointer<vtkNamedColors>::New();

        // Set the background color.
        std::array<unsigned char, 4> bkg{{26, 51, 102, 255}};
        colors->SetColor("BkgColor", bkg.data());
        cylinder = vtkSmartPointer<vtkCylinderSource>::New();
        cylinder->SetResolution(8);

        cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
        cylinderActor = vtkSmartPointer<vtkActor>::New();
        cylinderActor->SetMapper(cylinderMapper);
        cylinderActor->GetProperty()->SetColor(
                    colors->GetColor4d("Tomato").GetData());
        cylinderActor->RotateX(30.0);
        cylinderActor->RotateY(-45.0);

        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(cylinderActor);
        renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
        // Zoom in a little by accessing the camera and invoking its "Zoom" method.
        renderer->ResetCamera();
        renderer->GetActiveCamera()->Zoom(1.5);

        renderWindow = vtkSmartPointer<vtkExternalOpenGLRenderWindow>::New();
        renderWindow->SetSize(300, 300);
        renderWindow->AddRenderer(renderer);
        renderWindow->SetWindowName("Cylinder");
        renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);

        vtkInteractorStyle* s = vtkInteractorStyleTrackballCamera::New();
        renderWindowInteractor->SetInteractorStyle(s);
        s->Delete();
        renderWindowInteractor->Initialize();
        m_initialized = true;
    }


private:
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkCylinderSource> cylinder;
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper;
    vtkSmartPointer<vtkActor> cylinderActor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkExternalOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    bool m_initialized;

};

VtkSurfaceItem::VtkSurfaceItem()
{
    this->setAcceptedMouseButtons(Qt::AllButtons);
}

VtkSurfaceItem::~VtkSurfaceItem()
{

}
QQuickFramebufferObject::Renderer *VtkSurfaceItem::createRenderer() const
{
    return new VtkSurfaceRenderer;
}


void VtkSurfaceItem::keyPressEvent(QKeyEvent *event)
{
    m_pendingEvents << new QKeyEvent(event->type(), event->key(),
                                  event->modifiers(), event->text());
    this->update();
}

void VtkSurfaceItem::keyReleaseEvent(QKeyEvent *event)
{
    m_pendingEvents << new QKeyEvent(event->type(), event->key(),
                                  event->modifiers(), event->text());
    this->update();
}

void VtkSurfaceItem::mousePressEvent(QMouseEvent *event)
{
    m_pendingEvents << new QMouseEvent(event->type(), tx(event->localPos()),
                                    tx(event->windowPos()), tx(event->screenPos()),
                                    event->button(), event->buttons(),
                                    event->modifiers(), event->source());

    this->setFocus(true);
    this->forceActiveFocus();

    this->update();

}

void VtkSurfaceItem::mouseMoveEvent(QMouseEvent *event)
{
    m_pendingEvents << new QMouseEvent(event->type(), tx(event->localPos()),
                                    tx(event->windowPos()), tx(event->screenPos()),
                                    event->button(), event->buttons(),
                                    event->modifiers(), event->source());
    this->update();
}

void VtkSurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
    m_pendingEvents << new QMouseEvent(event->type(), tx(event->localPos()),
                                    tx(event->windowPos()), tx(event->screenPos()),
                                    event->button(), event->buttons(),
                                    event->modifiers(), event->source());
    this->update();
}

void VtkSurfaceItem::wheelEvent(QWheelEvent *event)
{
    m_pendingEvents << new QWheelEvent(tx(event->posF()), tx(event->globalPosF()), event->pixelDelta(),
                                        event->angleDelta(), event->delta(), event->orientation(),
                                        event->buttons(), event->modifiers(), event->phase(),
                                        event->source(), true);
    this->update();
}

