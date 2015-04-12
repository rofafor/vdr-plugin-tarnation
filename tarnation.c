/*
 * tarnation.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <vdr/plugin.h>
#include <vdr/dvbdevice.h>

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = trNOOP("Kids say the tarndest things");

class cTarnationDeviceHook : public cDeviceHook {
private:
  enum {
    eMagicMux = 730000000 // in Hz
  };
  bool IsDeviceAvailable() const;

public:
  cTarnationDeviceHook() {}
  ~cTarnationDeviceHook() {}
  virtual bool DeviceProvidesTransponder(const cDevice *deviceP, const cChannel *channelP) const;
};

bool cTarnationDeviceHook::IsDeviceAvailable() const
{
  int receiving = 0;
  for (int i = 0; i < cDevice::NumDevices(); ++i) {
      if (cDevice *d = cDevice::GetDevice(i)) {
         if (d->Receiving())
            receiving |= (1 << i);
         }
      }
  return !receiving;
}

bool cTarnationDeviceHook::DeviceProvidesTransponder(const cDevice *deviceP, const cChannel *channelP) const
{
  bool result = true;
  const cDvbDevice *device = reinterpret_cast<const cDvbDevice*>(deviceP);
  if (device && channelP) {
     int transponder = channelP->Transponder();
     switch (cDvbDeviceProbe::GetSubsystemId(device->Adapter(), device->Frontend())) {
       case 0x0ccd0038: // TerraTec Cinergy T^2
            break;
       default:
            if (ISTRANSPONDER(transponder, eMagicMux) && IsDeviceAvailable())
               result = false;
            break;
       }
     }
  return result;
}

class cPluginTarnation : public cPlugin {
private:
  // Add any member variables or functions you may need here.
public:
  cPluginTarnation(void);
  virtual ~cPluginTarnation();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  virtual const char *MainMenuEntry(void) { return NULL; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPluginTarnation::cPluginTarnation(void)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginTarnation::~cPluginTarnation()
{
  // Clean up after yourself!
}

const char *cPluginTarnation::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginTarnation::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginTarnation::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  new cTarnationDeviceHook;
  return true;
}

bool cPluginTarnation::Start(void)
{
  // Start any background activities the plugin shall perform.
  return true;
}

void cPluginTarnation::Stop(void)
{
  // Stop any background activities the plugin is performing.
}

void cPluginTarnation::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPluginTarnation::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginTarnation::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPluginTarnation::WakeupTime(void)
{
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPluginTarnation::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPluginTarnation::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return NULL;
}

bool cPluginTarnation::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  return false;
}

bool cPluginTarnation::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginTarnation::SVDRPHelpPages(void)
{
  // Return help text for SVDRP commands this plugin implements
  return NULL;
}

cString cPluginTarnation::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  // Process SVDRP commands this plugin implements
  return NULL;
}

VDRPLUGINCREATOR(cPluginTarnation); // Don't touch this!
