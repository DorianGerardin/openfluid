/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file SimulatorSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#ifndef __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__
#define __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__


#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/ware/WareSignature.hpp>


namespace openfluid { namespace ware {


typedef std::string SimMethod_t; // TOIMPL to remove, replaced by tags / #framework (2.2.0)

typedef std::string SimProcess_t; // TOIMPL to remove, replaced by tags / #framework (2.2.0)

typedef std::string SimDomain_t; // TOIMPL to remove, replaced by tags / #framework (2.2.0)


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureDataItem
{
  public:

    std::string Name;
    std::string Description;
    std::string SIUnit;
    openfluid::core::Value::Type DataType = openfluid::core::Value::NONE;

    SignatureDataItem()
    { }

    SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI);

    SignatureDataItem(const std::string& N, const std::string& D, const std::string& SI,
                      openfluid::core::Value::Type T);
};


// =====================================================================
// =====================================================================


/**
Class for storage of the definition of data handled by the simulator.
*/
class OPENFLUID_API SignatureSpatialDataItem : public SignatureDataItem
{
  public:

    openfluid::core::UnitsClass_t UnitsClass;

    SignatureSpatialDataItem() : SignatureDataItem()
    { }

    SignatureSpatialDataItem(const std::string& N, const openfluid::core::UnitsClass_t& U,
                             const std::string& D, const std::string& SI) :
      SignatureDataItem(N,D,SI),UnitsClass(U)
    { }

    SignatureSpatialDataItem(const std::string& N, const openfluid::core::UnitsClass_t& U,
                             const std::string& D, const std::string& SI, openfluid::core::Value::Type T) :
      SignatureDataItem(N,D,SI,T),UnitsClass(U)
    { }
};


// =====================================================================
// =====================================================================


/**
  Class for storage of the definition of the data handled by the simulator. This is part of the signature.
*/
class OPENFLUID_API SignatureHandledData
{
  public:

    // TODO use the same naming style for all members

    std::vector<SignatureDataItem> UsedParams;

    std::vector<SignatureDataItem> RequiredParams;

    std::vector<SignatureSpatialDataItem> ProducedVars;

    std::vector<SignatureSpatialDataItem> UpdatedVars;

    std::vector<SignatureSpatialDataItem> RequiredVars;

    std::vector<SignatureSpatialDataItem> UsedVars;

    std::vector<SignatureSpatialDataItem> ProducedAttribute; // TOIMPL add plural / #feature (2.2.x)

    std::vector<SignatureSpatialDataItem> RequiredAttribute; // TOIMPL add plural / #feature (2.2.x)

    std::vector<SignatureSpatialDataItem> UsedAttribute; // TOIMPL add plural / #feature (2.2.x)

    std::vector<std::string> RequiredExtraFiles; // TOIMPL add description associated with each file? / #feature (2.2.x)

    std::vector<std::string> UsedExtraFiles; // TOIMPL add description associated with each file? / #feature (2.2.x)

    std::vector<openfluid::core::UnitsClass_t> UsedEventsOnUnits; // TOIMPL add description to units class events?
                                                                  // #feature (2.2.x)


    SignatureHandledData()
    {
      clear();
    }


    void clear()
    {
      UsedParams.clear();
      RequiredParams.clear();
      ProducedVars.clear();
      UpdatedVars.clear();
      RequiredVars.clear();
      UsedVars.clear();
      ProducedAttribute.clear();
      RequiredAttribute.clear();
      UsedAttribute.clear();
      RequiredExtraFiles.clear();
      UsedExtraFiles.clear();
      UsedEventsOnUnits.clear();
    }

};


// =====================================================================
// =====================================================================


/**
  Class for storage of the definition of spatial units handled by the simulator.
*/
class OPENFLUID_API SignatureUnitsClassItem
{
  public:

    openfluid::core::UnitsClass_t UnitsClass;

    std::string Description;

    SignatureUnitsClassItem() :
      UnitsClass(""),Description("") 
    { }

