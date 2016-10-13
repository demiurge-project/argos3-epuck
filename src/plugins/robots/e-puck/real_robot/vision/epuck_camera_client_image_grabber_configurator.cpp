#include "epuck_camera_client_image_grabber_configurator.h"
#include "image.h"

#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <QGridLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include <QPainter>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

namespace argos {

   /****************************************/
   /****************************************/

   CEPuckCameraClientImageGrabberImageWidget::CEPuckCameraClientImageGrabberImageWidget(QWidget* pc_parent) :
      QLabel(pc_parent) {
   }

   /****************************************/
   /****************************************/
   
   void CEPuckCameraClientImageGrabberImageWidget::SetImage(const CByteArray& c_hsv_data) {

      /* Convert image to RGB */
      m_cRGBImageData.Resize(c_hsv_data.Size());
      HSV888toRGB888(m_nImageWidth, m_nImageHeight,
                     m_cRGBImageData.ToCArray(), c_hsv_data.ToCArray());
      /* Set image data */
      m_cDisplayedImage = QImage(reinterpret_cast<const UInt8*>(m_cRGBImageData.ToCArray()),
                                 m_nImageWidth, m_nImageHeight,
                                 QImage::Format_RGB888);


      setFixedWidth(m_nImageWidth);
      setFixedHeight(m_nImageHeight);
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::mousePressEvent(QMouseEvent* pc_event) {
       LOG<<""<<pc_event->x()<<","<<pc_event->y()<<"\n";
       LOG<<"Isize "<<size().width()<<","<<size().height()<<"\n";
      SetImageCenterX(pc_event->x());
      SetImageCenterY(pc_event->y());
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::paintEvent(QPaintEvent* pc_event) {
      /* Draw the label normally */
      QLabel::paintEvent(pc_event);
      /*
       * Draw the center
       */
      /* Create a pen */
      QPen cPen(Qt::red);
      cPen.setWidth(3);
      /* Create an image upon which to draw */
      QImage cImageToDrawUpon(m_cDisplayedImage);
      /* Create a painter */
      QPainter cPainter(&cImageToDrawUpon);
      cPainter.setPen(cPen);
      /* Draw the center */
      cPainter.drawPoint(m_nImageCenterX, m_nImageCenterY);
      /* Draw the circle */
      if(m_nImageRadius > 0) {
         cPen.setWidth(2);
         cPainter.setPen(cPen);
         cPainter.drawEllipse(m_nImageCenterX - m_nImageRadius,
                              m_nImageCenterY - m_nImageRadius,
                              m_nImageRadius * 2,
                              m_nImageRadius * 2);
      }
      /* Set the image */
      setPixmap(QPixmap::fromImage(cImageToDrawUpon));
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::SetImageWidth(int n_width) {
      if(m_nImageWidth != n_width) {
         m_nImageWidth = n_width;
         emit ImageWidthChanged(m_nImageWidth);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::SetImageHeight(int n_height) {
      if(m_nImageHeight != n_height) {
         m_nImageHeight = n_height;
         emit ImageHeightChanged(m_nImageHeight);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::SetImageCenterX(int n_center_x) {
      if(m_nImageCenterX != n_center_x) {
         m_nImageCenterX = n_center_x;
         emit ImageCenterXChanged(m_nImageCenterX);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::SetImageCenterY(int n_center_y) {
      if(m_nImageCenterY != n_center_y) {
         m_nImageCenterY = n_center_y;
         emit ImageCenterYChanged(m_nImageCenterY);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberImageWidget::SetImageRadius(int n_radius) {
      if(m_nImageRadius != n_radius) {
         m_nImageRadius = n_radius;
         emit RadiusChanged(m_nImageRadius);
      }
   }

   /****************************************/
   /****************************************/

   CEPuckCameraClientImageGrabberConfigurator::CEPuckCameraClientImageGrabberConfigurator(QWidget* pc_parent) :
      QWidget(pc_parent) {
      /*
       * Create the elements of the widget
       */
      /* Create the image widget */
      m_pcImageDisplay = new CEPuckCameraClientImageGrabberImageWidget();
      /* Create the labels */
      m_pcImageCenterXLabel = new QLabel(tr("Image Center X"));
      m_pcImageCenterYLabel = new QLabel(tr("Image Center Y"));
      m_pcImageRadiusLabel  = new QLabel(tr("Image Radius"));
      m_pcGainLabel         = new QLabel(tr("Gain"));
      m_pcExposureLabel     = new QLabel(tr("Exposure"));
      /* Create the sliders */
      m_pcImageCenterXSlider = new QSlider(Qt::Horizontal);
      m_pcImageCenterYSlider = new QSlider(Qt::Horizontal);
      m_pcImageRadiusSlider  = new QSlider(Qt::Horizontal);
      m_pcGainSlider         = new QSlider(Qt::Horizontal);
      m_pcExposureSlider     = new QSlider(Qt::Horizontal);
      /* Create the spinboxs */
      m_pcImageCenterXSpinBox = new QSpinBox();
      m_pcImageCenterYSpinBox = new QSpinBox();
      m_pcImageRadiusSpinBox  = new QSpinBox();
      m_pcGainSpinBox         = new QSpinBox();
      m_pcExposureSpinBox     = new QSpinBox();
     
      /*
       * Place the elements in the layout
       */              
       
      /* Create a layout for the controls and add the elements */
      QGridLayout* pcControlLayout = new QGridLayout();
      pcControlLayout->addWidget(m_pcImageCenterXLabel,   0, 0);
      pcControlLayout->addWidget(m_pcImageCenterYLabel,   1, 0);
      pcControlLayout->addWidget(m_pcImageRadiusLabel,    2, 0);
      pcControlLayout->addWidget(m_pcGainLabel,           3, 0);
      pcControlLayout->addWidget(m_pcExposureLabel,       4, 0);
      pcControlLayout->addWidget(m_pcImageCenterXSlider,  0, 1);
      pcControlLayout->addWidget(m_pcImageCenterYSlider,  1, 1);
      pcControlLayout->addWidget(m_pcImageRadiusSlider,   2, 1);
      pcControlLayout->addWidget(m_pcGainSlider,          3, 1);
      pcControlLayout->addWidget(m_pcExposureSlider,      4, 1);
      pcControlLayout->addWidget(m_pcImageCenterXSpinBox, 0, 2);
      pcControlLayout->addWidget(m_pcImageCenterYSpinBox, 1, 2);
      pcControlLayout->addWidget(m_pcImageRadiusSpinBox,  2, 2);
      pcControlLayout->addWidget(m_pcGainSpinBox,         3, 2);
      pcControlLayout->addWidget(m_pcExposureSpinBox,     4, 2);
      /* Create a groupbox for the controls */
      QGroupBox* pcConfBox = new QGroupBox(tr("Configuration"));
      pcConfBox->setLayout(pcControlLayout);
      /* Create a layout to place the image on top and the controls underneath */
      QVBoxLayout* pcMainLayout = new QVBoxLayout(this);
      pcMainLayout->addWidget(m_pcImageDisplay);
      pcMainLayout->addWidget(pcConfBox);
      setLayout(pcMainLayout);
      /*
       * Connect the elements
       */
      /* Image center X controls */
      connect(m_pcImageCenterXSlider,  SIGNAL(valueChanged(int)),
              m_pcImageCenterXSpinBox, SLOT(setValue(int)));
      connect(m_pcImageCenterXSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageCenterXSlider,  SLOT(setValue(int)));
      connect(m_pcImageCenterXSlider,  SIGNAL(valueChanged(int)),
              m_pcImageDisplay,        SLOT(SetImageCenterX(int)));
      connect(m_pcImageCenterXSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageDisplay,        SLOT(SetImageCenterX(int)));
      connect(m_pcImageDisplay,        SIGNAL(ImageCenterXChanged(int)),
              m_pcImageCenterXSlider,  SLOT(setValue(int)));
      connect(m_pcImageDisplay,        SIGNAL(ImageCenterXChanged(int)),
              m_pcImageCenterXSpinBox, SLOT(setValue(int)));
      /* Image center Y controls */
      connect(m_pcImageCenterYSlider, SIGNAL(valueChanged(int)),
              m_pcImageCenterYSpinBox, SLOT(setValue(int)));
      connect(m_pcImageCenterYSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageCenterYSlider, SLOT(setValue(int)));
      connect(m_pcImageCenterYSlider,  SIGNAL(valueChanged(int)),
              m_pcImageDisplay,        SLOT(SetImageCenterY(int)));
      connect(m_pcImageCenterYSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageDisplay,        SLOT(SetImageCenterY(int)));
      connect(m_pcImageDisplay,        SIGNAL(ImageCenterYChanged(int)),
              m_pcImageCenterYSlider,  SLOT(setValue(int)));
      connect(m_pcImageDisplay,        SIGNAL(ImageCenterYChanged(int)),
              m_pcImageCenterYSpinBox, SLOT(setValue(int)));
      /* Image radius controls */
      connect(m_pcImageRadiusSlider,  SIGNAL(valueChanged(int)),
              m_pcImageRadiusSpinBox, SLOT(setValue(int)));
      connect(m_pcImageRadiusSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageRadiusSlider,  SLOT(setValue(int)));
      connect(m_pcImageRadiusSlider,  SIGNAL(valueChanged(int)),
              m_pcImageDisplay,       SLOT(SetImageRadius(int)));
      connect(m_pcImageRadiusSpinBox, SIGNAL(valueChanged(int)),
              m_pcImageDisplay,       SLOT(SetImageRadius(int)));
      connect(m_pcImageDisplay,       SIGNAL(ImageRadiusChanged(int)),
              m_pcImageRadiusSlider,  SLOT(setValue(int)));
      connect(m_pcImageDisplay,       SIGNAL(ImageRadiusChanged(int)),
              m_pcImageRadiusSpinBox, SLOT(setValue(int)));
      /* Gain controls */
      connect(m_pcGainSlider, SIGNAL(valueChanged(int)),
              m_pcGainSpinBox, SLOT(setValue(int)));
      connect(m_pcGainSpinBox, SIGNAL(valueChanged(int)),
              m_pcGainSlider, SLOT(setValue(int)));
      /* Exposure controls */
      connect(m_pcExposureSlider, SIGNAL(valueChanged(int)),
              m_pcExposureSpinBox, SLOT(setValue(int)));
      connect(m_pcExposureSpinBox, SIGNAL(valueChanged(int)),
              m_pcExposureSlider, SLOT(setValue(int)));
     
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberConfigurator::SetImageData(CByteArray& c_data) {
      m_pcImageDisplay->SetImage(c_data);
      m_pcImageDisplay->update();
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberConfigurator::SetImageWidth(UInt32 un_width) {
      m_pcImageDisplay->SetImageWidth(un_width);
      m_pcImageCenterXSlider->setMaximum(un_width);
      m_pcImageRadiusSlider->setMaximum(Max<SInt32>(un_width,
                                                    m_pcImageDisplay->GetImageHeight()));
      m_pcImageCenterXSpinBox->setMaximum(un_width);
      m_pcImageRadiusSpinBox->setMaximum(Max<SInt32>(un_width,
                                                     m_pcImageDisplay->GetImageHeight()));
   }

   /****************************************/
   /****************************************/

   void CEPuckCameraClientImageGrabberConfigurator::SetImageHeight(UInt32 un_height) {
      m_pcImageDisplay->SetImageHeight(un_height);
      m_pcImageCenterYSlider->setMaximum(un_height);
      m_pcImageRadiusSlider->setMaximum(Max<SInt32>(m_pcImageDisplay->GetImageWidth(),
                                                    un_height));
      m_pcImageCenterYSpinBox->setMaximum(un_height);
      m_pcImageRadiusSpinBox->setMaximum(Max<SInt32>(m_pcImageDisplay->GetImageWidth(),
                                                     un_height));
   }
}

