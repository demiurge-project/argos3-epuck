/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rab_default_sensor.cpp>
 *
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */
#include <limits>
#include "epuck_range_and_bearing_default_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/plugins/simulator/media/rab_medium.h>

namespace argos {
static const Real EPUCK_RADIUS = 3.5f;
/****************************************/
/****************************************/

CRange<CRadians> INCLINATION_RANGE(CRadians(0), CRadians(ARGOS_PI));

/****************************************/
/****************************************/

CEPuckRABDefaultSensor::CEPuckRABDefaultSensor() :
    m_pcRangeAndBearingEquippedEntity(NULL),
    m_fDistanceNoiseStdDev(0.0f),
    m_pcRNG(NULL),
    m_cSpace(CSimulator::GetInstance().GetSpace()),
    m_bShowRays(false),
    m_bRealRangeNoise(false),
    m_fLossProbability(0.0f),
    m_uMaxNumberOfMessages(std::numeric_limits<UInt32>::max()),
    m_fExpA(9.06422181283387f),
    m_fExpB(-0.00565074879677167f)
{
}

/****************************************/
/****************************************/

void CEPuckRABDefaultSensor::SetRobot(CComposableEntity& c_entity) {
    /* Assign RAB equipped entity to this sensor */
    m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CEpuckRABEquippedEntity>("rab[rab_0]");
    /* Enable the RAB equipped entity */
    m_pcRangeAndBearingEquippedEntity->Enable();
    /* Get reference to controllable entity */
    m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
}

/****************************************/
/****************************************/

void CEPuckRABDefaultSensor::Init(TConfigurationNode& t_tree) {
    try {
        /* Parent class init */
        CCI_EPuckRangeAndBearingSensor::Init(t_tree);
        /* Show rays? */
        GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
        /* Parse noises */
        GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
        GetNodeAttributeOrDefault(t_tree, "real_range_noise", m_bRealRangeNoise, m_bRealRangeNoise);
        GetNodeAttributeOrDefault(t_tree, "max_packets", m_uMaxNumberOfMessages, m_uMaxNumberOfMessages);
        GetNodeAttributeOrDefault(t_tree, "loss_probability", m_fLossProbability, m_fLossProbability);

        m_pcRNG = CRandom::CreateRNG("argos");

        if(m_bRealRangeNoise){
            /* REAL readings
           DISTANCE  mu      sigma
         0       80 6.828551 0.1418425
         1       60 6.828551 0.1418425
         2       45 6.823188 0.1531546
         3       30 6.917447 0.1216533
         4       15 6.984497 0.1092330
         5       10 7.084636 0.1529397
         6        5 7.249550 0.1926492
         7        2 7.596525 0.3192310
         8        0 7.646890 0.3570609
          */
            m_cMuValues.push_back(std::pair<Real, Real > (0, 7.646890));
            m_cMuValues.push_back(std::pair<Real, Real > (2, 7.596525));
            m_cMuValues.push_back(std::pair<Real, Real > (5, 7.249550));
            m_cMuValues.push_back(std::pair<Real, Real > (10, 7.084636));
            m_cMuValues.push_back(std::pair<Real, Real > (15, 6.984497));
            m_cMuValues.push_back(std::pair<Real, Real > (30, 6.917447));
            m_cMuValues.push_back(std::pair<Real, Real > (45, 6.823188));
            m_cMuValues.push_back(std::pair<Real, Real > (60, 6.828551));
            m_cMuValues.push_back(std::pair<Real, Real > (80, 6.828551));
            m_cSigmaValues.push_back(std::pair<Real, Real > (0, 0.3570609));
            m_cSigmaValues.push_back(std::pair<Real, Real > (2, 0.3192310));
            m_cSigmaValues.push_back(std::pair<Real, Real > (5, 0.1926492));
            m_cSigmaValues.push_back(std::pair<Real, Real > (10, 0.1529397));
            m_cSigmaValues.push_back(std::pair<Real, Real > (15, 0.1092330));
            m_cSigmaValues.push_back(std::pair<Real, Real > (30, 0.1216533));
            m_cSigmaValues.push_back(std::pair<Real, Real > (45, 0.1531546));
            m_cSigmaValues.push_back(std::pair<Real, Real > (60, 0.1418425));
            m_cSigmaValues.push_back(std::pair<Real, Real > (80, 0.1418425));
        }


        /* Get RAB medium from id specified in the XML */
        std::string strMedium;
        GetNodeAttribute(t_tree, "medium", strMedium);
        m_pcRangeAndBearingMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strMedium));
        /* Assign RAB entity to the medium */
        m_pcRangeAndBearingMedium->AddEntity(*m_pcRangeAndBearingEquippedEntity);
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing the range and bearing medium sensor", ex);
    }
}

