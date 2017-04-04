

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/vector2.h>
#include "epuck_camera_client_blob_detector_configurator.h"
#include "image.h"
#include <QBitmap>
#include <QDebug>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QColorDialog>
#include <QCheckBox>
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDial>
#include <QTextEdit>

#include <math.h>
#include <iostream>
#include <fstream>
namespace argos {

/****************************************/
/****************************************/


MyLabel::MyLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f){}

/****************************************/
/****************************************/

MyLabel::MyLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):QLabel(text,parent,f){}

/****************************************/
/****************************************/

MyLabel::~MyLabel(){}

void MyLabel::mousePressEvent(QMouseEvent *ev){
    emit clicked(ev->pos());
}

/****************************************/
/****************************************/

CEPuckCameraClientBlobDetectorConfiguratorTab::CEPuckCameraClientBlobDetectorConfiguratorTab(QWidget* pc_parent) :
    QWidget(pc_parent),
    m_sEyedropperRange(30){

    /* Checkbox enable color */
    m_pcEnableColorCheckBox = new QCheckBox(tr("Enable color"));

    /* Checkbox real image or pixel filter */
    m_pcRealImageCheckBox = new QCheckBox(tr("Real image"));
    m_bShowRealImage = true;
    m_pcRealImageCheckBox->setChecked(true);

    /* Eye dropper button */
    m_pcEyeDropperButton = new QPushButton("Eye dropper", this);
    /*
       * Pixel filter widgets
       */
    /* Pixel filter */
    m_pcPixelFilterLabel = new MyLabel();
    /* Hue */
    QLabel* pcHueLabelMin = new QLabel(tr("Hue min"));
    m_pcHueRangeMin = new QSpinBox();
    m_pcHueRangeMin->setMaximum(180);
    QDial* m_pcHueRangeMinDial = new QDial();
    m_pcHueRangeMinDial->setMaximum(180);
    m_pcHueRangeMinDial->setNotchesVisible(true);
    m_pcHueRangeMinDial->setWrapping(true);

    QLabel* pcHueLabelMax = new QLabel(tr("Hue max"));
    m_pcHueRangeMax = new QSpinBox();
    m_pcHueRangeMax->setMaximum(180);
    QDial* m_pcHueRangeMaxDial = new QDial();
    m_pcHueRangeMaxDial->setMaximum(180);
    m_pcHueRangeMaxDial->setNotchesVisible(true);
    m_pcHueRangeMaxDial->setWrapping(true);

    QLabel* pcSatLabelMin = new QLabel(tr("Saturation min"));
    m_pcSaturationRangeMin = new QSpinBox();
    m_pcSaturationRangeMin->setMaximum(255);
    QSlider* m_pcSatRangeMinSlider = new QSlider(Qt::Horizontal);
    m_pcSatRangeMinSlider->setMaximum(255);

    QLabel* pcSatLabelMax = new QLabel(tr("Saturation max"));
    m_pcSaturationRangeMax = new QSpinBox();
    m_pcSaturationRangeMax->setMaximum(255);
    QSlider* m_pcSatRangeMaxSlider = new QSlider(Qt::Horizontal);
    m_pcSatRangeMaxSlider->setMaximum(255);

    QLabel* pcValueLabelMin = new QLabel(tr("Value min"));
    m_pcValueRangeMin = new QSpinBox();
    m_pcValueRangeMin->setMaximum(255);
    QSlider* m_pcValRangeMinSlider = new QSlider(Qt::Horizontal);
    m_pcValRangeMinSlider->setMaximum(255);

    QLabel* pcValueLabelMax = new QLabel(tr("Value max"));
    m_pcValueRangeMax = new QSpinBox();
    m_pcValueRangeMax->setMaximum(255);
    QSlider* m_pcValRangeMaxSlider = new QSlider(Qt::Horizontal);
    m_pcValRangeMaxSlider->setMaximum(255);

    /*
       * Blob detector widgets
       */
    /* Blob detector */
    m_pcBlobDetectorLabel = new QLabel();
    /* Area */
    QLabel* pcAreaLabelMin = new QLabel(tr("Area min"));
    m_pcAreaRangeMin = new QSpinBox();
    m_pcAreaRangeMin->setMaximum(100000);
    QSlider* m_pcAreaRangeMinSlider = new QSlider(Qt::Horizontal);
    m_pcAreaRangeMinSlider->setMaximum(100000);
    QLabel* pcAreaLabelMax = new QLabel(tr("Area max"));
    m_pcAreaRangeMax = new QSpinBox();
    m_pcAreaRangeMax->setMaximum(100000);
    QSlider* m_pcAreaRangeMaxSlider = new QSlider(Qt::Horizontal);
    m_pcAreaRangeMaxSlider->setMaximum(100000);
    /*
       * Configuration layouts
       */
    QGridLayout* pcPixelFilterConfLayout = new QGridLayout();
    /*HUE MIN*/
    pcPixelFilterConfLayout->addWidget(pcHueLabelMin        ,0, 0);
    pcPixelFilterConfLayout->addWidget(m_pcHueRangeMinDial,0, 1);
    pcPixelFilterConfLayout->addWidget(m_pcHueRangeMin      ,0, 2);
    /*HUE MAX*/
    pcPixelFilterConfLayout->addWidget(pcHueLabelMax        ,1, 0);
    pcPixelFilterConfLayout->addWidget(m_pcHueRangeMaxDial,1, 1);
    pcPixelFilterConfLayout->addWidget(m_pcHueRangeMax      ,1, 2);
    /*SATURATION MIN*/
    pcPixelFilterConfLayout->addWidget(pcSatLabelMin         ,2, 0);
    pcPixelFilterConfLayout->addWidget(m_pcSatRangeMinSlider ,2, 1);
    pcPixelFilterConfLayout->addWidget(m_pcSaturationRangeMin,2, 2);
    /*SATURATION MAX*/
    pcPixelFilterConfLayout->addWidget(pcSatLabelMax         ,3, 0);
    pcPixelFilterConfLayout->addWidget(m_pcSatRangeMaxSlider ,3, 1);
    pcPixelFilterConfLayout->addWidget(m_pcSaturationRangeMax,3, 2);
    /*VALUE MIN*/
    pcPixelFilterConfLayout->addWidget(pcValueLabelMin       ,4, 0);
    pcPixelFilterConfLayout->addWidget(m_pcValRangeMinSlider ,4, 1);
    pcPixelFilterConfLayout->addWidget(m_pcValueRangeMin     ,4, 2);
    /*VALUE MAX*/
    pcPixelFilterConfLayout->addWidget(pcValueLabelMax       ,5, 0);
    pcPixelFilterConfLayout->addWidget(m_pcValRangeMaxSlider ,5, 1);
    pcPixelFilterConfLayout->addWidget(m_pcValueRangeMax     ,5, 2);


    QGridLayout* pcBlobDetectorConfLayout = new QGridLayout();
    /*AREA MIN*/
    pcBlobDetectorConfLayout->addWidget(pcAreaLabelMin        ,0, 0);
    pcBlobDetectorConfLayout->addWidget(m_pcAreaRangeMinSlider,0, 1);
    pcBlobDetectorConfLayout->addWidget(m_pcAreaRangeMin      ,0, 2);
    /*AREA MAX*/
    pcBlobDetectorConfLayout->addWidget(pcAreaLabelMax        ,1, 0);
    pcBlobDetectorConfLayout->addWidget(m_pcAreaRangeMaxSlider,1, 1);
    pcBlobDetectorConfLayout->addWidget(m_pcAreaRangeMax      ,1, 2);
    /*
       * Groups
       */
    QGroupBox* pcPixelFilterConfBox = new QGroupBox(tr("Configuration - Color"));
    pcPixelFilterConfBox->setLayout(pcPixelFilterConfLayout);
    QGroupBox* pcBlobDetectorConfBox = new QGroupBox(tr("Configuration - Blobs"));
    pcBlobDetectorConfBox->setLayout(pcBlobDetectorConfLayout);
    /*
       * Image + configuration layouts
       */
    QVBoxLayout* pcPixelFilterLayout = new QVBoxLayout();
    pcPixelFilterLayout->addWidget(m_pcEnableColorCheckBox);
    pcPixelFilterLayout->addWidget(m_pcPixelFilterLabel);
    QGroupBox* pcPixelFilterOptions = new QGroupBox(tr("Options"));
    //QHBoxLayout* pcPixelFilterOptionLayout = new QHBoxLayout();
    QGridLayout* pcPixelFilterOptionLayout = new QGridLayout();
    pcPixelFilterOptionLayout->addWidget(m_pcRealImageCheckBox,0,1);
    pcPixelFilterOptionLayout->addWidget(m_pcEyeDropperButton,1,1);
    m_pcEyeDropperRange=new QSpinBox();
    m_pcEyeDropperRange->setMaximum(50);
    m_pcEyeDropperRange->setValue(30);
    m_pcEyeDropperRange->setSingleStep(5);
    m_pcEyeDropperRange->setEnabled(false);
    pcPixelFilterOptionLayout->addWidget(new QLabel(tr("Range")),1,3);
    pcPixelFilterOptionLayout->addWidget(m_pcEyeDropperRange,1,2);

    pcPixelFilterOptions->setLayout(pcPixelFilterOptionLayout);
    pcPixelFilterLayout->addWidget(pcPixelFilterOptions);
    pcPixelFilterLayout->addWidget(pcPixelFilterConfBox);
    QVBoxLayout* pcBlobDetectorLayout = new QVBoxLayout();
    pcBlobDetectorLayout->addWidget(m_pcBlobDetectorLabel);
    pcBlobDetectorLayout->addWidget(pcBlobDetectorConfBox);

    QGroupBox* pcConsoleBox = new QGroupBox(tr("Detected Blobs"));
    QVBoxLayout* pcConsoleBoxLayout = new QVBoxLayout();
    pcConsoleBox->setLayout(pcConsoleBoxLayout);
    console = new QTextEdit();
    pcConsoleBoxLayout->addWidget(console);
    pcBlobDetectorLayout->addWidget(pcConsoleBox);

    /*
       * Main layout
       */
    //QHBoxLayout* pcMainLayout = new QHBoxLayout();
    QVBoxLayout* pcMainLayout = new QVBoxLayout();
    pcMainLayout->addLayout(pcPixelFilterLayout);
    pcMainLayout->addLayout(pcBlobDetectorLayout);
    setLayout(pcMainLayout);



    /*
       * Connect the elements
       */
    /* Eye dropper*/
    m_bEyeDropperActive = false;
    connect(m_pcEyeDropperButton, SIGNAL(released()), this, SLOT(eyeDropperHandler()));
    connect(m_pcPixelFilterLabel,SIGNAL(clicked(QPoint)),this, SLOT(labelClicked(QPoint)));

    /*HUE Min*/
    connect(m_pcHueRangeMinDial, SIGNAL(valueChanged(int)),
            m_pcHueRangeMin, SLOT(setValue(int)));
    connect(m_pcHueRangeMin, SIGNAL(valueChanged(int)),
            m_pcHueRangeMinDial, SLOT(setValue(int)));
    /*HUE Max*/
    connect(m_pcHueRangeMaxDial, SIGNAL(valueChanged(int)),
            m_pcHueRangeMax, SLOT(setValue(int)));
    connect(m_pcHueRangeMax, SIGNAL(valueChanged(int)),
            m_pcHueRangeMaxDial, SLOT(setValue(int)));
    /*Saturation Min*/
    connect(m_pcSatRangeMinSlider, SIGNAL(valueChanged(int)),
            m_pcSaturationRangeMin, SLOT(setValue(int)));
    connect(m_pcSaturationRangeMin, SIGNAL(valueChanged(int)),
            m_pcSatRangeMinSlider, SLOT(setValue(int)));
    /*Saturation Max*/
    connect(m_pcSatRangeMaxSlider, SIGNAL(valueChanged(int)),
            m_pcSaturationRangeMax, SLOT(setValue(int)));
    connect(m_pcSaturationRangeMax, SIGNAL(valueChanged(int)),
            m_pcSatRangeMaxSlider, SLOT(setValue(int)));
    /*Value Min*/
    connect(m_pcValRangeMinSlider, SIGNAL(valueChanged(int)),
            m_pcValueRangeMin, SLOT(setValue(int)));
    connect(m_pcValueRangeMin, SIGNAL(valueChanged(int)),
            m_pcValRangeMinSlider, SLOT(setValue(int)));
    /*Value Max*/
    connect(m_pcValRangeMaxSlider, SIGNAL(valueChanged(int)),
            m_pcValueRangeMax, SLOT(setValue(int)));
    connect(m_pcValueRangeMax, SIGNAL(valueChanged(int)),
            m_pcValRangeMaxSlider, SLOT(setValue(int)));

    /*Area Min*/
    connect(m_pcAreaRangeMinSlider, SIGNAL(valueChanged(int)),
            m_pcAreaRangeMin, SLOT(setValue(int)));
    connect(m_pcAreaRangeMin, SIGNAL(valueChanged(int)),
            m_pcAreaRangeMinSlider, SLOT(setValue(int)));
    /*Area Max*/
    connect(m_pcAreaRangeMaxSlider, SIGNAL(valueChanged(int)),
            m_pcAreaRangeMax, SLOT(setValue(int)));
    connect(m_pcAreaRangeMax, SIGNAL(valueChanged(int)),
            m_pcAreaRangeMaxSlider, SLOT(setValue(int)));

    /*Check min max*/
    m_pcSaturationRangeMin->setKeyboardTracking(false);
    m_pcSaturationRangeMax->setKeyboardTracking(false);

    m_pcHueRangeMin->setKeyboardTracking(false);
    m_pcHueRangeMax->setKeyboardTracking(false);
    m_pcValueRangeMin->setKeyboardTracking(false);
    m_pcValueRangeMax->setKeyboardTracking(false);
    m_pcAreaRangeMin->setKeyboardTracking(false);
    m_pcAreaRangeMax->setKeyboardTracking(false);

    connect(m_pcSatRangeMaxSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMax()));
    connect(m_pcSatRangeMinSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMin()));



    connect(m_pcValRangeMaxSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMax()));
    connect(m_pcValRangeMinSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMin()));
    connect(m_pcAreaRangeMaxSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMax()));
    connect(m_pcAreaRangeMinSlider, SIGNAL(valueChanged(int)),
            this, SLOT(checkSlidersMin()));

    connect(m_pcRealImageCheckBox, SIGNAL(stateChanged (int)),
            this, SLOT(realImageStateHandler(int)));

    connect(m_pcEnableColorCheckBox, SIGNAL(stateChanged (int)),
            this, SLOT(enableColorHandler(int)));

    connect(m_pcEyeDropperRange, SIGNAL(valueChanged (int)),
            this, SLOT(updateEyeDropperRange(int)));


    /*Color enabler*/
    pcPixelFilterConfBox->setEnabled(false);
    pcBlobDetectorConfBox->setEnabled(false);
    pcPixelFilterOptions->setEnabled(false);
    m_pcEnableColorCheckBox->setCheckState(Qt::Unchecked);
    connect(m_pcEnableColorCheckBox, SIGNAL(toggled (bool)),
            pcPixelFilterConfBox, SLOT(setEnabled(bool)));
    connect(m_pcEnableColorCheckBox, SIGNAL(toggled (bool)),
            pcBlobDetectorConfBox, SLOT(setEnabled(bool)));
    connect(m_pcEnableColorCheckBox, SIGNAL(toggled (bool)),
            pcPixelFilterOptions, SLOT(setEnabled(bool)));

    /*Launches the interface*/
    show();

}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::enableColorHandler(int state){
    if(state == Qt::Checked){
        m_bColorEnabled = true;
    } else {
        m_bColorEnabled = false;

    }
}
void CEPuckCameraClientBlobDetectorConfiguratorTab::setPause(bool p){
    if(m_bEyeDropperActive){
    eyeDropperHandler();
    }
        m_pcEyeDropperButton->setEnabled(!p);
}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::realImageStateHandler(int state){
    if(state == Qt::Checked){
        m_bShowRealImage = true;
    } else {
        m_bShowRealImage = false;
    }
}

