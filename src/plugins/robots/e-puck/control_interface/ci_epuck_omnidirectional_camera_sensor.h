/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h
 *
 * @brief This file provides the definition of the e-puck
 * omnidirectional camera sensor.
 *
 *  * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
    * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 */
#ifndef CI_EPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H
#define CI_EPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H

namespace argos {
   class CCI_EPuckOmnidirectionalCameraSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/datatypes/color.h>
#include <vector>
namespace argos {

   class CCI_EPuckOmnidirectionalCameraSensor: virtual public CCI_Sensor {

   public:

       /**
        * An SBlob represents a generic colored 2D segment in the image.
        *
        * A blob has a color as defined by the CColor class. The position of the blob is given in
        * polar coordinates. The angle is counted counter-clockwise watching the FootBot from top, as to respect the
        * general frame convention. As for the distances, both the distance in cms from the robot center and in pixels from the
        * optical center in the image are given. The last attribute is the area of the blob in pixels.
        *
        * @see CColor
        */
       struct SBlob {
               /* Color */
               CColor Color;
               /* Angle */
               CRadians Angle;
               /* Distance in cms */
               Real Distance;
               /**
                *
                * @brief Area in pixels
                */
               UInt32 Area;
               /**
                * Constructor
                */
               SBlob() :
                   Color(CColor::BLACK),
       Distance(0.0f) {
               }
               /**
                * Constructor with paramters
                * @param c_color Blob color
                * @param c_angle Blob center
                * @param f_distance Blob distance in cm
                * @param f_raw_distance Blob distance in pixels
                * @param un_area Blob Area in pixel
                */
               SBlob(const CColor& c_color,
           const CRadians& c_angle,
           Real f_distance,UInt32 un_area) :
                   Color(c_color),
       Angle(c_angle),
       Distance(f_distance),
               Area(un_area){
               }

               friend std::ostream& operator<<(std::ostream& c_os, const SBlob& s_blob) {
                   c_os << "(Color = " << s_blob.Color << "," << "Angle = " << ToDegrees(s_blob.Angle) << ","
                           << "Distance = " << s_blob.Distance<< ", Area = " << s_blob.Area << ")";
                   return c_os;
               }
       };

       /**
        * Vector of pointers to colored blobs.
        */
       typedef std::vector<SBlob*> TBlobList;

       /**
        * It represents the readings collected through the camera at a specific time step.
        * It consists of two members, the vector of colored blobs detected and a counter which stores the time step
        * at which the blobs became available.
        */
       struct SReadings {
               TBlobList BlobList;
               UInt64 Counter;

               SReadings() :
                   Counter(0) {
               }

               friend std::ostream& operator<<(std::ostream& c_os, const SReadings& s_reading) {
                   c_os << "Counter: " <<  s_reading.Counter << std::endl;
                   for (size_t i = 0; i < s_reading.BlobList.size(); i++) {
                       c_os << "Blob[" << i << "]: " << s_reading.BlobList[i] << std::endl;
                   }
                   return c_os;
               }
       };

   public:

       /**
        * Constructor
        */
       CCI_EPuckOmnidirectionalCameraSensor():m_bEnabled(true) {
       }

       /**
        * Destructor
        */
       virtual ~CCI_EPuckOmnidirectionalCameraSensor() {
       }

       /**
        * Returns a reference to the current camera readings.
        * @return A reference to the current camera readings.
        */
       inline const SReadings& GetReadings() const {
           return m_sReadings;
       }

       /**
        * Enables image acquisition and processing.
        */
       virtual void Enable(){
           m_bEnabled=true;
       }

       /**
        * Disables image acquisition and processing.
        */
       virtual void Disable(){
           m_bEnabled=false;
       }

#ifdef ARGOS_WITH_LUA
 virtual void CreateLuaState(lua_State* pt_lua_state);

 virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

       bool Enabled() const;


   protected:
       bool m_bEnabled;
       SReadings m_sReadings;

};

}

#endif
