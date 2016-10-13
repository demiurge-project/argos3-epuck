#ifndef EPUCK_CAMERA_CLIENT_IMAGE_GRABBER_CONFIGURATOR_H
#define EPUCK_CAMERA_CLIENT_IMAGE_GRABBER_CONFIGURATOR_H

namespace argos {
   class CTCPSocket;
}

class QSlider;

#include <argos3/core/utility/datatypes/byte_array.h>

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleValidator>

namespace argos {

   /****************************************/
   /****************************************/

   class CEPuckCameraClientImageGrabberImageWidget : public QLabel {

      Q_OBJECT

   public:

      CEPuckCameraClientImageGrabberImageWidget(QWidget* pc_parent = 0);

      virtual ~CEPuckCameraClientImageGrabberImageWidget() {}

      inline int GetImageWidth() const {
         return m_nImageWidth;
      }

      inline int GetImageHeight() const {
         return m_nImageHeight;
      }

      inline int GetImageCenterX() const {
         return m_nImageCenterX;
      }

      inline int GetImageCenterY() const {
         return m_nImageCenterY;
      }

      inline int GetImageRadius() const {
         return m_nImageRadius;
      }

      void SetImage(const CByteArray& c_hsv_data);

   signals:

      void ImageWidthChanged(int);
      void ImageHeightChanged(int);
      void ImageCenterXChanged(int);
      void ImageCenterYChanged(int);
      void RadiusChanged(int);
      void ImageRadiusChanged(int);

   public slots:
         
      void mousePressEvent(QMouseEvent* pc_event);
      void paintEvent(QPaintEvent* pc_event);

      void SetImageWidth(int n_width);
      void SetImageHeight(int n_height);
      void SetImageCenterX(int n_center_x);
      void SetImageCenterY(int n_center_y);
      void SetImageRadius(int n_radius);

   private:

      int        m_nImageWidth;
      int        m_nImageHeight;
      int        m_nImageCenterX;
      int        m_nImageCenterY;
      int        m_nImageRadius;
      CByteArray m_cRGBImageData;
      QImage     m_cDisplayedImage;
   };

   /****************************************/
   /****************************************/

   class CEPuckCameraClientImageGrabberConfigurator : public QWidget {

      Q_OBJECT

   public:

      CEPuckCameraClientImageGrabberConfigurator(QWidget* pc_parent);
      virtual ~CEPuckCameraClientImageGrabberConfigurator() {}

      void SetImageData(CByteArray& c_data);

      inline UInt32 GetImageWidth() const {
         return m_pcImageDisplay->GetImageWidth();
      }

      void SetImageWidth(UInt32 un_width);

      inline UInt32 GetImageHeight() const {
         return m_pcImageDisplay->GetImageHeight();
      }

      void SetImageHeight(UInt32 un_height);

      inline UInt32 GetImageCenterX() const {
         return m_pcImageDisplay->GetImageCenterX();
      }

      inline void SetImageCenterX(UInt32 un_value) const {
         return m_pcImageCenterXSpinBox->setValue(un_value);
      }

      inline UInt32 GetImageCenterY() const {
         return m_pcImageDisplay->GetImageCenterY();
      }

      inline void SetImageCenterY(UInt32 un_value) const {
         return m_pcImageCenterYSpinBox->setValue(un_value);
      }

      inline UInt32 GetImageRadius() const {
         return m_pcImageDisplay->GetImageRadius();
      }

      inline void SetImageRadius(UInt32 un_value) const {
         return m_pcImageRadiusSpinBox->setValue(un_value);
      }

      inline UInt32 GetGain() const {
         return m_pcGainSpinBox->value();
      }

      inline void SetGain(UInt32 un_value) const {
         return m_pcGainSpinBox->setValue(un_value);
      }

      inline UInt32 GetExposure() const {
         return m_pcExposureSpinBox->value();
      }

      inline void SetExposure(UInt32 un_value) const {
         return m_pcExposureSpinBox->setValue(un_value);
      }
      

   private:

      CEPuckCameraClientImageGrabberImageWidget* m_pcImageDisplay;
      QImage*   m_pcImageData;
      QLabel*   m_pcImageCenterXLabel;
      QLabel*   m_pcImageCenterYLabel;
      QLabel*   m_pcImageRadiusLabel;
      QLabel*   m_pcGainLabel;
      QLabel*   m_pcExposureLabel;
      QSlider*  m_pcImageCenterXSlider;
      QSlider*  m_pcImageCenterYSlider;
      QSlider*  m_pcImageRadiusSlider;
      QSlider*  m_pcGainSlider;
      QSlider*  m_pcExposureSlider;
      QSpinBox* m_pcImageCenterXSpinBox;
      QSpinBox* m_pcImageCenterYSpinBox;
      QSpinBox* m_pcImageRadiusSpinBox;
      QSpinBox* m_pcGainSpinBox;
      QSpinBox* m_pcExposureSpinBox;
      
   };

   /****************************************/
   /****************************************/

}

#endif
