#ifndef CLIENT_SERVER_PROTOCOL
#define CLIENT_SERVER_PROTOCOL

#include <argos3/core/utility/datatypes/datatypes.h>
#include <sys/types.h>

namespace argos {

   enum ECameraConfiguratorCommand {
      COMMAND_GET_ATTRIBUTES     = 103, /* Character g */
      COMMAND_SET_ATTRIBUTES     = 115, /* Character s */
      COMMAND_REFRESH            = 114, /* Character r */
      COMMAND_SAVE_CONFIGURATION = 99   /* Character c */
   };

}

#endif
