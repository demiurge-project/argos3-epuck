/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_sensor.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include "ci_epuck_virtual_camrab_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#endif

namespace argos {

    const Real CCI_EPuckVirtualCamrabSensor::VARIANCE_CAMERA = 1.6;
    const Real CCI_EPuckVirtualCamrabSensor::VARIANCE_RAB = 324.7;

	const Real CCI_EPuckVirtualCamrabSensor::ROBOT_DIAMETER = 7.0;
	const Real CCI_EPuckVirtualCamrabSensor::VIRTUAL_ROBOT_DIAMETER = 10.0;
    
    const UInt32 CCI_EPuckVirtualCamrabSensor::DEFAULT_NUMBER_COLORS = 3;


	/****************************************/
	/****************************************/

	#ifdef ARGOS_WITH_LUA
	   void CCI_EPuckVirtualCamrabSensor::CreateLuaState(lua_State* pt_lua_state) {
		  CLuaUtility::OpenRobotStateTable(pt_lua_state, "virtual_camrab");
		  for(size_t i = 0; i < m_tReadings.size(); ++i) {
			 CLuaUtility::StartTable(pt_lua_state, i+1);
			 CLuaUtility::AddToTable(pt_lua_state, "distance", m_tReadings[i]->Distance);
			 CLuaUtility::AddToTable(pt_lua_state, "angle", m_tReadings[i]->Angle);
			 CLuaUtility::AddToTable(pt_lua_state, "color", m_tReadings[i]->Color);
			 
			 CLuaUtility::StartTable(pt_lua_state, "data");
			 CLuaUtility::AddToTable(pt_lua_state, 1, m_tReadings[i]->Data[0]);
			 CLuaUtility::AddToTable(pt_lua_state, 2, m_tReadings[i]->Data[1]);
			 CLuaUtility::AddToTable(pt_lua_state, 3, m_tReadings[i]->Data[2]);
			 CLuaUtility::AddToTable(pt_lua_state, 4, m_tReadings[i]->Data[3]);

			 CLuaUtility::EndTable(pt_lua_state);
			 CLuaUtility::EndTable(pt_lua_state);
		  }
		  CLuaUtility::CloseRobotStateTable(pt_lua_state);
	   }
	#endif

	/****************************************/
	/****************************************/

