#ifndef EXTENDED_CONFIGURABLE_RESOURCE
#define EXTENDED_CONFIGURABLE_RESOURCE

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {

   class CExtendedConfigurableResource : public CBaseConfigurableResource {

   public:

      virtual ~CExtendedConfigurableResource() {}

      /**
       * Saves to file the current configuration in XML format.
       * The file is truncated and rewritten.
       * @param str_path The path of the file to write.
       */
      virtual void SaveXMLConfiguration(const std::string& str_path) = 0;

   };

}

#endif
