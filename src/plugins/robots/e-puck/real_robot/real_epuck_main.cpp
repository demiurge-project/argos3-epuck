/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_main.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 */

#include "real_epuck.h"

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck.h>
#include <argos3/core/utility/configuration/command_line_arg_parser.h>

using namespace argos;

int main(int argc, char** argv) {
    /* The name of the XML config file */
    std::string strConfigFileName;
    /* The id of the controller to use (as specified in the XML config file) */
    std::string strControllerId;

    try {
        /*
         * Parse command line
         */
        bool bUsageHelp;
        CCommandLineArgParser cCLAP;
        cCLAP.AddFlag('h',
                      "help",
                      "display this usage information",
                      bUsageHelp);
        cCLAP.AddArgument<std::string>('c',
                                       "config-file",
                                       "the experiment XML configuration file [REQUIRED]",
                                       strConfigFileName);
        cCLAP.AddArgument<std::string>('i',
                                       "id-controller",
                                       "the ID of the controller you want to use [REQUIRED]",
                                       strControllerId);
        cCLAP.Parse(argc, argv);

        if(bUsageHelp) {
            /* if -h or --help specified, print help descr and nothing else */
            cCLAP.PrintUsage(LOG);
            LOG.Flush();
            return EXIT_SUCCESS;
        } else {
            /* if no -h or --help specified */
            /* be sure to have config file set */
            if (strConfigFileName.empty()) {
                THROW_ARGOSEXCEPTION("No config file given, see help (-h or --help)");
            }
            /* AND controller id set */
            if (strControllerId.empty()) {
                THROW_ARGOSEXCEPTION("No controller id given, see help (-h or --help)");
            }
        } /* else all is ok, we can continue */
    } catch (CARGoSException& ex) {
        LOGERR << "[FATAL] Error while parsing args"
               << std::endl
               << ex.what()
               << std::endl;
        LOGERR.Flush();
        return EXIT_FAILURE;
    }

    /*
     * TODO: check the size of the base device structs
     */
    LOG << "[INFO] ======== START ========== " << std::endl;
    LOG << "[INFO] SENSOR SIZE "
        << sizeof (BaseSensorState)
        << std::endl;
    LOG << "[INFO] ACTUATOR SIZE "
        << sizeof (BaseActuatorState)
        << std::endl;
    LOG.Flush();

    /*
     * Init the e-puck
     */
    CRealEPuck* pcRealEPuck;
    try{
        pcRealEPuck = &CRealEPuck::GetInstance();
        pcRealEPuck->Init(strConfigFileName, strControllerId);
    } catch (CARGoSException& ex) {
        LOGERR << "[FATAL] Error during initialization"
               << std::endl
               << ex.what()
               << std::endl;
        LOGERR.Flush();
        return EXIT_FAILURE;
    }

    /*
     * Main loop, control step execution, sync
     */
    try {
        /* be sure to be sync on the ticks before begining the steps */
        pcRealEPuck->SyncControlStep();
        while (!pcRealEPuck->IsExperimentFinished() && !pcRealEPuck->bGoHome()) {
           /* Receive raw data from robot sensors */
           pcRealEPuck->ReceiveSensorData();
           /* Perform sensor data post-processing */
           pcRealEPuck->UpdateValues();
           /* Execute control step only if we can at this time */
           pcRealEPuck->GetController().ControlStep();
           /* Synchronize the current step on the ticks from xml config file */
           pcRealEPuck->SyncControlStep();
           /* Send data to robot actuators */
           pcRealEPuck->SendActuatorData();
           /* Flush the logs */
           LOG.Flush();
           LOGERR.Flush();
        }
    } catch(CARGoSException& ex) {
        LOGERR << "[FATAL] Failed during the control steps: "
               << ex.what() << std::endl;
        LOGERR.Flush();
        return EXIT_FAILURE;
    }
  /* Normal ending conditions */
    LOG << "[INFO] Controller terminated" << std::endl;
    LOG.Flush();
    return EXIT_SUCCESS;
}