/****************************************/
/****************************************/
void CEPuckCameraClientBlobDetectorConfiguratorTab::eyeDropperHandler(){
    /* Toggle eye dropper state, handle the checkbox and the cursor states */
    if(m_bEyeDropperActive){
        m_bEyeDropperActive = false;
        m_pcRealImageCheckBox->setEnabled(true);
        m_pcEyeDropperButton->setText("Eye dropper");
        QApplication::restoreOverrideCursor();
    } else {
        m_bEyeDropperActive = true;
        m_pcRealImageCheckBox->setCheckState(Qt::Checked);
        m_pcRealImageCheckBox->setEnabled(false);
        m_pcEyeDropperButton->setText("STOP");
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    }
    //LOG << "FUNGE\n" << std::endl;
}
/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::updateEyeDropperRange(int value){
    m_sEyedropperRange = value;
    m_pcHueRangeMax->setValue((m_cLastPixelColor.hue()/2 + m_sEyedropperRange) % m_pcHueRangeMax->maximum());
    if(m_cLastPixelColor.hue()/2 - m_sEyedropperRange < 0){
        m_pcHueRangeMin->setValue(m_pcHueRangeMin->maximum() + (m_cLastPixelColor.hue()/2 - m_sEyedropperRange));
    } else {
        m_pcHueRangeMin->setValue((m_cLastPixelColor.hue()/2 - m_sEyedropperRange));
    }

    m_pcSaturationRangeMax->setValue(m_cLastPixelColor.saturation() + m_sEyedropperRange);
    m_pcSaturationRangeMin->setValue(m_cLastPixelColor.saturation() - m_sEyedropperRange);
    m_pcValueRangeMax->setValue(m_cLastPixelColor.value() + m_sEyedropperRange);
    m_pcValueRangeMin->setValue(m_cLastPixelColor.value() - m_sEyedropperRange);
}
bool CEPuckCameraClientBlobDetectorConfiguratorTab::ColorEnabled() const
{
    return m_bColorEnabled;
}

