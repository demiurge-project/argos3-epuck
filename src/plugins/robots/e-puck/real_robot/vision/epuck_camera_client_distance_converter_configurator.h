#ifndef EPUCK_CAMERA_CLIENT_DISTANCE_CONVERTER_CONFIGURATOR_H
#define EPUCK_CAMERA_CLIENT_DISTANCE_CONVERTER_CONFIGURATOR_H


#include <argos3/core/utility/datatypes/byte_array.h>

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleValidator>

namespace argos {
   class CEPuckCameraClientDistanceConverterConfigurator : public QWidget {

      Q_OBJECT

   public:

      CEPuckCameraClientDistanceConverterConfigurator(QWidget* pc_parent);
      virtual ~CEPuckCameraClientDistanceConverterConfigurator() {}
      
      inline Real GetAlphaVariable() const {
		 return m_pcAlphaVariableValue->text().toFloat();
	  }
	  
	  inline Real GetBetaVariable() const {
		 return m_pcBetaVariableValue->text().toFloat();
	  }
      
      inline Real GetFirstDistanceCm() const {
	     return m_pcFirstDistanceCmInput->text().toFloat();
	  }
	  
	  inline Real GetFirstDistancePixel() const {
	     return m_pcFirstDistancePixelInput->text().toFloat();
	  }
	  
	  inline Real GetSecondDistanceCm() const {
	     return m_pcSecondDistanceCmInput->text().toFloat();
	  }
	  
	  inline Real GetSecondDistancePixel() const {
	     return m_pcSecondDistancePixelInput->text().toFloat();
	  }
	  
	  inline void SetAlphaVariable(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcAlphaVariableValue->setText(s);
	  }
	  
	  inline void SetBetaVariable(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcBetaVariableValue->setText(s);
	  }
	  
	  inline void SetFirstDistanceCm(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcFirstDistanceCmInput->setText(s);
	  }
	  
	  inline void SetFirstDistancePixel(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcFirstDistancePixelInput->setText(s);
	  }
	  
	  inline void SetSecondDistanceCm(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcSecondDistanceCmInput->setText(s);
	  }
	  
	  inline void SetSecondDistancePixel(Real f_value) {
		  QString s = QString::number(f_value);
		  m_pcSecondDistancePixelInput->setText(s);
	  }
	  
   private:
   
      QLabel* 	m_pcFirstDistanceLabel;
      QLabel* 	m_pcSecondDistanceLabel;
      QLabel* 	m_pcFirstCentimetreDistanceLabel;
      QLabel* 	m_pcFirstPixelDistanceLabel;
      QLabel* 	m_pcSecondCentimetreDistanceLabel;
      QLabel* 	m_pcSecondPixelDistanceLabel;

	  QLineEdit* m_pcFirstDistanceCmInput;
	  QLineEdit* m_pcFirstDistancePixelInput;
	  QLineEdit* m_pcSecondDistanceCmInput;
	  QLineEdit* m_pcSecondDistancePixelInput;
	  
	  QPushButton* m_pcComputeButton;
	  QPushButton* m_pcResetButton;
	  
	  QLabel* 	m_pcAlphaVariableLabel;
	  QLabel* 	m_pcBetaVariableLabel;
	  QLineEdit* m_pcAlphaVariableValue;
	  QLineEdit* m_pcBetaVariableValue;
	  
   public slots:
	  /** Compute Alpha and Beta based on content of different fields */
      void ComputeVariables();
      
      /** Reset Alpha and Beta to default values */
      void ResetVariables(); 

   };
}

#endif