    SignatureUnitsClassItem(const openfluid::core::UnitsClass_t& UClass,const std::string& DDescription) :
      UnitsClass(UClass),Description(DDescription) 
    { }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API SignatureUnitsGraph
{
  public:

    std::string UpdatedUnitsGraph;

    std::vector<SignatureUnitsClassItem> UpdatedUnitsClass; // TOIMPL add plural / #feature (2.2.x)


    SignatureUnitsGraph()
    {
      clear();
    }


    void clear()
    {
      UpdatedUnitsGraph.clear();
      UpdatedUnitsClass.clear();
    }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API SignatureTimeScheduling
{
  public:

    enum class SchedulingType { UNDEFINED, DEFAULT, FIXED, RANGE };

    SchedulingType Type;

    openfluid::core::Duration_t Min;

    openfluid::core::Duration_t Max;


    SignatureTimeScheduling()
    {
      setAsUndefined();
    }

    void setAsUndefined()
    {
      Type = SchedulingType::UNDEFINED;
      Min = 0;
      Max = 0;
    }

    void setAsDefaultDeltaT()
    {
      Type = SchedulingType::DEFAULT;
      Min = 0;
      Max = 0;
    }

    void setAsFixed(openfluid::core::Duration_t Val)
    {
      Type = SchedulingType::FIXED;
      Min = Val;
      Max = Val;
    }

    void setAsRange(openfluid::core::Duration_t MinVal, openfluid::core::Duration_t MaxVal)
    {
      Type = SchedulingType::RANGE;
      Min = MinVal;
      Max = MaxVal;
    }

    std::string getTypeAsString() const
    {
      if (Type == SchedulingType::DEFAULT)
      {
        return "default";
      }
      else if (Type == SchedulingType::FIXED)
      {
        return "fixed";
      }
      else if (Type == SchedulingType::RANGE)
      {
        return "range";
      }
      return "undefined";
    }

    void setTypeFromString(const std::string& ST)
    {
      Type = SchedulingType::UNDEFINED;

      if (ST == "default")
      {
        Type = SchedulingType::DEFAULT;
      }
      else if (ST == "fixed")
      {
        Type = SchedulingType::FIXED;
      }
      else if (ST == "range")
      {
        Type = SchedulingType::RANGE;
      }
    }
};


// =====================================================================
// =====================================================================


/**
  Class encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
class OPENFLUID_API SimulatorSignature : public WareSignature
{

  public:

    virtual WareType getType() const
    {
      return WareType::SIMULATOR;
    }

    /**
      Plugin domain (i.e. hydrology, pop, erosion, ...)
    */
    SimDomain_t Domain; // TOIMPL to remove, replaced by tags / #framework (2.2.0)

    /**
      Plugin simulated process (i.e. surface rainfall-runoff production, ditch infiltration, ...)
    */
    SimProcess_t Process; // TOIMPL to remove, replaced by tags / #framework (2.2.0)

    /**
      Plugin involved method (i.e. morel-seytoux, hayami, ...)
    */
    SimMethod_t Method; // TOIMPL to remove, replaced by tags / #framework (2.2.0)

    /**
      Handled data
    */
    SignatureHandledData HandledData;

    /**
      Handled units graph
    */
    SignatureUnitsGraph HandledUnitsGraph;

    /**
      Time scheduling
    */
    SignatureTimeScheduling TimeScheduling;


    SimulatorSignature() : WareSignature()
    {
      clear();
    }

    virtual ~SimulatorSignature() = default;

    void clear()
    {
      WareSignature::clear();
      Domain.clear(); // TOIMPL to remove / #framework (2.2.x)
      Process.clear(); // TOIMPL to remove / #framework (2.2.x)
      Method.clear(); // TOIMPL to remove / #framework (2.2.x)
      HandledData.clear();
      HandledUnitsGraph.clear();
      TimeScheduling.setAsUndefined();
    }

};


} } // namespaces


#endif /* __OPENFLUID_WARE_SIMULATORSIGNATURE_HPP__ */