void CEPuckCameraClientBlobDetectorConfiguratorTab::setColorEnabled(bool bColorEnabled)
{
    if(bColorEnabled){
        m_pcEnableColorCheckBox->setCheckState(Qt::Checked);
    }else{
        m_pcEnableColorCheckBox->setCheckState(Qt::Unchecked);
    }
    m_bColorEnabled = bColorEnabled;
}


/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::labelClicked(QPoint point) {

    /* Eye dropper active and click on the image. The values of hue, saturation, and value are set in the configuration*/
    if(m_bEyeDropperActive){
        m_cLastPixelColor = m_pcPixelFilterLabel->pixmap()->toImage().pixel(point);
        m_pcHueRangeMax->setValue((m_cLastPixelColor.hue()/2 + m_sEyedropperRange) % m_pcHueRangeMax->maximum());
        if(m_cLastPixelColor.hue()/2 - m_sEyedropperRange < 0){
            m_pcHueRangeMin->setValue(m_pcHueRangeMin->maximum() + (m_cLastPixelColor.hue()/2 - m_sEyedropperRange));
        } else {
            m_pcHueRangeMin->setValue((m_cLastPixelColor.hue()/2 - m_sEyedropperRange));
        }

        m_pcSaturationRangeMax->setValue(m_cLastPixelColor.saturation() + m_sEyedropperRange);
        m_pcSaturationRangeMin->setValue(m_cLastPixelColor.saturation() - m_sEyedropperRange);
        m_pcValueRangeMax->setValue(m_cLastPixelColor.value() + m_sEyedropperRange);
        m_pcValueRangeMin->setValue(m_cLastPixelColor.value() - m_sEyedropperRange);
        m_bEyeDropperActive = false;
        m_pcRealImageCheckBox->setEnabled(true);
        m_pcEyeDropperButton->setText("Eye dropper");
        m_pcRealImageCheckBox->setChecked(false);
        m_pcEyeDropperRange->setEnabled(true);
        QApplication::restoreOverrideCursor();
    }
}

