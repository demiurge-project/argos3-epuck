#ifndef EPUCK_CAMERA_CLIENT_BLOB_DETECTOR_CONFIGURATOR
#define EPUCK_CAMERA_CLIENT_BLOB_DETECTOR_CONFIGURATOR

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/datatypes/byte_array.h>

#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <cstdio>


namespace argos {
class MyLabel;

   class CEPuckCameraClientBlobDetectorConfiguratorTab : public QWidget {

      Q_OBJECT

   public:

      CEPuckCameraClientBlobDetectorConfiguratorTab(QWidget* pc_parent = 0);
      virtual ~CEPuckCameraClientBlobDetectorConfiguratorTab();

      void SetPixelFilterData(const UInt8* un_data,
                              size_t un_size);
      void SetImagesData(CByteArray& c_real_image,const UInt8* un_color_filter_image_data,size_t un_color_filter_image_size);
      void AddBlobData(UInt32 un_center_i,
                       UInt32 un_center_j,
                       UInt32 un_area);

      void ClearBlobData();

      inline int GetImageWidth() const {
         return m_nImageWidth;
      }

      inline int GetImageHeight() const {
         return m_nImageHeight;
      }

      inline const CColor& GetDetectorColor() const {
         return m_cDetectorColor;
      }

      inline UInt8 GetHueRangeMin() const {
         return m_pcHueRangeMin->value();
      }

      inline UInt8 GetHueRangeMax() const {
         return m_pcHueRangeMax->value();
      }

      inline UInt8 GetSaturationRangeMin() const {
         return m_pcSaturationRangeMin->value();
      }

      inline UInt8 GetSaturationRangeMax() const {
         return m_pcSaturationRangeMax->value();
      }

      inline UInt8 GetValueRangeMin() const {
         return m_pcValueRangeMin->value();
      }

      inline UInt8 GetValueRangeMax() const {
         return m_pcValueRangeMax->value();
      }

      inline UInt32 GetAreaRangeMin() const {
         return m_pcAreaRangeMin->value();
      }

      inline UInt32 GetAreaRangeMax() const {
         return m_pcAreaRangeMax->value();
      }

      inline void SetImageWidth(int n_width) {
         m_nImageWidth = n_width;
      }

      inline void SetImageHeight(int n_height) {
         m_nImageHeight = n_height;
      }

      inline void SetDetectorColor(const CColor& c_color) {
         m_cDetectorColor = c_color;
         fprintf(stderr, "SetDetectorColor(%d, %d, %d)\n",
                 c_color.GetRed(),
                 c_color.GetGreen(),
                 c_color.GetBlue());
      }

      inline void SetHueRangeMin(UInt8 un_min) {
         m_pcHueRangeMin->setValue(un_min);
      }

      inline void SetHueRangeMax(UInt8 un_max) {
         m_pcHueRangeMax->setValue(un_max);
      }

      inline void SetSaturationRangeMin(UInt8 un_min) {
         m_pcSaturationRangeMin->setValue(un_min);
      }

      inline void SetSaturationRangeMax(UInt8 un_max) {
         m_pcSaturationRangeMax->setValue(un_max);
      }

      inline void SetValueRangeMin(UInt8 un_min) {
         m_pcValueRangeMin->setValue(un_min);
      }

      inline void SetValueRangeMax(UInt8 un_max) {
         m_pcValueRangeMax->setValue(un_max);
      }

      inline void SetAreaRangeMin(UInt32 un_min) {
         m_pcAreaRangeMin->setValue(un_min);
      }

      inline void SetAreaRangeMax(UInt32 un_max) {
         m_pcAreaRangeMax->setValue(un_max);
      }


      inline void SetImageCenterX(int un_center_x) {
         m_unImageCenterX = un_center_x;
      }

      inline void SetImageCenterY(int un_center_y) {
         m_unImageCenterY = un_center_y;
      }

    void SetRealImageData(CByteArray& c_data);

    bool ColorEnabled() const;
    void setColorEnabled(bool ColorEnabled);
    void setPause(bool t);
   private slots:

    void checkSlidersMax();
    void checkSlidersMin();
    void eyeDropperHandler();
    void labelClicked(QPoint point);
    void realImageStateHandler(int state);
    void enableColorHandler(int state);
    void updateEyeDropperRange(int value);

   private:
      int blob_index;
      int m_nImageWidth;
      int m_nImageHeight;

      int m_unImageCenterX;
      int m_unImageCenterY;

      bool m_bEyeDropperActive;
      bool m_bShowRealImage;
      bool m_bColorEnabled;
      int m_sEyedropperRange;

      CColor m_cDetectorColor;
      QColor m_cLastPixelColor;

      QCheckBox* m_pcEnableColorCheckBox;

      QCheckBox* m_pcRealImageCheckBox;
      QPushButton* m_pcEyeDropperButton;

      MyLabel* m_pcPixelFilterLabel;
      QLabel* m_pcBlobDetectorLabel;

      QSpinBox* m_pcHueRangeMin;
      QSpinBox* m_pcHueRangeMax;

      QSpinBox* m_pcSaturationRangeMin;
      QSpinBox* m_pcSaturationRangeMax;

      QSpinBox* m_pcValueRangeMin;
      QSpinBox* m_pcValueRangeMax;

      QSpinBox* m_pcAreaRangeMin;
      QSpinBox* m_pcAreaRangeMax;

      QSpinBox* m_pcEyeDropperRange;
      QPixmap* m_pcPixmap;

      QTextEdit* console;
   };

   class MyLabel : public QLabel {
       Q_OBJECT

   public:
       MyLabel(QWidget *parent = 0, Qt::WindowFlags f=0);
       MyLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
       ~MyLabel();

   protected:
       void mousePressEvent(QMouseEvent *ev);

   signals:
       void clicked(QPoint pos);
   };

   class CEPuckCameraClientBlobDetectorConfigurator : public QTabWidget {

      Q_OBJECT

   public:
      void EnablePause(bool pause){
          for (int i = 0; i < this->count(); ++i) {
              GetDetector(i)->setPause(pause);
          }
      }

      CEPuckCameraClientBlobDetectorConfigurator(QWidget* pc_parent = 0);
      virtual ~CEPuckCameraClientBlobDetectorConfigurator() {}

      inline CEPuckCameraClientBlobDetectorConfiguratorTab* GetDetector(size_t un_detector) {
         return dynamic_cast<CEPuckCameraClientBlobDetectorConfiguratorTab*>(widget(un_detector));
      }

      inline void AddDetector(CEPuckCameraClientBlobDetectorConfiguratorTab* pc_detector) {
         addTab(pc_detector, QString::fromStdString(ToString(pc_detector->GetDetectorColor())));
         update();
      }

   };

}

#endif