	#ifdef ARGOS_WITH_LUA
	   void CCI_EPuckVirtualCamrabSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
		  lua_getfield(pt_lua_state, -1, "range_and_bearing");
		  /* Save the number of elements in the table */
		  size_t unLastMsgNum = lua_rawlen(pt_lua_state, -1);
		  /* Overwrite the table with the new messages */
		  for(size_t i = 0; i < m_tReadings.size(); ++i) {
			 CLuaUtility::StartTable(pt_lua_state, i+1);
			 CLuaUtility::AddToTable(pt_lua_state, "distance", m_tReadings[i]->Distance);
			 CLuaUtility::AddToTable(pt_lua_state, "angle", m_tReadings[i]->Angle);
			 CLuaUtility::AddToTable(pt_lua_state, "color", m_tReadings[i]->Color);
			 
			 CLuaUtility::StartTable(pt_lua_state, "data");
			 CLuaUtility::AddToTable(pt_lua_state, 1, m_tReadings[i]->Data[0]);
			 CLuaUtility::AddToTable(pt_lua_state, 2, m_tReadings[i]->Data[1]);
			 CLuaUtility::AddToTable(pt_lua_state, 3, m_tReadings[i]->Data[2]);
			 CLuaUtility::AddToTable(pt_lua_state, 4, m_tReadings[i]->Data[3]);

			 CLuaUtility::EndTable(pt_lua_state);
			 CLuaUtility::EndTable(pt_lua_state);
		  }
		  /* Are the new messages less than the old ones? */
		  if(m_tReadings.size() < unLastMsgNum) {
			 /* Yes, set to nil all the extra entries */
			 for(size_t i = m_tReadings.size()+1; i <= unLastMsgNum; ++i) {
				lua_pushnumber(pt_lua_state,  i);
				lua_pushnil   (pt_lua_state    );
				lua_settable  (pt_lua_state, -3);
			 }
		  }
		  lua_pop(pt_lua_state, 1);
	   }
	#endif
    
    /****************************************/
	/****************************************/
    
    CCI_EPuckVirtualCamrabSensor::CCI_EPuckVirtualCamrabSensor() {
        m_unNumberColors = CCI_EPuckVirtualCamrabSensor::DEFAULT_NUMBER_COLORS;
    }
	
	/****************************************/
	/****************************************/
    
    void CCI_EPuckVirtualCamrabSensor::Init(TConfigurationNode& t_node) {
		/* get the number of color used to light the LEDs */
        GetNodeAttributeOrDefault(t_node, "number_colors", m_unNumberColors, m_unNumberColors);
        
        if (m_unNumberColors > 9) {
            LOG << "[WARNING] Number of colors too large, back to maximum value = 9" << std::endl;
            m_unNumberColors = 9;
        }
        
        
        /* get the flag used to tell whether the distances have to be merged or not */
        GetNodeAttributeOrDefault(t_node, "dist_fusion", m_bDistanceFusion, true);
        
        /* computes the number of bits needed to encode the colors */
        m_unNumberBitsToShift = std::ceil(log2(m_unNumberColors));
        /* computes the mask to extract color from rab message */ 
        m_unColorMask = std::pow(Real(2), m_unNumberBitsToShift) - 1;

    }
	
    /****************************************/
    /****************************************/
	
    void CCI_EPuckVirtualCamrabSensor::ClearReadings() {
        while(!m_tReadings.empty()) {
            delete m_tReadings.back();
            m_tReadings.pop_back();
        }
    }
	
    /****************************************/
    /****************************************/
  
    bool SortByAngle(CCI_EPuckOmnidirectionalCameraSensor::SBlob* l, CCI_EPuckOmnidirectionalCameraSensor::SBlob* r) {
        return (Real)((l->Angle.UnsignedNormalize().GetValue() * 180) / ARGOS_PI) < (Real)((r->Angle.UnsignedNormalize().GetValue() * 180) / ARGOS_PI); 
    } 

    /****************************************/
    /****************************************/
  
    std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> CCI_EPuckVirtualCamrabSensor::FilterBlobs(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs) {
        // Flag used 
        bool bGoBackward = true;
        
        // Will contain one blob for each robot. Result of this function.
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecFilteredBlobs;
        
        // Will contain blobs of same color.
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecSameColorBlobs;
        
        // Will contain blobs emitted by the same robot
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecRobotBlobs;
        
        CCI_EPuckOmnidirectionalCameraSensor::SBlob* sReferenceBlob;
        CCI_EPuckOmnidirectionalCameraSensor::SBlob* sCurrentBlob;

		for (UInt8 color = 0; color < m_unNumberColors; color++) {
			vecSameColorBlobs = GetBlobsWithColor(vec_blobs, color);
			
			// Sort the blobs according their angles
			std::sort(vecSameColorBlobs.begin(), vecSameColorBlobs.end(), SortByAngle);
			
			// This block will check if the first blob is close to last blob
			// It is necessary to do so because blobs belonging to the same robot
			// could be above and below 0 (or 360) degrees.
			if (!vecSameColorBlobs.empty()) {
				sReferenceBlob = vecSameColorBlobs.front();
				 
				vecRobotBlobs.push_back(sReferenceBlob); 
				while (bGoBackward && (1 < vecSameColorBlobs.size())) {
					sCurrentBlob = vecSameColorBlobs.at(vecSameColorBlobs.size() - 1);
					if (BelongingToSameRobot(sReferenceBlob, sCurrentBlob)){
						// Add sCurrentBlob to the list of blobs for that robot
						vecRobotBlobs.push_back(sCurrentBlob);
						vecSameColorBlobs.erase(vecSameColorBlobs.begin() + (vecSameColorBlobs.size() - 1));
					} else {
						bGoBackward = false;
					}
				}
				vecSameColorBlobs.erase(vecSameColorBlobs.begin());
			}
			
			for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator it = vecSameColorBlobs.begin(); it != vecSameColorBlobs.end(); ++it) {
				if (vecRobotBlobs.empty()) {
					sReferenceBlob = (*it);   
					vecRobotBlobs.push_back(sReferenceBlob); 
				} else {
					sCurrentBlob = (*it);
					// If sCurrentBlob and sReferenceBlob are considered to be emitted by the same robot
					if (BelongingToSameRobot(sReferenceBlob, sCurrentBlob)){
						// Add sCurrentBlob to the list of blobs for that robot
						vecRobotBlobs.push_back(sCurrentBlob);
					}
					// Otherwise, we have found all blobs belonging to the same robot
					else {
						// Merge all blobs found in one and add it to the returned vector.
						vecFilteredBlobs.push_back(MergeBlobs(vecRobotBlobs));

						vecRobotBlobs.clear();

						vecRobotBlobs.push_back(sCurrentBlob);
						sReferenceBlob = sCurrentBlob;
					}
				}
			}
			
			// If the vector of blobs for a single robot contains blobs, merge them into a single one and add it to the result vector.
			if (!vecRobotBlobs.empty()) {
				vecFilteredBlobs.push_back(MergeBlobs(vecRobotBlobs));
				vecRobotBlobs.clear();
			}
		}
        
        return vecFilteredBlobs;
    }

    /***************************************/
    /***************************************/

    CColor CCI_EPuckVirtualCamrabSensor::IntegerToColor(UInt8 un_color_id) {
        CColor cColor;
        if (un_color_id == 0) {
            cColor = CColor::RED;
        } else if (un_color_id == 1) {
            cColor = CColor::BLUE;
        } else if (un_color_id == 2) {
            cColor = CColor::GREEN;
        } else if (un_color_id == 3) {
            cColor = CColor::MAGENTA;
        } else if (un_color_id == 4) {
            cColor = CColor::CYAN;
        } else if (un_color_id == 5) {
            cColor = CColor::YELLOW;
        } else if (un_color_id == 6) {
            cColor = CColor::BROWN;
        } else if (un_color_id == 7) {
            cColor = CColor::ORANGE;
        } else if (un_color_id == 8) {
            cColor = CColor::PURPLE;
        }
        return cColor;
    }

    /***************************************/
    /***************************************/

    UInt8 CCI_EPuckVirtualCamrabSensor::ColorToInteger(CColor c_color) {
        UInt8 unColorId = 0;
        if (c_color == CColor::RED) {
            unColorId = 0;
        } else if (c_color == CColor::BLUE) {
            unColorId = 1;
        } else if (c_color == CColor::GREEN) {
            unColorId = 2;
        } else if (c_color == CColor::MAGENTA) {
            unColorId = 3;
        } else if (c_color == CColor::CYAN) {
            unColorId = 4;
        } else if (c_color == CColor::YELLOW) {
            unColorId = 5;
        } else if (c_color == CColor::BROWN) {
            unColorId = 6;
        } else if (c_color == CColor::ORANGE) {
            unColorId = 7;
        } else if (c_color == CColor::PURPLE) {
            unColorId = 8;
        }
        return unColorId;
    }

    /***************************************/
    /***************************************/

    bool CCI_EPuckVirtualCamrabSensor::SameColor(CColor c_color, UInt8 un_color_id) {
        return ColorToInteger(c_color) == un_color_id;
    }

    /***************************************/
    /***************************************/
   
    std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> CCI_EPuckVirtualCamrabSensor::GetBlobsWithColor(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, UInt8 un_color) {
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecBlobsWithSameColor;
        for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator itBlob = vec_blobs.begin(); itBlob != vec_blobs.end(); ++itBlob) {
            if (SameColor((*itBlob)->Color, un_color)) {
                vecBlobsWithSameColor.push_back(*itBlob);
            }
        }
        return vecBlobsWithSameColor;
    }

    /***************************************/
    /***************************************/

    Real CCI_EPuckVirtualCamrabSensor::RadiansToDegrees(CRadians c_angle) {
        return (c_angle.UnsignedNormalize().GetValue() * 180) / ARGOS_PI;
    } 

    /***************************************/
    /***************************************/
    
    Real CCI_EPuckVirtualCamrabSensor::AverageAngle(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs) {
        Real fResult = 0.0;
        Real fCurrentAngle;
        std::vector<Real> vecAllAngles;
        
        std::sort(vec_blobs.begin(), vec_blobs.end(), SortByAngle);  // Sort the blobs by ascending angles
        
        Real fReferenceAngle = RadiansToDegrees(vec_blobs.at(0)->Angle);
        for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator it = vec_blobs.begin(); it != vec_blobs.end(); ++it) {
            fCurrentAngle = RadiansToDegrees((*it)->Angle);
            if (abs(int(fCurrentAngle - fReferenceAngle)) > 300) {
                if (fCurrentAngle > 300) {
                    vecAllAngles.push_back(fCurrentAngle-360);
                }
                else {
                    vecAllAngles.push_back(fCurrentAngle);
                    vecAllAngles.at(0) = fReferenceAngle-360;
                }
            }
            else {
                vecAllAngles.push_back(fCurrentAngle);
            }
        }

        for (std::vector<Real>::iterator it = vecAllAngles.begin(); it != vecAllAngles.end(); ++it) {
            fResult = fResult + *it;
        }
        
        fResult = fResult/vecAllAngles.size();
        
        return fResult;
    }

    /***************************************/
    /***************************************/
   
    bool CCI_EPuckVirtualCamrabSensor::RelativelyCloseAngle(Real f_angle_1, Real f_angle_2, Real f_criterion) {
        Real fUpper = f_angle_1 + f_criterion;
        Real fLower = f_angle_1 - f_criterion;
        if ((fUpper < 0) || (fUpper > 360) || (fLower < 0) || (fLower > 360)) {
            fLower = fLower + 180;
            fUpper = fUpper + 180;
            f_angle_2 = f_angle_2 + 180;
        }
        if (fLower >= 360)
            fLower = fLower - 360;
        if (fUpper >= 360)
            fUpper = fUpper - 360;
        if (f_angle_2 >= 360)
            f_angle_2 = f_angle_2 - 360;
        return (f_angle_2 <= fUpper) && (f_angle_2 >= fLower);
    }

    /***************************************/
    /***************************************/

    bool CCI_EPuckVirtualCamrabSensor::RelativelyCloseDistance(Real f_dist_1, Real f_dist_2) {
        return (f_dist_2 <= f_dist_1 + VIRTUAL_ROBOT_DIAMETER) && (f_dist_2 >= f_dist_1 - VIRTUAL_ROBOT_DIAMETER);
    }

    /***************************************/
    /***************************************/

    bool CCI_EPuckVirtualCamrabSensor::BelongingToSameRobot(CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_reference_blob, CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_current_blob) {
        Real fMaxAngle = ( atan(ROBOT_DIAMETER/s_reference_blob->Distance) * 180 ) / ARGOS_PI;
        return (RelativelyCloseDistance(s_reference_blob->Distance, s_current_blob->Distance) && RelativelyCloseAngle(RadiansToDegrees(s_reference_blob->Angle), RadiansToDegrees(s_current_blob->Angle), fMaxAngle));
    }

    /***************************************/
    /***************************************/

    CCI_EPuckOmnidirectionalCameraSensor::SBlob* CCI_EPuckVirtualCamrabSensor::MergeBlobs(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_robot_blobs) {
        CColor cRobotColor;
        Real fRobotAngle = 0.0f;
        Real fRobotDistance = 10000.0f;
        UInt32 unRobotArea = 0;
        for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator it = vec_robot_blobs.begin(); it != vec_robot_blobs.end(); ++it) {
            cRobotColor = (*it)->Color;
            fRobotAngle += ((*it)->Angle.UnsignedNormalize().GetValue() * 180) / ARGOS_PI;
            // Take minimal distance
            if ((*it)->Distance < fRobotDistance) {
                fRobotDistance = (*it)->Distance;    
            }
            unRobotArea = (*it)->Area;
        }
        // Take average angle
        CRadians cRobotAngleRad = CRadians( (AverageAngle(vec_robot_blobs) * ARGOS_PI) / 180);
        return new CCI_EPuckOmnidirectionalCameraSensor::SBlob(cRobotColor, cRobotAngleRad, fRobotDistance, unRobotArea);
    }

    /****************************************/
    /****************************************/

    Real CCI_EPuckVirtualCamrabSensor::Fusion(Real f_blob_distance, Real f_RAB_distance) {
		Real res = f_blob_distance - 3.5;
		if (m_bDistanceFusion) {
			Real fDenominator = VARIANCE_RAB + VARIANCE_CAMERA;
			res = (VARIANCE_RAB/fDenominator)*(f_blob_distance - 3.5) + (VARIANCE_CAMERA/fDenominator)*f_RAB_distance;
		}
		return res;
    }

    /****************************************/
    /****************************************/

}