/****************************************/
/****************************************/
void CEPuckCameraClientBlobDetectorConfiguratorTab::checkSlidersMin(){
    if(m_pcSaturationRangeMax->value()<m_pcSaturationRangeMin->value()){
        m_pcSaturationRangeMax->setValue(m_pcSaturationRangeMin->value());
    }
    //    if(m_pcHueRangeMax->value()<m_pcHueRangeMin->value()){
    //        m_pcHueRangeMax->setValue(m_pcHueRangeMin->value());
    //    }
    //
    if(m_pcAreaRangeMax->value()<m_pcAreaRangeMin->value()){
        m_pcAreaRangeMax->setValue(m_pcAreaRangeMin->value());
    }
    if(m_pcValueRangeMax->value()<m_pcValueRangeMin->value()){
        m_pcValueRangeMax->setValue(m_pcValueRangeMin->value());
    }

}

/****************************************/
/****************************************/
void CEPuckCameraClientBlobDetectorConfiguratorTab::checkSlidersMax(){
    if(m_pcSaturationRangeMin->value()>m_pcSaturationRangeMax->value()){
        m_pcSaturationRangeMin->setValue(m_pcSaturationRangeMax->value());
    }
    if(m_pcAreaRangeMin->value()>m_pcAreaRangeMax->value()){
        m_pcAreaRangeMin->setValue(m_pcAreaRangeMax->value());
    }
    if(m_pcValueRangeMin->value()>m_pcValueRangeMax->value()){
        m_pcValueRangeMin->setValue(m_pcValueRangeMax->value());
    }

}


