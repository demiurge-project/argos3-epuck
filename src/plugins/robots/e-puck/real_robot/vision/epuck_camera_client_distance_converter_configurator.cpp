#include "epuck_camera_client_distance_converter_configurator.h"

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
	
	const Real ALPHA_DEFAULT_VALUE = 0.13228;
	const Real BETA_DEFAULT_VALUE = 1.05702;

   /****************************************/
   /****************************************/

   CEPuckCameraClientDistanceConverterConfigurator::CEPuckCameraClientDistanceConverterConfigurator(QWidget* pc_parent) :
      QWidget(pc_parent) {
	  
	  /* Create labels */
	  m_pcFirstDistanceLabel= new QLabel(tr("First Distance"));
      m_pcSecondDistanceLabel= new QLabel(tr("Second Distance"));
      m_pcFirstCentimetreDistanceLabel= new QLabel(tr("Cm:"));
      m_pcFirstPixelDistanceLabel= new QLabel(tr("Pixel:"));
      m_pcSecondCentimetreDistanceLabel= new QLabel(tr("Cm:"));
      m_pcSecondPixelDistanceLabel= new QLabel(tr("Pixel:"));
      m_pcAlphaVariableLabel = new QLabel(tr("Alpha"));
      m_pcBetaVariableLabel = new QLabel(tr("Beta"));
      
      /* Create the input fields */
      m_pcFirstDistanceCmInput = new QLineEdit();
      m_pcFirstDistancePixelInput = new QLineEdit();
      m_pcSecondDistanceCmInput = new QLineEdit();
      m_pcSecondDistancePixelInput = new QLineEdit();
      m_pcAlphaVariableValue = new QLineEdit();
      m_pcBetaVariableValue = new QLineEdit();
      
      /* Restrict the inputs to integers */
      m_pcFirstDistanceCmInput->setValidator(new QDoubleValidator(0,200,4,this));
      m_pcFirstDistanceCmInput->setMaxLength(10);
      m_pcFirstDistancePixelInput->setValidator(new QDoubleValidator(0,200,4,this));
      m_pcSecondDistanceCmInput->setValidator(new QDoubleValidator(0,200,4,this));
      m_pcSecondDistancePixelInput->setValidator(new QDoubleValidator(0,200,4,this));
      m_pcAlphaVariableValue->setValidator(new QDoubleValidator(0,200,4,this));
      m_pcBetaVariableValue->setValidator(new QDoubleValidator(0,200,4,this));
      /* Create the button */
      m_pcComputeButton = new QPushButton(tr("Compute Alpha and Beta"));
      m_pcResetButton = new QPushButton(tr("Reset to default"));
      
      /*
       * Place the elements in the layout
       */              
      /* Create a layout for the transformation pixel to cm distance and add the elements */
      QGridLayout* pcTransfoLayout = new QGridLayout();
      pcTransfoLayout->addWidget(m_pcFirstDistanceLabel, 				0, 0);
      pcTransfoLayout->addWidget(m_pcFirstCentimetreDistanceLabel, 		1, 1);
      pcTransfoLayout->addWidget(m_pcFirstDistanceCmInput, 				1, 2);
      pcTransfoLayout->addWidget(m_pcFirstPixelDistanceLabel,			1, 3);
      pcTransfoLayout->addWidget(m_pcFirstDistancePixelInput, 			1, 4);
      pcTransfoLayout->addWidget(m_pcSecondDistanceLabel, 				2, 0);
      pcTransfoLayout->addWidget(m_pcSecondCentimetreDistanceLabel, 	3, 1);
      pcTransfoLayout->addWidget(m_pcSecondDistanceCmInput, 			3, 2);
      pcTransfoLayout->addWidget(m_pcSecondPixelDistanceLabel,			3, 3);
      pcTransfoLayout->addWidget(m_pcSecondDistancePixelInput, 			3, 4);
      QGridLayout* pcComputeLayout = new QGridLayout();
      pcComputeLayout->addWidget(m_pcComputeButton, 					0, 1, 1, 5);
      pcComputeLayout->addWidget(m_pcResetButton,                       0, 2, 3, 4);
      pcComputeLayout->addWidget(m_pcAlphaVariableLabel,				1, 1);
      pcComputeLayout->addWidget(m_pcAlphaVariableValue, 				1, 2);
      pcComputeLayout->addWidget(m_pcBetaVariableLabel,					1, 3);
      pcComputeLayout->addWidget(m_pcBetaVariableValue, 				1, 4);
      QGroupBox* pcTransfoBox = new QGroupBox(tr("Distance conversion"));
      pcTransfoBox->setLayout(pcTransfoLayout);
      
      QGroupBox* pcComputeBox = new QGroupBox();
      pcComputeBox->setLayout(pcComputeLayout);
      
      QVBoxLayout* pcMainLayout = new QVBoxLayout(this);
      pcMainLayout->addWidget(pcTransfoBox);
      pcMainLayout->addWidget(pcComputeBox);
      setLayout(pcMainLayout);
       
       
      /* Connect the buttons */
      connect(m_pcComputeButton, SIGNAL(clicked()),
			  this, SLOT(ComputeVariables()));
              
      connect(m_pcResetButton, SIGNAL(clicked()),
              this, SLOT(ResetVariables()));
	}
	
	
    /****************************************/
    /****************************************/

    void CEPuckCameraClientDistanceConverterConfigurator::ComputeVariables() {
       Real PixOne = 0.0f, CmOne = 0.0f, PixTwo = 0.0f, CmTwo = 0.0f;
       PixOne = GetFirstDistancePixel();
       CmOne = GetFirstDistanceCm();
       PixTwo = GetSecondDistancePixel();
       CmTwo = GetSecondDistanceCm();
       if (PixOne != 0.0 && PixTwo != 0.0 && CmOne != 0.0 && CmTwo != 0.0) {
            try {
                Real DiffPix = PixTwo - PixOne;
                Real DivCm = CmTwo / CmOne;
                Real beta = std::pow(DivCm, 1/DiffPix);
                Real alpha = CmOne / std::pow(beta, PixOne);
                SetAlphaVariable(alpha);
                SetBetaVariable(beta);
            } catch (std::exception e) {
                LOGERR << "Error while computing transformation variables" << std::endl;
            }
        }
    }
    
    /****************************************/
    /****************************************/
    
    void CEPuckCameraClientDistanceConverterConfigurator::ResetVariables() {
        SetAlphaVariable(ALPHA_DEFAULT_VALUE);
        SetBetaVariable(BETA_DEFAULT_VALUE);
    }
}
