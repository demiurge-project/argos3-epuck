/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_sensor.h>
 *
 * @brief This file provides the definition of the epuck virtual camrab sensor.
 *
 * This file provides the definition of the epuck virtual camrab sensor.
 * The actuator combines the range and bearing and omnidirectional actuator to enable 
 * better distance estimation.
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#ifndef CCI_EPUCK_VIRTUAL_CAMRAB_SENSOR_H
#define CCI_EPUCK_VIRTUAL_CAMRAB_SENSOR_H

namespace argos {
   class CCI_EPuckVirtualCamrabSensor;
}

#include <list>
#include <algorithm>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>

namespace argos {

class CCI_EPuckVirtualCamrabSensor : public CCI_Sensor {

    static const UInt32 MAX_BYTES_RECEIVED = 4;

    /** Variance of the distance estimation of the omnidirectional camera */
    static const Real VARIANCE_CAMERA;
    
    /** Variance of the distance estimation of the range and bearing */
    static const Real VARIANCE_RAB;

    /** The robot diameter */
    static const Real ROBOT_DIAMETER;
    
    /** Approximation of the robot diameter */
    static const Real VIRTUAL_ROBOT_DIAMETER;
    
    /** the default number of color to light the LEDs */
    static const UInt32 DEFAULT_NUMBER_COLORS;
    
    typedef UInt8 TData[MAX_BYTES_RECEIVED];

public:

    /**
     * Constructor
     */
    CCI_EPuckVirtualCamrabSensor();

    /**
     * Destructor
     */
    virtual ~CCI_EPuckVirtualCamrabSensor() {}
	
    /** Single sensor reading */
    struct SReading
    {
        /** Color of the emitting robot */
        CColor Color;
        
        /** Distance of the reading */
        Real Distance;
        
        /** Angle of the reading */
        Real Angle;
        
        /** Data read by the range and bearing sensor */
        TData Data;
        
        /**
         * Flag used to tell wether the Distance and Angle
         * have been computed from range-and-bearing AND camera
         * information (when true), or not (when false)*/
        bool Fusion;
	
        SReading() : 
            Distance(0.0), 
            Angle(0.0),
            Fusion(false) {
            Data[0] = 0;
            Data[1] = 0;
            Data[2] = 0;
            Data[3] = 0;
        }
		
		/**
		 * Redefine the "=" operator
		 */
        SReading& operator=(const SReading & t_reading) {
            if (&t_reading != this) {
                Color = t_reading.Color;
                Angle = t_reading.Angle;
                Distance = t_reading.Distance;
                Data[0] = t_reading.Data[0];
                Data[1] = t_reading.Data[1];
                Data[2] = t_reading.Data[2];
                Data[3] = t_reading.Data[3];
                Fusion = t_reading.Fusion;
            }
            return *this;
        }
    };
     
    void Init(TConfigurationNode& t_node);
    
    /**
     * Sensor readings
     */
    typedef std::vector<SReading*> TReadings;
    
    inline const TReadings& GetReadings() {
        return m_tReadings;
    }
	
    /**
     * Clears the messages received from sensor.
     * Call this at the end of your time-step to be sure that at each
     * time-step you only have the most recently received packets.
     */
    void ClearReadings();
	
    /* Variables */
protected:
    
    /** Sensor readings. */
    TReadings m_tReadings;
    
    /** Number of different colors used to light the LEDs. */
    UInt32 m_unNumberColors;
    
    /** 
     * Number of bits that the first byte of the range and bearing payload
     * is shifted to remove m_unCurrentColorId from message. Depends on m_uNumberColors.
     */
	UInt8 m_unNumberBitsToShift;
    
    /** Mask used to extract the color identifier from the range and bearing message. */
    UInt8 m_unColorMask;
    
    /** 
     * This flag determines whether the distance of the two sensors have to be merged. 
     * Default value is false.
     */
    bool m_bDistanceFusion;
    
    
    /* Functions */
protected:
    /** 
     * This function takes all the blobs seen by the omnidirectional camera
     * and identify the different robots among all the blobs. The blobs that are considered
     * to belong to a single robot are aggregated in a single blob representing that robot.
     */
     std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> FilterBlobs(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs);

    /** 
     * Create a single blob from a list of blobs. The new angle is the average
     * of all available angles. The new distance is the minimal measured distance.
     */
    CCI_EPuckOmnidirectionalCameraSensor::SBlob* MergeBlobs(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs);

    /** Determines if two blobs correspond to the same robot. */
    bool BelongingToSameRobot(CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_reference_blob, CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_current_blob);

    /** Determines if two blob distances could come from the same robot. */
    bool RelativelyCloseDistance(Real f_dist_1, Real f_dist_2);

    /** 
     * Determines if two blob angles could come from the same robot according to a given criterion.
     * This criterion is the maximum angle that could separate two LEDs given the distances. 
     */
    bool RelativelyCloseAngle(Real f_angle_1, Real f_angle_2, Real f_criterion);

    /** 
     * Returns an estimation of the distance that represents the fusion of the distances 
     * estimated by the omnidirectional camera and the range and bearing sensors, if m_bDistanceFusion is set to true.
     * Then, the fusion is the average over the global variance of both distance estimation sensors.
     * Otherwise the returned distance is the one perceived by the omnidirectional camera sensor.
     */
    Real Fusion(Real f_cam_distance, Real f_RAB_distance);

    /** Transform an UInt8 to the corresponding CColor. */
    CColor IntegerToColor(UInt8 un_color_id);

    /** Transform an CColor to the corresponding UInt8. */
    UInt8 ColorToInteger(CColor c_color);

    /** Checks if two colors identifiers are equivalent. */
    bool SameColor(CColor c_color, UInt8 un_color_id);

    /** 
     * Given a list of blobs and a color identifier, return a list containing only blobs of 
     * the color corresponding to the color identifier.
     */
    std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> GetBlobsWithColor(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, UInt8 un_color);

    /** Transforms an radian angle into a degree angle */
    Real RadiansToDegrees(CRadians angle);
    
    /** Computes the average angle of given blobs. */
    Real AverageAngle(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs);
    
    /* Lua Functions */
    
	#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
	#endif
};
      
}
#endif