/****************************************/
/****************************************/

void CEPuckRABDefaultSensor::Update() {
    /** TODO: there's a more efficient way to implement this */
    /* Delete old readings */
    ClearPackets();
    /*Variables to calculate the noise*/
    Real fMu;
    Real fSigma;
    Real fPower;

    /* Get list of communicating RABs */
    const CSet<CRABEquippedEntity*>& setRABs = m_pcRangeAndBearingMedium->GetRABsCommunicatingWith(*m_pcRangeAndBearingEquippedEntity);
    /* Buffer for calculating the message distance and angle from the Sender to the Receiver */
    CVector3 cVectorSenderToReceiver;
    CRadians cSenderToReceiverVerticalAngle;

    /* Buffer for calculating the message distance and angle from the Receiver to the Sender*/
    CVector3 cVectorReceiverToSender;
    Real fReceiverToSenderRange;
    CRadians cReceiverToSenderVerticalAngle;
    CRadians cReceiverToSenderAngle;

    /* Pointer for the arriving packet */
    CCI_EPuckRangeAndBearingSensor::SReceivedPacket* psReceivedPacket = NULL;
    /* Pointer to the RAB entity sending the packet */
    CEpuckRABEquippedEntity* pcRABSendingEquipedEntity = NULL;
    /* Go through communicating RABs and create packets */
    size_t unSize=setRABs.size();
    CEpuckRABEquippedEntity* vRABentities[unSize];
    std::vector<size_t> indexes(unSize);
    for (size_t var = 0; var < unSize; ++var) {
        indexes[var]=var;
    }
    /*let's copy the entities in a proper data structure in a random order.*/
    for(CSet<CRABEquippedEntity*>::iterator itRABSendingEquipedEntity = setRABs.begin();
        itRABSendingEquipedEntity != setRABs.end(); ++itRABSendingEquipedEntity) {
        size_t selectedIndex=indexes.size();
	// Changed because of bug on cluster. To be fixed
	while(selectedIndex >= indexes.size()){
		selectedIndex=m_pcRNG->Uniform(CRange<UInt32>(0,indexes.size()));
        }
	vRABentities[indexes.at(selectedIndex)]=static_cast<CEpuckRABEquippedEntity*>(*itRABSendingEquipedEntity);
        indexes.erase(indexes.begin()+selectedIndex);
    }
    UInt32 unMessagesNeeded=m_uMaxNumberOfMessages;
    for (size_t i = 0; i < unSize && unMessagesNeeded>0; ++i) {
        /* Pointer to the RAB entity sending the packet */
        pcRABSendingEquipedEntity = vRABentities[i];

        /* Calculate vector to entity */
        cVectorSenderToReceiver = pcRABSendingEquipedEntity->GetPosition();
        cVectorSenderToReceiver -= m_pcRangeAndBearingEquippedEntity->GetPosition();
        /* If noise was setup, add it */
        if(m_fDistanceNoiseStdDev > 0.0f) {
            cVectorSenderToReceiver += CVector3(
                        m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
                        m_pcRNG->Uniform(INCLINATION_RANGE),
                        m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
        }

        cVectorReceiverToSender=-cVectorSenderToReceiver;

        /*
          * First, we must rotate the cVectorSenderToReceiver (cVectorReceiverToSender) so that
          * it is local to the receiver (sender) robot coordinate system. To do this,
          * it enough to rotate cVectorSenderToReceiver (cVectorReceiverToSender) by the inverse
          * of the receiver (sender) robot orientation.
          */
        cVectorReceiverToSender.Rotate(pcRABSendingEquipedEntity->GetOrientation().Inverse());
        cVectorReceiverToSender.ToSphericalCoords(fReceiverToSenderRange,
                                                  cReceiverToSenderVerticalAngle,
                                                  cReceiverToSenderAngle);
        cVectorSenderToReceiver.Rotate(m_pcRangeAndBearingEquippedEntity->GetOrientation().Inverse());
        /**
         * At this point, we have the angle cReceiverToSenderAngle that is the angle (in the sender's frame of reference)
         * from which the message has been fired.
         * Using cReceiverToSenderAngle we can calculate the emitter used to fire the message
         * in order to place the right payload (if any) in the Data[] vector of the packet.
         */
        CCI_EPuckRangeAndBearingActuator::SEmitter& em=pcRABSendingEquipedEntity->GetEmitterForAngle(cReceiverToSenderAngle);
        if(em.Enabled){
            /*we are about to process a message, so we need one less*/
            --unMessagesNeeded;
            /*we add a packet if a) the loss probability is zero. b) if the random experiment to
            evaluate whether a packet is lost gives false.*/
            if(m_fLossProbability==0.0f || !m_pcRNG->Bernoulli(m_fLossProbability)){
                /* Add ray if requested */
                if(m_bShowRays) {
                    m_pcControllableEntity->AddCheckedRay(false,
                                                          CRay3(pcRABSendingEquipedEntity->GetPosition(),
                                                                m_pcRangeAndBearingEquippedEntity->GetPosition()));
                }
                /* allocating memory for the packet */
                psReceivedPacket = new CCI_EPuckRangeAndBearingSensor::SReceivedPacket();
                cVectorSenderToReceiver.ToSphericalCoords(psReceivedPacket->Range,
                                                          cSenderToReceiverVerticalAngle,
                                                          psReceivedPacket->Bearing);
                /* Convert range to cm */
                psReceivedPacket->Range *= 100.0f;

                psReceivedPacket->Range-=2*EPUCK_RADIUS;
                if(m_bRealRangeNoise){
                    /** Add noise
            * The noise is computed from the noise distribution of the real rab.
            * The noise has a lognorm distribution that is function of the distance.
            * The real robot measures the power of the received signal. This power is
            * converted in distance using the function Max(exp(m_fExpA + m_fExpB * fPower), 0.0);
            * To obtain the measured distance:
            * 1- We calculate the parameters mu and sigma by interpolating
            * the actual distance with the real data stored in m_cMuValues and m_cSigmaValues.
            * 2- We get the simulated value of the power by drawing a number from the lognorm distribution
            * 3- We convert the power in distance
            */
                    fMu = Interpolate(psReceivedPacket->Range, m_cMuValues);       /* Value of the mu parameter of the lognorm distribution*/
                    fSigma = Interpolate(psReceivedPacket->Range, m_cSigmaValues); /* Value of the fsigma parameter of the lognorm distribution*/
                    fPower = m_pcRNG->Lognormal(fSigma, fMu);        /* Value of the power obtained from the lognorm distribution*/
                    psReceivedPacket->Range = Max(exp(m_fExpA + m_fExpB * fPower), 0.0); /* Conversion from power to distance using the conversion fuction*/
                }

                /* Normalize horizontal bearing between [-pi,pi] */
                psReceivedPacket->Bearing.SignedNormalize();
                /* Set message data */
                if (m_uDataSize > 3){
                    psReceivedPacket->Data[3] = em.Data[3];
                }
                if (m_uDataSize > 2){
                    psReceivedPacket->Data[2] = em.Data[2];
                }
                if (m_uDataSize > 1){
                    psReceivedPacket->Data[1] = em.Data[1];
                }
                psReceivedPacket->Data[0] = em.Data[0];
                /* Add message to the list */
                m_tPackets.push_back(psReceivedPacket);
            }
        }
    }
}

/****************************************/
/****************************************/

void CEPuckRABDefaultSensor::Reset() {
    ClearPackets();
}

/****************************************/
/****************************************/

void CEPuckRABDefaultSensor::Destroy() {
    ClearPackets();
    m_pcRangeAndBearingMedium->RemoveEntity(*m_pcRangeAndBearingEquippedEntity);
}

/****************************************/
/****************************************/

REGISTER_SENSOR(CEPuckRABDefaultSensor,
                "epuck_range_and_bearing", "medium",
                "Gianpiero Francesca - [gianpiero.francesca@ulb.ac.be]"
                "Lorenzo Garattoni- [lgaratto@ulb.ac.be]",
                "2.0",
                "The range-and-bearing sensor of the e-puck.",
                "This sensor allows epucks to perform situated communication, i.e., a form of\n"
                "wireless communication whereby the receiver also knows the location of the\n"
                "sender with respect to its own frame of reference.\n"
                "This implementation of the range-and-bearing sensor is associated to the\n"
                "range-and-bearing medium. To be able to use this sensor, you must add a\n"
                "range-and-bearing medium to the <media> section.\n"
                "This sensor allows a epucks to receive messages. To send messages, you need the\n"
                "range-and-bearing actuator (\"epuck_range_and_bearing\" in the actuator section).\n"
                "For use, refer to [ci_epuck_range_and_bearing_sensor.h].\n\n"
                "REQUIRED XML CONFIGURATION\n\n"
                "   <controllers>\n"
                "      ...\n"
                "      <my_controller>\n"
                "         ...\n"
                "         <sensors>\n"
                "            ...\n"
                "            <epuck_range_and_bearing implementation=\"medium\" medium=\"rab\" show_rays=\"false\" data_size=\"2\" noise_std_dev=\"0.0\" real_range_noise=\"false\" max_packets=\"UINT32_MAX\" loss_probability=\"0.0\"/>\n"
                "            ...\n"
                "         <sensors/>\n"
                "         ...\n"
                "      <my_controller/>\n"
                "      ...\n"
                "   <controllers>\n\n"
                "The attribute [medium] refers to the [id] of the [range_and_bearing] medium defined\n"
                "in the [media] xml section.\n"
                "The attribute [data_size] is the number of bytes sent and it is in the range [1,4]."
                "The attribute [noise_std_dev] is the standard deviation of the gaussian noise on both range and bearing."
                "The attribute [real_range_noise] is a flag to activate the noise model on the range generated from real data."
                "The attribute [max_packets] is maximum number of packets that the rab can receive in a control step (in reality you receive up to 5 packets)."
                "The attribute [loss_probability] is the probability to lose a packet. This loss probability is applied on the set of max_packets packets."
                "\n"
                "   <media>\n"
                "      ...\n"
                "      <range_and_bearing id=\"rab\"/>\n"
                "      ...\n"
                "   </media>\n\n"
                "OPTIONAL XML CONFIGURATION\n\n"
                "It is possible to draw the rays shot by the rab sensor in the OpenGL\n"
                "visualization. This can be useful for sensor debugging but also to understand\n"
                "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                "they are not obstructed and in purple when they are. In case a ray is\n"
                "obstructed, a black dot is drawn where the intersection occurred.\n"
                "To turn this functionality on, add the attribute \"show_rays\".\n\n"
                "   <controllers>\n"
                "      ...\n"
                "      <my_controller>\n"
                "         ...\n"
                "         <sensors>\n"
                "            ...\n"
                "            <epuck_range_and_bearing implementation=\"medium\" medium=\"rab\" show_rays=\"true\"/>\n"
                "            ...\n"
                "         <sensors/>\n"
                "         ...\n"
                "      <my_controller/>\n"
                "      ...\n"
                "   <controllers>\n\n"
                , "Usable");
}
