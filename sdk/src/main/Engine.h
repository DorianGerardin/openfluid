/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "sdk-core.h"
#include "sdk-base.h"

#include "Module.h"
#include "IOMan.h"
#include "PluginManager.h"



/**
  Computation engine class
*/
class Engine
{
  private:
     Module *mp_HydroModule;
     FunctionsList m_HydroFunctions;

     mhydasdk::core::CoreRepository* mp_CoreData;

     mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

     mhydasdk::base::SimulationStatus* mp_SimStatus;

     PluginManager* mp_PlugMan;

     IOManager* mp_IOMan;

     /**

     */
     EngineConfig m_Config;

     /**
       Processes the config file, check the list of plugins to load,
       loads them, registers params to pass and builds the processing list of each module.
     */
     bool processConfig();

     /**
       plugs the processing list into each module.
     */
     bool plugFunctions();

  public:
    /**
      Constructor
    */
    Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::RuntimeEnvironment* RunEnv,
           PluginManager* PlugMan);

    /**
      Destructor
    */
    ~Engine();

    bool buildModel();

    bool loadData();

    bool prepareDataAndCheckConsistency();

    bool run();

    bool saveResults();

    Module *getHydroModule() { return mp_HydroModule; };

    EngineConfig getConfig() const { return m_Config; };


};



#endif






