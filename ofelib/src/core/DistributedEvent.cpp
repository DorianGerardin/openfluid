/**
  \file DistributedEvent.cpp
  \brief implements distributed event definition class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "DistributedEvent.h"

#include <iostream>

namespace openfluid { namespace core {

DistributedEvent::DistributedEvent()
{
  m_Infos.clear();
  m_Date = DateTime();
}

// =====================================================================
// =====================================================================


DistributedEvent::DistributedEvent(DateTime Date)
{
  m_Infos.clear();
  m_Date = Date;
}

// =====================================================================
// =====================================================================


DistributedEvent::~DistributedEvent()
{

}


// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoExists(wxString Key)
{
  return !(m_Infos.find(Key) == m_Infos.end());
}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(wxString Key, wxString Value)
{
  wxString TmpValue;

  return (getInfoAsString(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(wxString Key, long Value)
{
 long TmpValue;

  return (getInfoAsLong(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(wxString Key, double Value)
{
  double TmpValue;

  return (getInfoAsDouble(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool DistributedEvent::isInfoEquals(wxString Key, ScalarValue *Value)
{
  ScalarValue TmpValue;

  return (getInfoAsScalarValue(Key,&TmpValue) && (TmpValue == (*Value)));

}


// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsString(wxString Key, wxString *Info)
{

  if (m_Infos.find(Key) == m_Infos.end())
  {
    return false;
  }
  else *Info = m_Infos[Key];

  return true;
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsLong(wxString Key, long *Info)
{

  wxString InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (InfoStr.ToLong(Info)));
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsDouble(wxString Key, double *Info)
{
  wxString InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (InfoStr.ToDouble(Info)));
}

// =====================================================================
// =====================================================================


bool DistributedEvent::getInfoAsScalarValue(wxString Key, ScalarValue *Info)
{
  return getInfoAsDouble(Key,Info);
}


// =====================================================================
// =====================================================================

bool DistributedEvent::addInfo(wxString Key, wxString Info)
{
  if (m_Infos.find(Key) != m_Infos.end())
  {
    return false;
  }
  else m_Infos[Key] = Info;

  return true;
}

// =====================================================================
// =====================================================================


void DistributedEvent::println()
{
  std::cerr << "Date : " << m_Date.getAsISOString().mb_str(wxConvUTF8) << std::endl;
  std::cerr << "Infos : " <<std::endl;

  EventInfosMap::iterator it;
  for (it = m_Infos.begin();it != m_Infos.end();++it)
  {
    std::cerr << " - " << it->first.mb_str(wxConvUTF8) << " = " << it->second.mb_str(wxConvUTF8) << std::endl;
  }
}



} }