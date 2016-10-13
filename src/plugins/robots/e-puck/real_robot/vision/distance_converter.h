#ifndef DISTANCE_CONVERTER_H
#define DISTANCE_CONVERTER_H

#include <math.h>

#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/extended_configurable_resource.h>

namespace argos{
	class CDistanceConverter : public CExtendedConfigurableResource {
		public:
			CDistanceConverter();
			~CDistanceConverter() {}

			void Init(TConfigurationNode& t_tree);
			virtual void Reset() {}
			virtual void Destroy() {}
			
			void SaveXMLConfiguration(const std::string& str_path);
		
			Real PixelToCmDistance(Real pixel_distance);
			
			inline Real GetAlphaVariable() const {
				return m_fAlphaVariable;
			}
			
			inline Real GetBetaVariable() const {
				return m_fBetaVariable;
			}
			
			inline Real GetFirstDistanceCm() const {
				return m_fFirstDistanceCm;
			}
			
			inline Real GetSecondDistanceCm() const {
				return m_fSecondDistanceCm;
			}
			
			inline Real GetFirstDistancePixel() const {
				return m_fFirstDistancePixel;
			}
			
			inline Real GetSecondDistancePixel() const {
				return m_fSecondDistancePixel;
			}
			
			inline void SetAlphaVariable(Real alpha) {
				m_fAlphaVariable = alpha;
			}
			
			inline void SetBetaVariable(Real beta) {
				m_fBetaVariable = beta;
			}
			
			inline void SetFirstDistanceCm(Real cm_distance) {
				m_fFirstDistanceCm = cm_distance;
			}
			
			inline void SetFirstDistancePixel(Real pixel_distance) {
				m_fFirstDistancePixel = pixel_distance;
			}
			
			inline void SetSecondDistanceCm(Real cm_distance) {
				m_fSecondDistanceCm = cm_distance;
			}
			
			inline void SetSecondDistancePixel(Real pixel_distance) {
				m_fSecondDistancePixel = pixel_distance;
			}
			
		private:
			Real m_fAlphaVariable;
			Real m_fBetaVariable;
			
			Real m_fFirstDistanceCm;
			Real m_fSecondDistanceCm;
			
			Real m_fFirstDistancePixel;
			Real m_fSecondDistancePixel;
	};
}


#endif
