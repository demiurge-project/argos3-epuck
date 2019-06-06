#include "distance_converter.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

/****************************************/
/****************************************/

CDistanceConverter::CDistanceConverter() :
//	m_fAlphaVariable(0.13228),
//	m_fBetaVariable(1.05702),
	m_fAlphaVariable(1.25455),
	m_fBetaVariable(1.03848),
	m_fFirstDistanceCm(0.0),
	m_fSecondDistanceCm(0.0),
	m_fFirstDistancePixel(0.0),
	m_fSecondDistancePixel(0.0) {
}

/****************************************/
/****************************************/

void CDistanceConverter::Init(TConfigurationNode& t_tree) {
	TConfigurationNode tGroundNode;
	try{
		tGroundNode = GetNode(t_tree, "pixel_to_cm_conversion");
	} catch(CARGoSException& ex) {
        LOG << "Node pixel_to_cm_conversion not found, using default values" << std::endl;
        return;
    }
	
    GetNodeAttributeOrDefault(tGroundNode, "alpha", m_fAlphaVariable, m_fAlphaVariable);
    GetNodeAttributeOrDefault(tGroundNode, "beta", m_fBetaVariable, m_fBetaVariable);
    GetNodeAttributeOrDefault(tGroundNode, "cm_one", m_fFirstDistanceCm, m_fFirstDistanceCm);
    GetNodeAttributeOrDefault(tGroundNode, "pixel_one", m_fFirstDistancePixel, m_fFirstDistancePixel);
    GetNodeAttributeOrDefault(tGroundNode, "cm_two", m_fSecondDistanceCm, m_fSecondDistanceCm);
    GetNodeAttributeOrDefault(tGroundNode, "pixel_two", m_fSecondDistancePixel, m_fSecondDistancePixel);
}

/****************************************/
/****************************************/

void CDistanceConverter::SaveXMLConfiguration(const std::string& str_path) {
    /*
     * Load and parse the XML configuration file
     */
    ticpp::Document tDocument(str_path);
    tDocument.LoadFile();
    ticpp::Element tConf = *tDocument.FirstChildElement();
    ticpp::Element tConfElement("pixel_to_cm_conversion");
    SetNodeAttribute(tConfElement,"alpha",m_fAlphaVariable);
	SetNodeAttribute(tConfElement,"beta",m_fBetaVariable);
    SetNodeAttribute(tConfElement,"cm_one",m_fFirstDistanceCm);
    SetNodeAttribute(tConfElement,"pixel_one",m_fFirstDistancePixel);
    SetNodeAttribute(tConfElement,"cm_two",m_fSecondDistanceCm);
    SetNodeAttribute(tConfElement,"pixel_two",m_fSecondDistancePixel);
    try{
		tConf.ReplaceChild(tConf.FirstChildElement("pixel_to_cm_conversion"),tConfElement);
	} catch(ticpp::Exception& ex) {
        LOG << "Node pixel_to_cm_conversion not found, provide an xml with this node in order to save the pixel_to_cm calibration. Default values will be used with the current configuration file" << std::endl;
        return;
    }
    tDocument.SaveFile(str_path);
}

/****************************************/
/****************************************/

Real CDistanceConverter::PixelToCmDistance(Real pixel_distance){
	return Real(m_fAlphaVariable * pow(m_fBetaVariable, pixel_distance));
	//1.25455 e^(0.0377548 x) for TAM
	
	
}

}
