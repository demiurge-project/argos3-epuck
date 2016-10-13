#include "blob_detector.h"
#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <argos3/core/utility/logging/argos_log.h>
namespace argos {

const size_t CBlobDetector::u_color_number = 9;
const CColor CBlobDetector::colors[u_color_number]={CColor::RED,CColor::BLUE,CColor::GREEN,CColor::MAGENTA,
                                     CColor::YELLOW,CColor::CYAN,CColor::ORANGE,CColor::PURPLE,
                                     CColor::BROWN};


/****************************************/
/****************************************/

CBlobDetector::CBitSet::CBitSet(size_t un_size) :
    m_unSize(un_size),
    m_unStorageSize(m_unSize / 8),
    m_punBuffer(NULL) {
    /* If the size is not a multiple of 8, we need one more byte */
    if(m_unSize % 8 != 0) ++m_unStorageSize;
    m_punBuffer = new UInt8[m_unStorageSize];
}

/****************************************/
/****************************************/

CBlobDetector::CBitSet::~CBitSet() {
    delete[] m_punBuffer;
}

/****************************************/
/****************************************/

void CBlobDetector::CBitSet::Reset() {
    ::memset(m_punBuffer, 0, m_unStorageSize * sizeof(UInt8));
}

/****************************************/
/****************************************/

CBlobDetector::SBlob::SBlob() :
    CenterI(0),
    CenterJ(0),
    Area(0) {}

/****************************************/
/****************************************/

CBlobDetector::SDetectionData::SDetectionData(bool c_Enabled, const CColor& c_color,
                                              UInt8 c_hue_min, UInt8 c_hue_max,
                                              const CRange<UInt8>& c_saturation_range,
                                              const CRange<UInt8>& c_value_range,
                                              const CRange<UInt32>& c_area_range,
                                              UInt32 un_image_width,
                                              UInt32 un_image_height,
                                              bool b_create_phase_images) :
    Enabled(c_Enabled),
    Color(c_color),
    HueMin(c_hue_min),HueMax(c_hue_max),
    SaturationRange(c_saturation_range),
    ValueRange(c_value_range),
    AreaRange(c_area_range),
    ImageSize(un_image_width * un_image_height),
    Labels(new UInt16[ImageSize]),
    LabelParentListSize(16),
    LabelParentList(new UInt16[LabelParentListSize]),
    DetectedPixels(NULL) {
    if(b_create_phase_images) {
        DetectedPixels = new CBitSet(ImageSize);
    }
    Reset();
}



/****************************************/
/****************************************/

inline bool CBlobDetector::SDetectionData::isPixelAccepted(SHSV888Pixel& sPixel) {
    if(SaturationRange.WithinMinBoundIncludedMaxBoundIncluded(sPixel.Saturation) &&
            ValueRange.WithinMinBoundIncludedMaxBoundIncluded(sPixel.Value)){

        /* A pixel is accepted when the hue value is in the angle range.
         * a=sPixel.Hue>=HueMin;
         * b=HueMin>HueMax;
         * c=sPixel.Hue<=HueMax;
         *
         * ac \  |00|01|11|10|
         *  b   0| F| F| T| F|
         *      1| F| T| T| T|
         *     -------------
         * This boolean function has been simplified using the Carnout's table above.
         */
        bool a=sPixel.Hue>=HueMin;
        bool b=HueMin>HueMax;
        bool c=sPixel.Hue<=HueMax;

        return (a && (c || b)) || (b && c);

    }
    return false;
}
/****************************************/
/****************************************/

CBlobDetector::SDetectionData::~SDetectionData() {
    delete[] Labels;
    delete[] LabelParentList;
    if(DetectedPixels != NULL) delete DetectedPixels;
}

/****************************************/
/****************************************/

void CBlobDetector::SDetectionData::Reset() {
    if(Enabled){
        ::memset(Labels, 0, ImageSize * sizeof(UInt16));
        ::memset(LabelParentList, 0, LabelParentListSize * sizeof(UInt16));
        LabelCounter = 0;
        LabelBlobs.clear();
        if(DetectedPixels != NULL) DetectedPixels->Reset();
    }
}

/****************************************/
/****************************************/

void CBlobDetector::SDetectionData::NewLabel() {
    /* Increase label counter */
    ++LabelCounter;
    /* Double the size of the label parent list of the current label exceeds its size */
    if(LabelCounter >= LabelParentListSize) {
        UInt16* punNewList = new UInt16[LabelParentListSize * 2];
        ::memcpy(punNewList, LabelParentList, LabelParentListSize * sizeof(UInt16));
        delete[] LabelParentList;
        LabelParentList = punNewList;
        LabelParentListSize *= 2;
    }
    /* Set the parent of this new label as itself */
    LabelParentList[LabelCounter] = LabelCounter;
}

/****************************************/
/****************************************/

CBlobDetector::CBlobDetector(UInt32 un_image_width,
                             UInt32 un_image_height,
                             bool b_create_phase_images) :
    m_unImageWidth(un_image_width),
    m_unImageHeight(un_image_height),
    m_bCreatePhaseImages(b_create_phase_images) {}

/****************************************/
/****************************************/

CBlobDetector::~CBlobDetector() {
    while(! m_vecDetectors.empty()) {
        delete m_vecDetectors.back();
        m_vecDetectors.pop_back();
    }
}

/****************************************/
/****************************************/

void CBlobDetector::Init(TConfigurationNode& t_tree) {

    bool enabled;
    UInt8 hue_max,hue_min,sat_min,sat_max,value_min,value_max;
    UInt32 area_min,area_max;
    std::stringstream colorStr;
    for (size_t i = 0; i < u_color_number; ++i) {
        colorStr.str("");
        colorStr << colors[i];
        TConfigurationNode tGroundNode = GetNode(t_tree, colorStr.str());
        GetNodeAttributeOrDefault(tGroundNode, "enabled", enabled, false);
        GetNodeAttributeOrDefault(tGroundNode, "hue_min",hue_min,50);
        GetNodeAttributeOrDefault(tGroundNode, "hue_max",hue_max,120);
        GetNodeAttributeOrDefault(tGroundNode, "sat_min",sat_min,100);
        GetNodeAttributeOrDefault(tGroundNode, "sat_max",sat_max,200);
        GetNodeAttributeOrDefault(tGroundNode, "value_min",value_min,100);
        GetNodeAttributeOrDefault(tGroundNode, "value_max",value_max,200);
        GetNodeAttributeOrDefault(tGroundNode, "area_min",area_min,(UInt32)0);
        GetNodeAttributeOrDefault(tGroundNode, "area_max",area_max,(UInt32)1000);

        AddDetector(enabled,colors[i],
                    hue_min,
                    hue_max,
                    CRange<UInt8>(sat_min,sat_max),
                    CRange<UInt8>(value_min,value_max),
                    CRange<UInt32>(area_min,area_max));
    }

}

/****************************************/
/****************************************/

void CBlobDetector::SaveXMLConfiguration(const std::string& str_path){
    ticpp::Document tDocument(str_path);
    tDocument.LoadFile();
    ticpp::Element tConf = *tDocument.FirstChildElement();

    std::stringstream colorStr;
    for (size_t i = 0; i < u_color_number; ++i) {
        colorStr.str("");
        colorStr << colors[i];
        ticpp::Element tConfElement(colorStr.str());
        SetNodeAttribute(tConfElement,"enabled",m_vecDetectors[i]->Enabled);
        SetNodeAttribute(tConfElement,"hue_min",m_vecDetectors[i]->HueMin);
        SetNodeAttribute(tConfElement,"hue_max",m_vecDetectors[i]->HueMax);
        SetNodeAttribute(tConfElement,"sat_min",m_vecDetectors[i]->SaturationRange.GetMin());
        SetNodeAttribute(tConfElement,"sat_max",m_vecDetectors[i]->SaturationRange.GetMax());
        SetNodeAttribute(tConfElement,"value_min",m_vecDetectors[i]->ValueRange.GetMin());
        SetNodeAttribute(tConfElement,"value_max",m_vecDetectors[i]->ValueRange.GetMax());
        SetNodeAttribute(tConfElement,"area_min",m_vecDetectors[i]->AreaRange.GetMin());
        SetNodeAttribute(tConfElement,"area_max",m_vecDetectors[i]->AreaRange.GetMax());
        tConf.ReplaceChild(tConf.FirstChildElement(colorStr.str()),tConfElement);
    }
    tDocument.SaveFile(str_path);
}

/****************************************/
/****************************************/

void CBlobDetector::Detect(const CImage<SHSV888Pixel>& c_image) {
    /*
       * Reset color detection data
       */
    for(size_t i = 0; i < m_vecDetectors.size(); ++i) {
        m_vecDetectors[i]->Reset();
    }
    /*
       * First filtering pass
       */
    SHSV888Pixel sPixel;
    size_t unRCIdx;
    UInt16 unNorthLabel, unWestLabel;
    /* For each row */
    for(UInt32 r = 0; r < m_unImageHeight; ++r) {
        /* For each column in row r */
        for(UInt32 c = 0; c < m_unImageWidth; ++c) {
            /* Calculate index corresponding to position (c,r) */
            unRCIdx = r * m_unImageWidth + c;
            /* Get pixel value as HSV */
            sPixel = c_image(c, r);
            /* Apply filters for each enabled color */
            for(size_t i = 0; i < m_vecDetectors.size(); ++i) {

                /* Get reference to current detector */
                SDetectionData& cDetector = *m_vecDetectors[i];
                if(cDetector.Enabled){
                    /* Check whether current pixel passes the color filter */
                    if(cDetector.isPixelAccepted(sPixel)) {
                        /* sPixel passed the color filter */
                        if(m_bCreatePhaseImages) cDetector.DetectedPixels->Set(unRCIdx, true);
                        /*
                   * Look for labels around (c,r)
                   * Since we proceed north->south and west->east it is enough
                   * to check the north and west neighbors
                   */
                        unNorthLabel = 0;
                        unWestLabel = 0;
                        /* Get north label */
                        if(r > 0) unNorthLabel = cDetector.Labels[(r-1) * m_unImageWidth + c];
                        /* Get west label */
                        if(c > 0) unWestLabel = cDetector.Labels[r * m_unImageWidth + (c-1)];
                        /* Depending on these labels, assign a label for (c,r) */
                        if(unNorthLabel == 0 &&
                                unWestLabel == 0) {
                            /* Both labels are zero, create a new label and assign it to (c,r) */
                            cDetector.NewLabel();
                            cDetector.Labels[unRCIdx] = cDetector.LabelCounter;
                        }
                        else if(unNorthLabel != 0 &&
                                unWestLabel != 0) {
                            /*
                      * Both labels are non-zero, assign the smallest to (c,r) and
                      * mark the smallest as parent of the largest
                      */
                            if(unNorthLabel == unWestLabel) {
                                /* Same label, assign it to (c,r) */
                                cDetector.Labels[unRCIdx] = unNorthLabel;
                            }
                            else if(unNorthLabel < unWestLabel) {
                                /* Assign north label to (c,r) */
                                cDetector.Labels[unRCIdx] = unNorthLabel;
                                /* Mark north label as parent of west label */
                                cDetector.LabelParentList[unWestLabel] = unNorthLabel;
                            }
                            else /* unWestLabel < unNorthLabel */ {
                                /* Assign west label to (c,r) */
                                cDetector.Labels[unRCIdx] = unWestLabel;
                                /* Mark west label as parent of north label */
                                cDetector.LabelParentList[unNorthLabel] = unWestLabel;
                            }
                        }
                        else if(unNorthLabel != 0) {
                            /* Only the north label is non-zero, assign it to (c,r) */
                            cDetector.Labels[unRCIdx] = unNorthLabel;
                        }
                        else /* unWestLabel != 0 */ {
                            /* Only the west label is non-zero, assign it to (c,r) */
                            cDetector.Labels[unRCIdx] = unWestLabel;
                        }
                    }
                }
            }
        }
    }
    /*
       * Second filtering pass
       */
    UInt16 unMinLabel;
    /* Apply filters for each enabled color */
    for(size_t i = 0; i < m_vecDetectors.size(); ++i) {
        if(m_vecDetectors[i]->Enabled){
            /* Get reference to current detector */
            SDetectionData& cDetector = *m_vecDetectors[i];
            /* For each row */
            for(UInt32 r = 0; r < m_unImageHeight; ++r) {
                /* For each column in row r */
                for(UInt32 c = 0; c < m_unImageWidth; ++c) {
                    /* Calculate index corresponding to position (c,r) */
                    unRCIdx = r * m_unImageWidth + c;
                    /* Check whether the pixel is labeled */
                    if(cDetector.Labels[unRCIdx] != 0) {
                        /* The pixel is labeled */
                        /* Find the smallest equivalent label */
                        unMinLabel = cDetector.Labels[unRCIdx];
                        while(unMinLabel != cDetector.LabelParentList[unMinLabel]) {
                            unMinLabel = cDetector.LabelParentList[unMinLabel];
                        }
                        /* Update blob data */
                        cDetector.LabelBlobs[unMinLabel].CenterI += c;
                        cDetector.LabelBlobs[unMinLabel].CenterJ += r;
                        ++cDetector.LabelBlobs[unMinLabel].Area;
                    }
                }
            }
            /* Finalize blob data, filtering out small blobs and
          * calculating the center of each remaining blob */
            std::map<UInt16, SBlob>::iterator it = cDetector.LabelBlobs.begin();
            while(it != cDetector.LabelBlobs.end()) {

                if(cDetector.AreaRange.WithinMinBoundIncludedMaxBoundIncluded(it->second.Area)) {
                    it->second.CenterI /= it->second.Area;
                    it->second.CenterJ /= it->second.Area;
                    ++it;
                }
                else {
                    cDetector.LabelBlobs.erase(it++);
                }
            }
        }
    }
}

/****************************************/
/****************************************/

void CBlobDetector::AddDetector(bool c_enabled,const CColor& c_color,
                                const UInt8 c_hue_min,
                                const UInt8 c_hue_max,
                                const CRange<UInt8>& c_saturation_range,
                                const CRange<UInt8>& c_value_range,
                                const CRange<UInt32>& c_area_range) {
    m_vecDetectors.push_back(
                new SDetectionData(c_enabled,c_color,
                                   c_hue_min,
                                   c_hue_max,
                                   c_saturation_range,
                                   c_value_range,
                                   c_area_range,
                                   m_unImageWidth,
                                   m_unImageHeight,
                                   m_bCreatePhaseImages));
}

/****************************************/
/****************************************/

}