/****************************************/
/****************************************/
CEPuckCameraClientBlobDetectorConfiguratorTab::~CEPuckCameraClientBlobDetectorConfiguratorTab() {
}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::SetRealImageData(CByteArray& c_hsv_data){

    /* Convert image to RGB */
    CByteArray  m_cRGBImageData;
    m_cRGBImageData.Resize(c_hsv_data.Size());
    HSV888toRGB888(m_nImageWidth, m_nImageHeight,
                   m_cRGBImageData.ToCArray(), c_hsv_data.ToCArray());
    /* Set image data */
    QImage m_cDisplayedImage = QImage(reinterpret_cast<const UInt8*>(m_cRGBImageData.ToCArray()),
                                      m_nImageWidth, m_nImageHeight,
                                      QImage::Format_RGB888);
    m_pcPixelFilterLabel->setPixmap(QPixmap::fromImage(m_cDisplayedImage));

}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::SetImagesData(CByteArray& c_real_image,const UInt8* un_color_filter_image_data,size_t un_color_filter_image_size) {
    if(m_bShowRealImage){
        SetRealImageData(c_real_image);
    }else{
        SetPixelFilterData(un_color_filter_image_data,un_color_filter_image_size);
    }

    if(!m_bColorEnabled){
        /* Create a pen */
        QPen cPen(Qt::red);
        cPen.setWidth(1);
        /* Create a painter */
        if(m_pcPixelFilterLabel->pixmap()->size().width()==0){
            return;
        }
        QPainter cPainter(const_cast<QPixmap*>(m_pcPixelFilterLabel->pixmap()));
        cPainter.setPen(cPen);
        QFont font = cPainter.font();

        /* twice the size than the current font size */
        font.setPointSize(font.pointSize() * 3);
        cPainter.setFont(font);

        /* Draw the blob */
        cPainter.drawText (0,0,m_nImageWidth,m_nImageHeight,Qt::AlignVCenter|Qt::AlignHCenter, tr("I'm Disabled"));
    }
}
void CEPuckCameraClientBlobDetectorConfiguratorTab::SetPixelFilterData(const UInt8* un_data,
                                                                       size_t un_size) {
    if(m_bShowRealImage){
        return;
    }
    QImage cBitmap(un_data,
                   m_nImageWidth, m_nImageHeight,
                   un_size / m_nImageHeight,
                   QImage::Format_MonoLSB);
    m_pcPixelFilterLabel->setPixmap(
                QBitmap::fromData(QSize(m_nImageWidth,
                                        m_nImageHeight),
                                  un_data));
    QImage cBitMap(m_nImageWidth, m_nImageHeight, QImage::Format_RGB32);
    cBitMap.fill(0x00ff0000);
    m_pcPixelFilterLabel->setPixmap(QPixmap::fromImage(cBitmap));
    m_pcPixelFilterLabel->update();
}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::AddBlobData(UInt32 un_center_i,
                                                                UInt32 un_center_j,
                                                                UInt32 un_area) {

    /*we do not draw blobs when the eyedropper is active*/
    if(m_bEyeDropperActive){
        return;
    }

    /* Create a pen */
    QPen cPen(Qt::red);
    cPen.setWidth(1);

    /* Create a painter */


    if(m_pcPixelFilterLabel->pixmap()->size().width()==0){
        return;
    }
    QPainter cPainter(const_cast<QPixmap*>(m_pcPixelFilterLabel->pixmap()));
    cPainter.setPen(cPen);
    /* Draw the blob */
    Real m_nImageRadius=sqrt(un_area/3.14f);

    QBrush brush(QColor(0, 255, 0, 128));
    cPainter.setBrush(brush);
    cPainter.drawEllipse(un_center_i - m_nImageRadius,
                         un_center_j - m_nImageRadius,
                         m_nImageRadius * 2,
                         m_nImageRadius * 2);

    /* Computation of blob pixel distance */
    CVector2 cImagePointWRTCenter((Real)m_unImageCenterX - (Real)un_center_i,
                                  (Real)m_unImageCenterY - (Real)un_center_j);
    Real pixel_distance = cImagePointWRTCenter.Length();

    QString msg=tr("");
    msg=msg+QString::number(blob_index++)+tr("- (")+QString::number(un_center_i)+tr(",")+QString::number(un_center_j)+tr(") -> ")+QString::number(un_area)
		+tr(" @ ")+QString::number(pixel_distance);
    console->append(msg);
    console->scrollToAnchor(tr("0-"));
    //   cPainter.drawPoint(un_center_i, un_center_j);
    //  m_pcPixelFilterLabel;
    /* Set the image */
    //  m_pcPixelFilterLabel->setPixmap(*m_pcPixmap);
}

/****************************************/
/****************************************/

void CEPuckCameraClientBlobDetectorConfiguratorTab::ClearBlobData() {
    console->clear();
    blob_index=0;
}

/****************************************/
/****************************************/

CEPuckCameraClientBlobDetectorConfigurator::CEPuckCameraClientBlobDetectorConfigurator(QWidget* pc_parent) :
    QTabWidget(pc_parent) {
}

/****************************************/
/****************************************/

}
